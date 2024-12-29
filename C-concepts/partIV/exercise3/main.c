#include <stdio.h>

unsigned char even(int number);

int main(void){
    printf("Is %d even? %d \n", 3, even(3));
    printf("Is %d even? %d \n", 4, even(4));
    printf("Is %d even? %d \n", 13, even(13));
    printf("Is %d even? %d \n", 16, even(16));
}

unsigned char even(int number){
    return number % 2 == 0;
}