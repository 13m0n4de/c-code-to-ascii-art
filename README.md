# c-code-to-ascii-art

**c-code-to-ascii-art** is a command-line tool that transforms c source code into ASCII art.

It generates a mask by specifying an image and formats the C source code into the shape of the mask.

This tool can be used for [IOCCC](https://www.ioccc.org/) and similar competitions.

> \[!Note\]
> This is a semi-automatic tool, and the generated code cannot be guaranteed to compile successfully and requires manual review.

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

## Example

The example file [saya.c](./examples/saya.c) is a command line tool that converts text to ASCII art for the [SVUCTF-SPRING-2024](https://github.com/SVUCTF/SVUCTF-SPRING-2024) competition. When run without any parameters, it prints a FLAG string.

```c
                          L[29]={18414916,16301615
                       ,31491134,16303663,32570431
                    ,1113151,32039998,18415153,14815374
                  ,15254040,18128177,32539681,18405233,
                18667121,33080895,1097263,17258030, 18136623
               ,33061951,4329631,33080881,4539953,18732593,18157905
             ,4329809,32575775,31523934,16007439,32505856 ,};F
            [27] ={ 5,11,0,    6,26,12,14,4,28,14,1,5  ,20,18,2,
          0,19,  4, 3,28,2,     28,2 ,14,3,4,27};row,i ,column,length
        ,idx;main(int argc,      char* argv[]){char*text={ 0};if(argc
      ==1){for( row=0;row          <5;   row++){for(i= 0;i <27;i++){for
    (column=0;  column<               5;    column++){ putchar((
               L[F [i]]                        &( 1<<( row *
               5+column                           )))? '#'
               :' ');}putchar                     (' ');}putchar
                ('\n');                           }printf(
                "Usage: %s <text>\n"              ,argv[0
                ]) ;}else                         if(argc
                ==2){text                        =argv [1
                ];length =                       strlen(text
                );for(row=0;                   row<5; row
                ++) {for(i=0;i               <length; i++
                ){if (text[i]>=           'A' && text [i]
               <='Z'){idx =text[         i]-'A';}else if(text
               [i]>= 'a'   &&text         [i]<='z'){ idx=text
               [i]-'a';}else if(           text [i]=='{'){
               idx=26  ;}else                if (text[i]==
              '}')     {idx                     =27   ;}else
              if        (text                  [i]       ==
             '_')        {idx                  =28        ;}
             else         {continue           ;}for       (column
            =0;column      <5;                column      ++)
            {putchar         ((              L[           idx]
           &(1<<(              row                        *5+column
           )))?'#'                                       :' ');}
           putchar                                       (' ');}
       putchar('\n');}}else{printf("Usage: %s <text>\n",argv[0]);}}
```

```
gcc flag.saya.c -std=c89 -o flag && ./flag
```

## References

- [Why do C Programmers Always Obfuscate Their Code?](https://www.youtube.com/watch?v=fJbAiXIum0k)
- [The International Obfuscated C Code Contest](http://www.ioccc.org/)
- [defg43/ascshape](https://github.com/defg43/ascshape)
