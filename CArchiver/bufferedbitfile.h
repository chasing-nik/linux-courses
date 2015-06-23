#ifndef BUFFEREDBITFILE_H_INCLUDED
#define BUFFEREDBITFILE_H_INCLUDED

#include "huffmancode.h"
#include <stdio.h>

#define MAX_BUFFER_SIZE 1000000
#define MAX_BLOCK_SIZE 8
#define MSB_MASK 0x80

typedef struct{
    FILE *fp;
    unsigned char buffer[MAX_BUFFER_SIZE];
    int current_block_length;
    int block_count;
    int current_reading_block;
}buffered_bit_file_t;

void BitFileWriteCode(buffered_bit_file_t *bitFile, huffman_code_t *code);
buffered_bit_file_t *CreateBitOutFile(FILE *fp);
buffered_bit_file_t *CreateBitInFile(FILE *fp);
int BitFileGetBit(buffered_bit_file_t *bitFile);
void BitFileFlush(buffered_bit_file_t *bitFile);
void BitFilePutChar(buffered_bit_file_t *bitFile, unsigned char ch);

#endif // BUFFEREDBITFILE_H_INCLUDED
