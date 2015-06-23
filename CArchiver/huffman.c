#include "huffman.h"
#include <stdlib.h>
#include <stdio.h>

void EncodeFile(FILE *infile,FILE *outfile){
    int ch;
    huffman_node_t *huffmanLeaves[CHARCOUNT], *htree;
    buffered_bit_file_t *bufferedOutFile=CreateBitOutFile(outfile);
    huffman_code_t *huffmanCodes[CHARCOUNT];
    unsigned char *buffer;
    size_t buffersize=1000000, readcount;
    buffer=(unsigned char*)malloc(buffersize*sizeof(unsigned char));
    for(ch=0;ch<CHARCOUNT;ch++){
        huffmanLeaves[ch]=CreateNode(ch);
    }
    while ((readcount=fread(buffer, sizeof(unsigned char), buffersize, infile)) == buffersize)
    {
        for(int i=0;i<buffersize;i++)
            ++huffmanLeaves[buffer[i]]->count;
    }
    for(int i=0;i<readcount;i++)
            ++huffmanLeaves[buffer[i]]->count;
    huffmanLeaves[EOF_CHAR]->value=EOF_CHAR;
    huffmanLeaves[EOF_CHAR]->count=1;
    WriteFrequencyArray(bufferedOutFile, huffmanLeaves, CHARCOUNT);
    htree=BuildTree(huffmanLeaves, CHARCOUNT);
    for(int i=0;i<CHARCOUNT;i++){
        huffmanCodes[i]=CreateCode();
    }
    GenerateCodeList(huffmanCodes, htree);
    rewind(infile);
    while ((readcount=fread(buffer, sizeof(unsigned char), buffersize, infile)) == buffersize)
    {
        for(int i=0;i<buffersize;i++)
            BitFileWriteCode(bufferedOutFile, huffmanCodes[buffer[i]]);
    }
    for(int i=0;i<readcount;i++)
            BitFileWriteCode(bufferedOutFile, huffmanCodes[buffer[i]]);
    BitFileWriteCode(bufferedOutFile, huffmanCodes[EOF_CHAR]);
    BitFileFlush(bufferedOutFile);
    FreeHuffmanTree(htree);
    free(bufferedOutFile);
    free(buffer);
    for(int i=0;i<CHARCOUNT;i++)
        free(huffmanCodes[i]);
}

void DecodeFile(FILE *infile,FILE *outfile){
    int ch;
    huffman_node_t *huffmanLeaves[CHARCOUNT], *htree, *currentnode;
    huffman_code_t *huffmanCodes[CHARCOUNT];
    buffered_bit_file_t *bufferedInFile;
    buffered_bit_file_t *bufferedOutFile=CreateBitOutFile(outfile);
    for(ch=0;ch<CHARCOUNT;ch++){
        huffmanLeaves[ch]=CreateNode(ch);
    }
    huffmanLeaves[EOF_CHAR]->value=EOF_CHAR;
    huffmanLeaves[EOF_CHAR]->count=1;
    ReadFrequencyArray(infile, huffmanLeaves);
    htree=BuildTree(huffmanLeaves, CHARCOUNT);
    bufferedInFile=CreateBitInFile(infile);
    if(htree != huffmanLeaves[EOF_CHAR]){
        for(int i=0;i<CHARCOUNT;i++){
            huffmanCodes[i]=CreateCode();
        }
        GenerateCodeList(huffmanCodes, htree);
        currentnode=htree;
        for(;;){
            int bit=BitFileGetBit(bufferedInFile);
            if(bit != 0)
                currentnode=currentnode->left;
            else
                currentnode=currentnode->right;
            if(currentnode->value != COMBINE_NODE){
                if(currentnode->value == EOF_CHAR){
                    BitFileFlush(bufferedOutFile);
                    break;
                }
                BitFilePutChar(bufferedOutFile, currentnode->value);
                currentnode=htree;
            }
        }
        for(int i=0;i<CHARCOUNT;i++)
            free(huffmanCodes[i]);
    }
    FreeHuffmanTree(htree);
    free(bufferedInFile);
    free(bufferedOutFile);
}

huffman_node_t *CreateNode(int ch){
    huffman_node_t *hnode;
    hnode=(huffman_node_t *)(malloc(sizeof(huffman_node_t)));
    hnode->count=0;
    hnode->value=ch;
    hnode->depth=0;
    hnode->parent=NULL;
    hnode->left=NULL;
    hnode->right=NULL;
    return hnode;
}

int CompareNodes(const void * first, const void * second){
    return (*(huffman_node_t **)first)->count-(*(huffman_node_t **)second)->count;
}

huffman_node_t *CombineNodes(huffman_node_t *left, huffman_node_t *right){
    huffman_node_t *parent;
    parent=(huffman_node_t *)malloc(sizeof(huffman_node_t));
    parent->left=left;
    parent->right=right;
    parent->count=right->count+left->count;
    parent->parent=NULL;
    parent->depth=0;
    parent->value=COMBINE_NODE;
    left->parent=parent;
    right->parent=parent;
    return parent;
}

huffman_node_t *BuildTree(huffman_node_t **nodelist, int count){
    int iter=0,qfront=0,qend=-1;
    huffman_node_t *huffmanQueue[CHARCOUNT], *firstmin, *secondmin;
    qsort(nodelist, count, sizeof(huffman_node_t *), CompareNodes);
    while(iter<count && nodelist[iter]->count==0)
        ++iter;
    if(iter==count-1)
        return nodelist[iter];
    for(;;){
        if(iter==count && qfront==qend)
            break;
        if(qfront>qend || (iter<count && nodelist[iter]->count < huffmanQueue[qfront]->count)){
            firstmin=nodelist[iter];
            ++iter;
        }
        else{
            firstmin=huffmanQueue[qfront];
            ++qfront;
        }
        if(qfront>qend || (iter<count && nodelist[iter]->count < huffmanQueue[qfront]->count)){
            secondmin=nodelist[iter];
            ++iter;
        }
        else{
            secondmin=huffmanQueue[qfront];
            ++qfront;
        }
        ++qend;
        huffmanQueue[qend]=CombineNodes(firstmin,secondmin);
    }
    return huffmanQueue[qfront];
}

void WriteFrequencyArray(buffered_bit_file_t *outFile, huffman_node_t **freqArr, int count){
    int endofarr=0;
    unsigned char value;
    for(int i=0;i<count;i++){
        if(freqArr[i]->value==EOF_CHAR || freqArr[i]->count == 0)
            continue;
        value=(unsigned char)freqArr[i]->value;
        fwrite(&value, sizeof(unsigned char),1,outFile->fp);
        fwrite(&freqArr[i]->count, sizeof(int),1,outFile->fp);
    }
    value=0;
    fwrite(&value, sizeof(unsigned char),1,outFile->fp);
    fwrite(&endofarr, sizeof(int),1,outFile->fp);
}

void ReadFrequencyArray(FILE *fp, huffman_node_t **freqarr){
    int count;
    unsigned  char value;
    for(;;){
        if(fread(&value,sizeof(unsigned char),1,fp) != 1)
            exit(0);
        if(fread(&count,sizeof(int),1,fp) !=1)
            exit(0);
        if(value == 0 && count == 0)
            break;
        freqarr[value]->count=count;
    }
}


void GenerateCodeList(huffman_code_t **codelist, huffman_node_t *huffmanTree){
    huffman_node_t *codeStack[CHARCOUNT],*current=huffmanTree;
    huffman_code_t *code=CreateCode();
    int stacktop=-1, done=0,depth=0,depthdiff=0;
    while(done != 1){
        if(current!=NULL){
            ++stacktop;
            codeStack[stacktop]=current;
            if(current->parent != NULL)
                current->depth=current->parent->depth+1;
            current=current->left;
            ++depth;
            AddBit(code,1);
        }
        else{
            if(stacktop>-1){
                current=codeStack[stacktop];
                --stacktop;
                depthdiff=depth-current->depth;
                if(depthdiff < 0)
                    depthdiff=-depthdiff;
                ShiftRight(code, depthdiff);
                if(current->value != COMBINE_NODE){
                    codelist[current->value]=CloneCode(code);
                }
                depth=current->depth+1;
                AddBit(code,0);
                current=current->right;
            }
            else
                done=1;
        }
    }
    free(code);
}

void FreeHuffmanTree(huffman_node_t *huffmanTree){
    if(huffmanTree == NULL)
        return;
    FreeHuffmanTree(huffmanTree->left);
    FreeHuffmanTree(huffmanTree->right);
    free(huffmanTree);
}
