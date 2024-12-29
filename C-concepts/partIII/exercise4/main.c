#include <stdio.h>

int main(void){
    int i, j;
    for(i = 0; i < 10; i++){
        for(int j = 0; j <= i; j++) printf("*");
        printf("\n");
    }

    for(i = 10; i > 0; i--){
        for(int j = 0; j < i; j++) printf("*");
        printf("\n");
    }

    for(i = 0; i < 10; i++){
        for(j = i; j > 0; j--) printf(" ");
        for(j = i; j < 10; j++) printf("*");
        printf("\n");
    }

    for(i = 1; i <= 10; i++){
        for(j = i; j < 10; j++) printf(" ");
        for(j = i; j > 0; j--) printf("*");
        printf("\n");
    }

}