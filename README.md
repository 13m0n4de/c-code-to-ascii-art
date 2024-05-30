# c-code-to-ascii-art

**c-code-to-ascii-art** is a command-line tool that transforms c source code into ASCII art.

It generates a mask by specifying an image and formats the C source code into the shape of the mask.

This tool can be used for [IOCCC](https://www.ioccc.org/) and similar competitions.

> [!Note]
> This is a semi-automatic tool, and the generated code cannot be guaranteed to compile successfully and requires manual review.

## Example

## Build and Run

```
./build.sh
```

```
./code2art
Usage, ./code2art <command> [<args>]
Commands:
  mask <image_path> <mask_path> [output_width] [output_height] [brightness_threshold] [invert]
  code <code_path> <mask_path> <output_path>
```

## References

- [Why do C Programmers Always Obfuscate Their Code?](https://www.youtube.com/watch?v=fJbAiXIum0k)
- [The International Obfuscated C Code Contest](http://www.ioccc.org/)
