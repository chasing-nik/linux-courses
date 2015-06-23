#include "bufferedbitfile.h"
#include <stdlib.h>


buffered_bit_file_t *CreateBitOutFile(FILE *fp){
    buffered_bit_file_t *bitfile;
    bitfile=(buffered_bit_file_t *)malloc(sizeof(buffered_bit_file_t));
    bitfile->fp=fp;
    bitfile->current_block_length=0;
    bitfile->block_count=1;
    bitfile->current_reading_block=-1;
    return bitfile;
}

buffered_bit_file_t *CreateBitInFile(FILE *fp){
    buffered_bit_file_t *bitfile;
    bitfile=(buffered_bit_file_t *)malloc(sizeof(buffered_bit_file_t));
    bitfile->fp=fp;
    bitfile->current_block_length=8;
    bitfile->block_count=fread(bitfile->buffer, sizeof(unsigned char), MAX_BUFFER_SIZE, bitfile->fp);
    bitfile->current_reading_block=0;
    return bitfile;
}

void BitFileWriteCode(buffered_bit_file_t *bitFile, huffman_code_t *code){
    unsigned char tempBuf=0;
    long long int leftBits=0, mask=0;
    int shiftcount;
    if(bitFile->current_block_length+code->length <= MAX_BLOCK_SIZE){
        tempBuf=0;
        tempBuf |= code->bits;
        shiftcount=(MAX_BLOCK_SIZE-bitFile->current_block_length)-code->length;
        tempBuf <<= shiftcount;
        bitFile->buffer[bitFile->block_count-1] |= tempBuf;
        bitFile->current_block_length += code->length;
    }
    else{
        shiftcount=code->length-(MAX_BLOCK_SIZE-bitFile->current_block_length);
        leftBits=code->bits >> shiftcount;
        tempBuf=0;
        tempBuf |=leftBits;
        bitFile->buffer[bitFile->block_count-1] |= tempBuf;
        ++bitFile->block_count;
        bitFile->current_block_length=0;
        if(bitFile->block_count > MAX_BUFFER_SIZE){
            fwrite(bitFile->buffer, sizeof(unsigned char), MAX_BUFFER_SIZE, bitFile->fp);
            for(int i=0;i<MAX_BUFFER_SIZE;i++)
                bitFile->buffer[i]=0;
            bitFile->block_count=1;
        }
        for(int i=1;shiftcount>=MAX_BLOCK_SIZE; i++){
            shiftcount-=MAX_BLOCK_SIZE;
            leftBits=(code->bits >> shiftcount) & LS_BYTE_MASK;
            tempBuf=0;
            tempBuf |= leftBits;
            bitFile->buffer[bitFile->block_count-1] |= tempBuf;
            ++bitFile->block_count;
            if(bitFile->block_count > MAX_BUFFER_SIZE){
                fwrite(bitFile->buffer, sizeof(unsigned char), MAX_BUFFER_SIZE, bitFile->fp);
                for(int i=0;i<MAX_BUFFER_SIZE;i++)
                    bitFile->buffer[i]=0;
                bitFile->block_count=1;
            }
        }
        leftBits=code->bits;
        for(int i=1;i<=shiftcount;i++){
            mask <<= 1;
            mask |= 1;
        }
        leftBits &= mask;
        leftBits <<= MAX_BLOCK_SIZE-shiftcount;
        tempBuf=0;
        tempBuf |=leftBits;
        bitFile->buffer[bitFile->block_count-1] |= tempBuf;
        bitFile->current_block_length = shiftcount;
    }
}

int BitFileGetBit(buffered_bit_file_t *bitFile){
    int returnvalue;
    if(bitFile->current_block_length == 0){
        ++bitFile->current_reading_block;
        if(bitFile->current_reading_block >= bitFile->block_count){
            bitFile->block_count=fread(bitFile->buffer, sizeof(unsigned char), MAX_BUFFER_SIZE, bitFile->fp);
            if(bitFile->block_count == 0)
                return -1;
            bitFile->current_block_length=MAX_BLOCK_SIZE;
            bitFile->current_reading_block=0;
        }
        bitFile->current_block_length=MAX_BLOCK_SIZE;
    }
    returnvalue=bitFile->buffer[bitFile->current_reading_block] & MSB_MASK;
    bitFile->buffer[bitFile->current_reading_block] <<= 1;
    --bitFile->current_block_length;
    return returnvalue;
}

void BitFileFlush(buffered_bit_file_t *bitFile){
    if(bitFile->current_block_length == 0)
        fwrite(bitFile->buffer, sizeof(unsigned char), bitFile->block_count-1, bitFile->fp);
    else{
        fwrite(bitFile->buffer, sizeof(unsigned char), bitFile->block_count, bitFile->fp);
    }
}

void BitFilePutChar(buffered_bit_file_t *bitFile, unsigned char ch){
    int shiftcount;
    unsigned char leftBits, mask=0;
    if(bitFile->current_block_length == 0){
        bitFile->buffer[bitFile->block_count-1]=ch;
        ++bitFile->block_count;
        if(bitFile->block_count > MAX_BUFFER_SIZE){
            fwrite(bitFile->buffer, sizeof(unsigned char), MAX_BUFFER_SIZE, bitFile->fp);
            for(int i=0;i<MAX_BUFFER_SIZE;i++)
                bitFile->buffer[i]=0;
            bitFile->block_count=1;
        }
    }
    else{
        shiftcount=bitFile->current_block_length;
        leftBits=ch >> shiftcount;
        bitFile->buffer[bitFile->block_count-1] |= leftBits;
        ++bitFile->block_count;
        bitFile->current_block_length=0;
        if(bitFile->block_count > MAX_BUFFER_SIZE){
            fwrite(bitFile->buffer, sizeof(unsigned char), MAX_BUFFER_SIZE, bitFile->fp);
            for(int i=0;i<MAX_BUFFER_SIZE;i++)
                bitFile->buffer[i]=0;
            bitFile->block_count=1;
        }
        leftBits=ch;
        for(int i=1;i<=shiftcount;i++){
            mask <<= 1;
            mask |= 1;
        }
        leftBits &= mask;
        leftBits <<= MAX_BLOCK_SIZE-shiftcount;
        bitFile->buffer[bitFile->block_count-1] |= leftBits;
        bitFile->current_block_length = shiftcount;
    }
}

