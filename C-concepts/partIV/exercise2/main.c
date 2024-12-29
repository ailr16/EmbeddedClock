#include <stdio.h>

unsigned char multiple(int numberA, int numberB);

int main(void){
    printf("%d multiple of %d? : %d\n", 6, 3, multiple(3,6));
    printf("%d multiple of %d? : %d\n", 3, 6, multiple(6,3));
    printf("%d multiple of %d? : %d\n", 8, 1, multiple(1,8));
}

unsigned char multiple(int numberA, int numberB){
    return numberB % numberA == 0;
}