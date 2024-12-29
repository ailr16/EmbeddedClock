#include <stdio.h>

int main(void){
    int number, decimalNumber;
    int digit1, digit2, digit3, digit4, digit5;

    printf("Enter a 5-digit binary number: ");
    scanf("%d", &number);

    digit1 = number / 10000;
    if(digit1 != 0 && digit1 != 1){
        printf("Not a binary number!\n");
        return 0;
    }

    digit2 = (number % 10000) / 1000;
    if(digit2 != 0 && digit2 != 1){
        printf("Not a binary number!\n");
        return 0;
    }

    digit3 = ((number % 10000) % 1000) / 100;
    if(digit3 != 0 && digit3 != 1){
        printf("Not a binary number!\n");
        return 0;
    }

    digit4 = (((number % 10000) % 1000) % 100) / 10;
    if(digit4 != 0 && digit4 != 1){
        printf("Not a binary number!\n");
        return 0;
    }

    digit5= ((((number % 10000) % 1000) % 100) % 10);
    if(digit5 != 0 && digit5 != 1){
        printf("Not a binary number!\n");
        return 0;
    }

    decimalNumber = digit1*16 + digit2*8 + digit3*4 + digit4*2 + digit5;

    printf("Binary Number: %d %d %d %d %d\n", digit1, digit2, digit3, digit4, digit5);
    printf("Decimal: %d\n", decimalNumber);

}