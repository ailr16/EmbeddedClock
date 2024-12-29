#include <stdio.h>

int main(void){
    int i = 1;
    
    while(i < 101){
        printf("*");
        if(i % 10 == 0) printf("\n");
        i++;
    }
}