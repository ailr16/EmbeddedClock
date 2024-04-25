#include <stdio.h>
#include <string.h>
#include "buffer.h"

void Buffer_Init( Buffer_t *hbuffer ){
    hbuffer->Head = 0;  //write
    hbuffer->Tail = 0;  //read
    hbuffer->Full = 0;
    hbuffer->Empty = 1;
    hbuffer->SavedElements = 0;
}

void Buffer_Write( Buffer_t *hbuffer, unsigned char data ){
    if(hbuffer->SavedElements == hbuffer->Elements){
        hbuffer->Full = 1;
    }
    else{
        hbuffer->Buffer[hbuffer->Head] = data;
        hbuffer->SavedElements++;
        hbuffer->Head++;
        hbuffer->Empty = 0;
        if(hbuffer->Head == hbuffer->Elements){
            hbuffer->Head = 0;
        }
    }
}

unsigned char Buffer_Read( Buffer_t *hbuffer ){
    unsigned char aux = 0;
    if(hbuffer->SavedElements == 0){
        hbuffer->Empty = 1;
    }
    else{
        aux = hbuffer->Buffer[hbuffer->Tail];
        hbuffer->SavedElements--;
        if(hbuffer->SavedElements == 0){
            hbuffer->Empty = 1;
        }
        hbuffer->Tail++;
        hbuffer->Full = 0;
        if(hbuffer->Tail == hbuffer->Elements){
            hbuffer->Tail = 0;
        }
    }
    return aux;
}

unsigned char Buffer_IsEmpty( Buffer_t *hbuffer ){    
    return hbuffer->Empty;
}
