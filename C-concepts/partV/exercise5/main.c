#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 11

void InitializeArray(int *array, int size);
void PrintResults(int *array, int size);

int main(void){
    int dice1, dice2, sum;
    int results[ARRAY_SIZE];

    InitializeArray(results, ARRAY_SIZE);

    for(int i = 0; i < 3600; i++){
        dice1 = (rand() % 6) + 1;
        dice2 = (rand() % 6) + 1;
        sum = dice1 + dice2;
        results[sum - 2]++;
    }
    
    PrintResults(results, ARRAY_SIZE);
}

void InitializeArray(int *array, int size){
    for(int i = 0; i < size; i++){
        array[i] = 0;
    }
}

void PrintResults(int *array, int size){
    printf("Sum\tTimes\n");
    for(int i = 0; i < size; i++){
        printf("%d\t%d\n", i+2, array[i]);
    }
}