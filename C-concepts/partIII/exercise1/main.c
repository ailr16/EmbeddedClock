#include <stdio.h>

int main(void){
    int totalValues, actualValue, sum = 0;
    int i = 0;
    scanf("%d", &totalValues);
    while(i < totalValues){
        scanf("%d", &actualValue);
        sum += actualValue;
        i++;
    }
    printf("Sum=%d\n", sum);
}