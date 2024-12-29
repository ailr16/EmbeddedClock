#include <stdio.h>

int number;

int main(void){
    printf("Enter a number: ");
    scanf("%d", &number);
    
    if (number % 2 == 0)    printf("%d is an even number\n", number);
    if (number % 2 != 0)    printf("%d is an odd number\n", number);
}