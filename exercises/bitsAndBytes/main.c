#include <stdio.h>
#include "bitsHeader.h"

/*
    Tests for byte and bits handling functions 
*/

int main(void){
    unsigned char var = 0xf0;

    printf("shit rigth 0xf0 -> 4\n");
    printB(ShiftBitRigth(var, 8));
    printB(ShiftBitRigth(var, 4));

    printf("\nshit left 0x0f <- 4\n");
    var = 0x0f;
    printB(ShiftBitLeft(var, 8));
    printB(ShiftBitLeft(var, 3));

    printf("\nset mask 0xa0, byte 0x01\n");
    var = 0x01;
    unsigned char mask = 0xA0;
    printB(var);
    printB(mask);
    printB(SetBitMask(var, mask));

    printf("\nclear mask 0xa0, byte 0xff\n");
    var = 0xFF;
    mask = 0x03;
    printB(var);
    printB(mask);
    printB(ClearBitMask(var, mask));

    printf("\ntogle mask 0xa0, byte 0x16\n");
    var = 0x16;
    mask = 0x0F;
    printB(var);
    printB(mask);
    printB(ToggleBitMask(var, mask));

    printf("\nset bit 6 in byte 0x01\n");
    var = 0x3;
    printB(var);
    printB(SetBit(var, 3));

    printf("\nclear bit 1 in byte 0x0F\n");
    var = 0x0F;
    printB(var);
    printB(ClearBit(var, 1));

    printf("\ntoggle bit 1 in byte 0x0F\n");
    var = 0x0F;
    printB(var);
    printB(ToggleBit(var, 1));

    printf("\nget bit 1 in byte 0x0F\n");
    var = 0x0F;
    printB(var);
    printB(GetBit(var, 3));
}