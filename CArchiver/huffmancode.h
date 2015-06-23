#ifndef HUFFMANCODE_H_INCLUDED
#define HUFFMANCODE_H_INCLUDED

#define MAX_CODE_LENGTH 64
#define LS_BYTE_MASK 0xFF

typedef struct{
    long long int bits;
    int length;
}huffman_code_t;

void AddBit(huffman_code_t *bitCode, int bit);
huffman_code_t *CreateCode();
void ShiftRight(huffman_code_t *bitcode, int bitCount);
huffman_code_t *CloneCode(huffman_code_t *hcode);

#endif // HUFFMANCODE_H_INCLUDED
