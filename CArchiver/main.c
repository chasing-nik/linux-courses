#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int main(int argc, char *argv[])
{
    FILE *infile;
    FILE *outfile;
    if(argc == 4){
        infile=fopen(argv[1], "rb");
        outfile=fopen(argv[3], "wb");
        if(infile != NULL){
            if(strcmp(argv[2],"-c") == 0)
                EncodeFile(infile, outfile);
            else if(strcmp(argv[2], "-x") == 0)
                DecodeFile(infile, outfile);
        }
        fclose(infile);
        fclose(outfile);
    }
    return 0;
}
