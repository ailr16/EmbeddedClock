#include <stdio.h>
#include <string.h>
#include "pointersHeader.h"

unsigned char average( unsigned char *array, unsigned char size ){
    unsigned char sum = 0;
    for(int i = 0; i < size; i++){
        sum += *array++;
    }
    return sum / size;
}

void arrayCopy( unsigned char *arrayA, unsigned char *arrayB, unsigned char size ){
    for(int i = 0; i < size; i++){
        *arrayB++ = *arrayA++;
    }
}

unsigned char arrayCompare( unsigned char *arrayA, unsigned char *arrayB, unsigned char size ){
    unsigned char flagDifferent = 0;
    for(int i = 0; i < size; i++){
        if(*arrayB++ != *arrayA++){
            flagDifferent = 1;
            break;
        }
    }
    return flagDifferent;
}

unsigned short largest( unsigned short int *array, unsigned char size ){
    unsigned short max = *array;
    for(int i = 0; i < size; i++){
        if(*array > max){
            max = *array;
        }
        array++;
    }
    return max;
}

void sortArray( unsigned short int *array, unsigned char size ){
    unsigned short int i, j, aux;
    
    aux = 0;

    for(j = 1; j < size - 1 ; j++){
        for(i = 0; i < (size - j); i++){
            if(*(array + i) > *(array + i + 1)){
                aux = *(array + i + 1);
                *(array + i + 1) = *(array + i);
                *(array + i) = aux;
            }
        }
    }
}

void TimeString( char *string, unsigned char hours, unsigned char minutes, unsigned char seconds){
    *string = (hours / 10) + 48;
    *(string + 1) = (hours % 10) + 48;
    *(string + 2) = ':';
    *(string + 3) = (minutes / 10) + 48;
    *(string + 4) = (minutes % 10) + 48;
    *(string + 5) = ':';
    *(string + 6) = (seconds / 10) + 48;
    *(string + 7) = (seconds % 10) + 48;
    *(string + 8) = '\0';
}

void DateString( char *string, unsigned char month, unsigned char day, unsigned short int year, unsigned char weekday ){
    
    switch( month ){
        case 1:
            *string = 'J';
            *(string + 1) = 'a';
            *(string + 2) = 'n';
        break;
        case 2:
            *string = 'F';
            *(string + 1) = 'e';
            *(string + 2) = 'b';
        break;
        case 3:
            *string = 'M';
            *(string + 1) = 'a';
            *(string + 2) = 'r';
        break;
        case 4:
            *string = 'A';
            *(string + 1) = 'p';
            *(string + 2) = 'r';
        break;
        case 5:
            *string = 'M';
            *(string + 1) = 'a';
            *(string + 2) = 'y';
        break;
        case 6:
            *string = 'J';
            *(string + 1) = 'u';
            *(string + 2) = 'n';
        break;
        case 7:
            *string = 'J';
            *(string + 1) = 'u';
            *(string + 2) = 'l';
        break;
        case 8:
            *string = 'A';
            *(string + 1) = 'u';
            *(string + 2) = 'g';
        break;
        case 9:
            *string = 'S';
            *(string + 1) = 'e';
            *(string + 2) = 'p';
        break;
        case 10:
            *string = 'O';
            *(string + 1) = 'c';
            *(string + 2) = 't';
        break;
        case 11:
            *string = 'N';
            *(string + 1) = 'o';
            *(string + 2) = 'v';
        break;
        case 12:
            *string = 'D';
            *(string + 1) = 'e';
            *(string + 2) = 'c';
        break;
        default:
    };

    *(string + 3) = ' ';

    *(string + 4) = (day / 10) + 48; 
    *(string + 5) = (day % 10) + 48;

    *(string + 6) = ',';
    *(string + 7) = ' ';

    *(string + 8) = (year / 1000) + 48;
    *(string + 9) = (year % 1000) / 100 + 48;
    *(string + 10) = ((year % 1000) % 100) / 10 + 48;
    *(string + 11) = (((year % 1000) % 100) % 10) + 48;
    *(string + 12) = ' ';


    switch(weekday){
        case 0:
            *(string + 13) = 'M';
            *(string + 14) = 'o';
        break;
        case 1:
            *(string + 13) = 'T';
            *(string + 14) = 'u';
        break;
        case 2:
            *(string + 13) = 'W';
            *(string + 14) = 'e';
        break;
        case 3:
            *(string + 13) = 'T';
            *(string + 14) = 'h';
        break;
        case 4:
            *(string + 13) = 'F';
            *(string + 14) = 'r';
        break;
        case 5:
            *(string + 13) = 'S';
            *(string + 14) = 'a';
        break;
        case 6:
            *(string + 13) = 'S';
            *(string + 14) = 'u';
        break;
        default:
    }

}