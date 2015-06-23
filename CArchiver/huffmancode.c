#include "huffmancode.h"
#include <stdlib.h>

void AddBit(huffman_code_t *bitCode, int bit){
    bitCode->bits <<= 1;
    bitCode->bits |= bit;
    ++bitCode->length;
}

huffman_code_t *CreateCode(){
    huffman_code_t *hcode;
    hcode=(huffman_code_t *)malloc(sizeof(huffman_code_t));
    hcode->bits=0;
    hcode->length=0;
    return hcode;
}

void ShiftRight(huffman_code_t *bitCode, int bitCount){
    if(bitCount >= bitCode->length){
        bitCode->bits=0;
        bitCode->length=0;
    }
    else{
        bitCode->bits >>= bitCount;
        bitCode->length -= bitCount;
    }
}

huffman_code_t *CloneCode(huffman_code_t *hcode){
    huffman_code_t *clonecode;
    clonecode=(huffman_code_t *)malloc(sizeof(huffman_code_t));
    clonecode->bits=hcode->bits;
    clonecode->length=hcode->length;
    return clonecode;
}
