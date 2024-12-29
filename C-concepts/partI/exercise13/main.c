#include <stdio.h>

int number;

int main(void){
    printf("Enter a 5-digit number: ");
    scanf("%d", &number);

    if (number < 10000 || number > 99999)   printf("%d is not a 5-digit number!\n", number);
    else{
        printf("%d ", number / 10000);
        printf("%d ", (number % 10000) / 1000);
        printf("%d ", ((number % 10000) % 1000) / 100);
        printf("%d ", (((number % 10000) % 1000) % 100) / 10);
        printf("%d\n", (((number % 10000) % 1000) % 100) % 10);
    }
}