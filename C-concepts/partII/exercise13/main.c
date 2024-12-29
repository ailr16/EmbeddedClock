#include <stdio.h>

int main(void){
    int side;

    printf("Enter side of square (from 1 to 20): ");
    scanf("%d", &side);

    if(side < 0 && side > 20){
        printf("The side must be between 1 and 20!");
        return 0;
    }

    for(int i = 0; i < side; i++){
        for(int j = 0; j < side; j++){
            printf("*");
        }
        printf("\n");
    }
}