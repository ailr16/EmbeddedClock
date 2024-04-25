#include <stdio.h>
#include "buffer.h"

/*
    Tests for circular buffer
*/

int main(void){
    unsigned char bufferArray[18];
    unsigned long N = 18;

    Buffer_t bufferTest;
    bufferTest.Buffer = bufferArray;
    bufferTest.Elements = N;

    Buffer_Init(&bufferTest);

    printf("First cycle\n");

    for(unsigned char i=0; i<28; i++){
        Buffer_Write(&bufferTest, 10+i);
    }

    while(Buffer_IsEmpty(&bufferTest) == 0){
        printf("%X\n", Buffer_Read(&bufferTest));
    }

    printf("\nSecond cycle\n");

    for(unsigned char i=0; i<18; i++){
        Buffer_Write(&bufferTest, 20+i);
    }

    while(Buffer_IsEmpty(&bufferTest) == 0){
        printf("%X\n", Buffer_Read(&bufferTest));
    }
}