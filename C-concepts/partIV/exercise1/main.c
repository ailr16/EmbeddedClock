#include <stdio.h>

int integerPower(int base, int exponent);

int main(void){
    printf("2^3=%d\n", integerPower(2,3));
    printf("2^8=%d\n", integerPower(2,8));
    printf("4^9=%d\n", integerPower(4,9));
}

int integerPower(int base, int exponent){
    int result = 1;
    for(int i = 0; i < exponent; i++)   result *= base;
    return result;
}