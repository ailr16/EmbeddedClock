#include <stdio.h>

void AsterisksSquare(int side, char fillCharacter);

int main(void){
    AsterisksSquare(4, '#');
    printf("\n");
    AsterisksSquare(8, '"');
}

void AsterisksSquare(int side, char fillCharacter){
    for(int i = 0; i < side; i++){
        for(int j = 0; j < side; j++)   printf("%c",fillCharacter);
        printf("\n");
    }
}