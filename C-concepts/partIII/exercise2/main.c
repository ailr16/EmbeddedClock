#include <stdio.h>

int main(void){
    int actualNumber, sum = 0;
    while(1){
        scanf("%d", &actualNumber);
        if(actualNumber == 9999)    break;
        sum += actualNumber;
    }
    printf("Sum=%d\n", sum);
}