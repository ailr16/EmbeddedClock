#include <stdio.h>

int main(void){
    int totalValues, actualValue, smallestNumber;
    int i = 0;
    scanf("%d", &totalValues);
    scanf("%d", &actualValue);
    smallestNumber = actualValue;
    while(i < totalValues -1 ){
        scanf("%d", &actualValue);
        if(actualValue < smallestNumber)    smallestNumber = actualValue;
        i++;
    }
    printf("Smallest number=%d\n", smallestNumber);
}