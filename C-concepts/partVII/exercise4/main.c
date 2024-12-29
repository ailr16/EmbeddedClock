#include <stdio.h>

struct StructPack{
    unsigned long word1;
    unsigned long word2;
    unsigned char byte;
    unsigned short hword1;
};

void PackingBytes( unsigned char *array, struct StructPack *message );

int main(void){
    unsigned char testArray[11] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};
    struct StructPack a;

    PackingBytes(testArray, &a);
    printf("word1 = %lX\n", a.word1);
    printf("word2 = %lX\n", a.word2);
    printf("byte = %lX\n", a.byte);
    printf("hword1 = %lX\n", a.hword1);
}

void PackingBytes( unsigned char *array, struct StructPack *message ){
    union{
        unsigned long word1;
        unsigned char messageWord1[4];
    } packWord1 = {.messageWord1[0] = array[0],
                   .messageWord1[1] = array[1],
                   .messageWord1[2] = array[2],
                   .messageWord1[3] = array[3]
                  };

    union{
        unsigned long word2;
        unsigned char messageWord2[4];
    } packWord2 = {.messageWord2[0] = array[7],
                   .messageWord2[1] = array[8],
                   .messageWord2[2] = array[9],
                   .messageWord2[3] = array[10]
                  };

    union{
        unsigned short hword1;
        unsigned char messagehword1[2];
    } packHword1 = {.messagehword1[0] = array[5],
                    .messagehword1[1] = array[6]
                   };
    
    message->word1 = packWord1.word1;
    message->word2 = packWord2.word2;
    message->hword1 = packHword1.hword1;
    message->byte = array[4];
}