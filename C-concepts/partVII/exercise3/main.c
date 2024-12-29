#include <stdio.h>

struct StructPack{
    unsigned long word;     //8bytes
    unsigned char byte;     //1byte
    unsigned short hword;   //2bytes
};

void UnpackingBytes( struct StructPack *message, unsigned char *array);
void printUCharArray(unsigned char array[], int size);

int main(void){
    unsigned char testArray[7];

    struct StructPack a = {.word = 0x1011121356657887, 
                            .byte = 0x22,
                            .hword = 0x3883};    

    UnpackingBytes(&a, testArray);
    printUCharArray(testArray, 7);
}

void UnpackingBytes( struct StructPack *message, unsigned char *array){
    union{
        unsigned long messageWord;
        unsigned char arrayWord[4];
    } getBytesFromWord = {.messageWord = message->word};

    union{
        unsigned short messagehWord;
        unsigned char arrayhWord[2];
    } getBytesFromhWord = {.messagehWord = message->hword};

    array[0] = getBytesFromWord.arrayWord[0];
    array[1] = getBytesFromWord.arrayWord[1];
    array[2] = getBytesFromWord.arrayWord[2];
    array[3] = getBytesFromWord.arrayWord[3];
    array[4] = message->byte;
    array[5] = getBytesFromhWord.arrayhWord[0];
    array[6] = getBytesFromhWord.arrayhWord[1];
}

void printUCharArray(unsigned char array[], int size){
    for(int i = (size - 1); i >= 0; i--){
        printf("%X", array[i]);
    }
    printf("\n");
}