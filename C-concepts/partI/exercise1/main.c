#include <stdio.h>

int num1, num2, sum, product, difference, quotient, remainderVar;

int main(void){
    printf("Enter a number: ");
    scanf("%d", &num1);

    printf("Enter another number: ");
    scanf("%d", &num2);

    printf("Number 1 is %d , and number 2 is %d \n", num1, num2);

    sum = num1 + num2;
    product = num1 * num2;
    difference = num1 - num2;
    quotient = num1 / num2;
    remainderVar = num1 % num2;
    
    printf("sum = %d, difference = %d, product = %d, quotient = %d, remainder = %d\n", sum, difference, product, quotient, remainderVar);
}