#include <stdio.h>

int number1, number2;

int main(void){
    printf("Enter two numbers: ");
    scanf("%d %d", &number1, &number2);

    if (number2 % number1 == 0) printf("%d is multiple of %d\n", number2, number1);
    else printf("Not multiple\n");
}