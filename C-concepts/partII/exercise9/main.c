#include <stdio.h>

int main(void){
    int number, auxNumber;
    int digit1, digit2, digit3, digit4, digit5;

    printf("Enter a 5-digit number: ");
    scanf("%d", &number);

    if(number < 10000 || number > 99999){
        printf("%d is not 5-digit number!\n", number);
        return 0;
    }

    digit1 = number / 10000;
    digit2 = (number % 10000) / 1000;
    digit3 = ((number % 10000) % 1000) / 100;
    digit4 = (((number % 10000) % 1000) % 100) / 10;
    digit5= ((((number % 10000) % 1000) % 100) % 10);

    printf("%d %d %d %d %d\n", digit1, digit2, digit3, digit4, digit5);
    if(digit1 == digit5 && digit2 == digit4)    printf("%d is palindrome!\n", number);

}