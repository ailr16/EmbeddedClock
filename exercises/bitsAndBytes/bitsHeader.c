#include <stdio.h>
#include "bitsHeader.h"

void printB(unsigned char byte){
    unsigned char mask = 1;
    unsigned char result[8];

    for(int i = 0; i < 8; i++){
        if ((byte & mask) == 0){
            result[i] = 0;
        }
        else{
            result[i] = 1;
        }
        mask = mask << 1;
    }
    
    for(int i = 7; i >= 0; i--){
        printf("%d", result[i]);
    }

    printf("\n");
}

unsigned char ShiftBitRigth( unsigned char byte, unsigned char shift ){
    if( shift > 7){
        return byte;
    }
    else{
        return byte >> shift;
    }
}

unsigned char ShiftBitLeft( unsigned char byte, unsigned char shift ){
    if( shift > 7){
        return byte;
    }
    else{
        return byte << shift;
    }
}

unsigned char SetBitMask( unsigned char byte, unsigned char mask ){
    byte = byte | mask;
    return byte;
}

unsigned char ClearBitMask( unsigned char byte, unsigned char mask ){
    byte = byte & ~mask;
    return byte;
}

unsigned char ToggleBitMask( unsigned char byte, unsigned char mask ){
    byte = byte ^ mask;
    return byte;
}

unsigned char SetBit( unsigned char byte, unsigned char bit ){
    unsigned char mask = 0x01;
    if( bit > 7){
        return byte;
    }
    else{
        mask = mask << bit;
        return byte | mask;
    }   
}

unsigned char ClearBit( unsigned char byte, unsigned char bit ){
    unsigned char mask = 0x01;
    if( bit > 7){
        return byte;
    }
    else{
        mask = mask << bit;
        return byte & ~mask;
    }   
}

unsigned char ToggleBit( unsigned char byte, unsigned char bit ){
    if( bit > 7){
        return byte;
    }
    else{
        return byte ^ (0x01 << bit);
    }
}

unsigned char GetBit( unsigned char byte, unsigned char bit ){
    unsigned char mask = 0x01;
    if( bit > 7){
        return byte;
    }
    else{
        mask = mask << bit;
        if( (byte & mask) == 0){
            return 0;
        }
        else{
            return 1;
        }
    }
}