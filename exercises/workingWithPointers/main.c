#include <stdio.h>
#include "pointersHeader.h"

#define ARRAY_SIZE 10
#define ARRAY_SIZE_2 20

void printArray(unsigned char array[], int size);
void printArrayShort(unsigned short int array[], int size);
void printHour(char hour[]);
void printDate(char date[]);

int main(void){
    unsigned char testArray[ARRAY_SIZE] = {10, 10, 10, 10, 10, 20, 10, 10, 10, 10};
    unsigned char testArray2[ARRAY_SIZE];
    unsigned char testArray3[ARRAY_SIZE] = {10, 10, 10, 10, 10, 30, 10, 10, 10, 10};
    unsigned short testArrayShort[ARRAY_SIZE] = {50, 2, 1, 11, 10, 30, 10, 20, 10, 10};
    unsigned short testArrayShort2[ARRAY_SIZE_2] = {50, 2, 1, 11, 10, 30, 10, 20, 444, 10, 99, 8, 23, 12, 55, 16, 18, 19, 20, 21};
    char hour[9];
    char date[16];

    printArray(testArray, ARRAY_SIZE);
    printArray(testArray2, ARRAY_SIZE);
    printf("%d \n", average(testArray, ARRAY_SIZE));
    arrayCopy(testArray, testArray2, ARRAY_SIZE);
    printArray(testArray2, ARRAY_SIZE);
    printf("%d\n", arrayCompare(testArray, testArray3, ARRAY_SIZE));
    printf("%d\n", largest(testArrayShort, ARRAY_SIZE));
    printArrayShort(testArrayShort2, ARRAY_SIZE_2);
    sortArray(testArrayShort2, ARRAY_SIZE_2);
    printArrayShort(testArrayShort2, ARRAY_SIZE_2);
   
    TimeString(hour, 23, 30, 0);
    printHour(hour);

    DateString(date, 12, 16, 2001, 5);
    printDate(date);
    DateString(date, 1, 7, 1966, 3);
    printDate(date);
}

void printArray(unsigned char array[], int size){
    for(int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

void printArrayShort(unsigned short int array[], int size){
    for(int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

void printHour(char hour[]){
    for(int i = 0; i < 9; i++){
        printf("%c", hour[i]);
    }
    printf("\n");
}

void printDate(char date[]){
    for(int i = 0; i < 15; i++){
        printf("%c", date[i]);
    }
    printf("\n");
}