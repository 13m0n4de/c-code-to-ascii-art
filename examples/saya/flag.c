#include <stdio.h>
#include <string.h>

int letters[29] = {
    18414916, 16301615, 31491134, 16303663, 32570431, 1113151,
    32039998, 18415153, 14815374, 15254040, 18128177, 32539681,
    18405233, 18667121, 33080895, 1097263,  17258030, 18136623,
    33061951, 4329631,  33080881, 4539953,  18732593, 18157905,
    4329809,  32575775, 31523934, 16007439, 32505856,
};

void print_text(const char* text) {
    int length = strlen(text);
    for (int row = 0; row < 5; row++) {
        for (int i = 0; i < length; i++) {
            int letter_index = 0;
            if (text[i] >= 'A' && text[i] <= 'Z') {
                letter_index = text[i] - 'A';
            } else if (text[i] >= 'a' && text[i] <= 'z') {
                letter_index = text[i] - 'a';
            } else if (text[i] == '{') {
                letter_index = 26;
            } else if (text[i] == '}') {
                letter_index = 27;
            } else if (text[i] == '_') {
                letter_index = 28;
            } else {
                continue;
            }

            for (int column = 0; column < 5; column++) {
                putchar((letters[letter_index] & (1 << (row * 5 + column)))
                            ? '#'
                            : ' ');
            }
            putchar(' ');
        }
        putchar('\n');
    }
}

void print_flag(void) {
    int letter_index[27] = {5, 11, 0,  6, 26, 12, 14, 4,  28, 14, 1, 5, 20, 18,
                            2, 0,  19, 4, 3,  28, 2,  28, 2,  14, 3, 4, 27};
    for (int row = 0; row < 5; row++) {
        for (int i = 0; i < 27; i++) {
            for (int column = 0; column < 5; column++) {
                putchar((letters[letter_index[i]] & (1 << (row * 5 + column)))
                            ? '#'
                            : ' ');
            }
            putchar(' ');
        }
        putchar('\n');
    }
}

int main(int argc, char* argv[]) {
    char* text = {0};
    if (argc == 1) {
        print_flag();
        printf("Usage: %s <text>\n", argv[0]);
    } else if (argc == 2) {
        text = argv[1];
        print_text(text);
    } else {
        printf("Usage: %s <text>\n", argv[0]);
        return 1;
    }

    return 0;
}
