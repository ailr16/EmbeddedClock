#include <stdio.h>

#define ARRAY_SIZE 1000

unsigned char multiple(int numberA, int numberB);
unsigned char IsPrime(int numberA);
void InitializeArrayOnes(int *array, int size);
void PrintArray(int *array, int size);
void PrintPrimes(int *array, int size);

int main(void){
    int numberArray[ARRAY_SIZE];

    InitializeArrayOnes(numberArray, ARRAY_SIZE);

    for(int i = 2; i < ARRAY_SIZE; i++){
        for(int j = i + 1; j < ARRAY_SIZE; j++){
            if ((j % i) == 0){
                numberArray[j] = 0;
            }
        }
    }
   
    PrintPrimes(numberArray, ARRAY_SIZE);
}

unsigned char multiple(int numberA, int numberB){
    return numberB % numberA == 0;
}

unsigned char IsPrime(int number){
    for(int i = (number - 1); i > 1; i--){
        if(multiple(i, number)){
            return 0;
        }
    }
    return 1;
}

void InitializeArrayOnes(int *array, int size){
    for(int i = 0; i < size; i++){
        array[i] = 1;
    }
}

void PrintArray(int *array, int size){
    for(int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

void PrintPrimes(int *array, int size){
    printf("1 to %d primes\n", size);
    for(int i = 1; i < size; i++){
        if(array[i] == 1){
            printf("%d ", i);
        }
    }
    printf("\n");
}