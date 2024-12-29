#include <stdio.h>

int num1, num2;

int main(void){
    printf("Enter a number: ");
    scanf("%d", &num1);

    printf("Enter another number: ");
    scanf("%d", &num2);

    if (num1 > num2) printf("%d is larger\n", num1);
    if (num2 > num1) printf("%d is larger\n", num2);
    if (num1 == num2) printf("These numbers are equal\n");
}