#ifndef HUFFMAN_H_INCLUDED
#define HUFFMAN_H_INCLUDED

#include <stdio.h>
#include "huffmancode.h"
#include "bufferedbitfile.h"

#define CHARCOUNT 257
#define EOF_CHAR 256
#define COMBINE_NODE -1

typedef struct huffman_node_t{
    int value;
    int count;
    int depth;
    struct huffman_node_t *left, *right, *parent;
}huffman_node_t;

huffman_node_t *CreateNode(int ch);
huffman_node_t *BuildTree(huffman_node_t **nodelist, int count);
int CompareNodes(const void * first, const void * second);
huffman_node_t *CombineNodes(huffman_node_t *left, huffman_node_t *right);
void WriteFrequencyArray(buffered_bit_file_t *outFile, huffman_node_t **freqArr, int count);
void ReadFrequencyArray(FILE *fp, huffman_node_t **freqarr);
void GenerateCodeList(huffman_code_t **codelist, huffman_node_t *huffmanTree);
void EncodeFile(FILE *infile,FILE *outfile);
void DecodeFile(FILE *infile,FILE *outfile);
void FreeHuffmanTree(huffman_node_t *huffmanTree);

#endif // HUFFMAN_H_INCLUDED
