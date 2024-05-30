#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_C_LEXER_IMPLEMENTATION
#include "stb_c_lexer.h"

#include <stdio.h>
#include <string.h>

void escape_and_print(FILE* output_file, const char* str, int len) {
    for (int i = 0; i < len; i++) {
        switch (str[i]) {
            case '\n':
                fprintf(output_file, "\\n");
                break;
            case '\t':
                fprintf(output_file, "\\t");
                break;
            case '\r':
                fprintf(output_file, "\\r");
                break;
            default:
                fputc(str[i], output_file);
        }
    }
}

void print_token_to_file(stb_lexer* lexer, FILE* output_file) {
    int length = lexer->where_lastchar - lexer->where_firstchar + 1;
    escape_and_print(output_file, lexer->where_firstchar, length);
}

int read_entire_file(const char* path, char** content, long* size) {
    FILE* file = fopen(path, "r");
    if (file == NULL)
        return 0;

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    rewind(file);

    *content = (char*)malloc(*size + 1);
    if (*content == NULL) {
        fclose(file);
        return 0;
    }

    fread(*content, 1, *size, file);
    (*content)[*size] = '\0';
    fclose(file);

    return 1;
}

void handle_error(const char* error_message, void* memory) {
    fprintf(stderr, "%s\n", error_message);
    if (memory != NULL)
        free(memory);
    exit(1);
}

typedef struct {
    const char* image_path;
    const char* mask_path;
    int output_height;
    int output_width;
    float brightness_threshold;
    int invert;
} MaskParams;

typedef struct {
    const char* code_path;
    const char* mask_path;
    const char* output_path;
} CodeParams;

void parse_mask_params(int argc, char** argv, MaskParams* params) {
    params->output_height = 0;
    params->output_width = 0;
    params->brightness_threshold = 0.5f;
    params->invert = 0;

    if (argc < 4 || argc > 8) {
        fprintf(
            stderr,
            "Usage: %s mask <image_path> <mask_path> "
            "[output_width] [output_height] [brightness_threshold] [invert]\n",
            argv[0]);
        exit(1);
    }

    params->image_path = argv[2];
    params->mask_path = argv[3];
    if (argc > 4)
        params->output_width = atoi(argv[4]);
    if (argc > 5)
        params->output_height = atoi(argv[5]);
    if (argc > 6)
        params->brightness_threshold = atof(argv[6]);
    if (argc > 7)
        params->invert = atoi(argv[7]);
}

void parse_code_params(int argc, char** argv, CodeParams* params) {
    if (argc != 5) {
        fprintf(stderr,
                "Usage: %s code <code_path> <mask_path> <output_path>\n",
                argv[0]);
        exit(1);
    }

    params->code_path = argv[2];
    params->mask_path = argv[3];
    params->output_path = argv[4];
}

void generate_mask(const MaskParams* params) {
    int width, height, channels;
    stbi_uc* pixels =
        stbi_load(params->image_path, &width, &height, &channels, 0);
    if (pixels == NULL) {
        handle_error("Failed to load image", NULL);
    }

    int output_width = params->output_width ? params->output_width : width;
    int output_height = params->output_height ? params->output_height : height;

    FILE* mask_file = fopen(params->mask_path, "w");
    if (mask_file == NULL) {
        stbi_image_free(pixels);
        handle_error("Failed to create mask file", NULL);
    }

    float w_scale = (float)width / output_width;
    float h_scale = (float)height / output_height;

    for (int y = 0; y < output_height; y++) {
        for (int x = 0; x < output_width; x++) {
            int img_x = (int)(x * w_scale);
            int img_y = (int)(y * h_scale);

            stbi_uc* pixel = &pixels[(img_y * width + img_x) * channels];

            float brightness = 0;
            for (int i = 0; i < channels; i++) {
                brightness += pixel[i];
            }
            brightness /= (channels * 255.0);

            if (brightness < params->brightness_threshold) {
                fprintf(mask_file, params->invert ? " " : "#");
            } else {
                fprintf(mask_file, params->invert ? "#" : " ");
            }
        }
        fprintf(mask_file, "\n");
    }

    fclose(mask_file);
    stbi_image_free(pixels);
}

void generate_code(const CodeParams* params) {
    char* code = NULL;
    long code_size = 0;
    if (!read_entire_file(params->code_path, &code, &code_size)) {
        handle_error("Failed to open code file", code);
    }

    FILE* mask_file = fopen(params->mask_path, "r");
    if (mask_file == NULL) {
        handle_error("Failed to open mask file", code);
    }

    FILE* output_file = fopen(params->output_path, "w");
    if (output_file == NULL) {
        handle_error("Failed to open output file", code);
    }

    stb_lexer lexer;
    char buffer[1024];
    stb_c_lexer_init(&lexer, code, code + code_size, buffer, sizeof(buffer));

    char mask_line[1024];
    while (fgets(mask_line, sizeof(mask_line), mask_file)) {
        for (int i = 0; mask_line[i] != '\0' && mask_line[i] != '\n';) {
            if (mask_line[i] == '#') {
                if (!stb_c_lexer_get_token(&lexer)) {
                    fprintf(output_file, " ");
                    i++;
                } else {
                    print_token_to_file(&lexer, output_file);
                    i += lexer.where_lastchar - lexer.where_firstchar + 1;
                }
            } else {
                fprintf(output_file, " ");
                i++;
            }
        }
        fprintf(output_file, "\n");
    }

    while (stb_c_lexer_get_token(&lexer)) {
        print_token_to_file(&lexer, output_file);
    }

    free(code);
    fclose(mask_file);
    fclose(output_file);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr,
                "Usage, %s <command> [<args>]\n"
                "Commands:\n"
                "  mask <image_path> <mask_path> [output_width] "
                "[output_height] [brightness_threshold] [invert]\n"
                "  code <code_path> <mask_path> <output_path>\n",
                argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "mask") == 0) {
        MaskParams params;
        parse_mask_params(argc, argv, &params);
        generate_mask(&params);
    } else if (strcmp(argv[1], "code") == 0) {
        CodeParams params;
        parse_code_params(argc, argv, &params);
        generate_code(&params);
    } else {
        fprintf(stderr,
                "Unknown command: %s\n"
                "Usage: %s <command> [<args>]\n"
                "Commands:\n"
                "  mask <image_path> <mask_path> [output_width] "
                "[output_height] [brightness_threshold] [invert]\n"
                "  code <code_path> <mask_path> <output_path>\n",
                argv[1], argv[0]);
        return 1;
    }

    return 0;
}
