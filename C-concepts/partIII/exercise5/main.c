#include <stdio.h>

int main(void){
    int num1, num2, num3, num4, num5, i;
    printf("Enter 5 numbers (between 1 and 30): ");
    scanf("%d %d %d %d %d", &num1, &num2, &num3, &num4, &num5);

    printf("C1|");
    for(i = 0; i < num1; i++)    printf("*");

    printf("\nC2|");
    for(i = 0; i < num2; i++)    printf("*");

    printf("\nC3|");
    for(i = 0; i < num3; i++)    printf("*");

    printf("\nC4|");
    for(i = 0; i < num4; i++)    printf("*");

    printf("\nC5|");
    for(i = 0; i < num5; i++)    printf("*");
    printf("\n");
}