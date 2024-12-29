#include <stdio.h>

int qualityPoints(int average);

int main(void){
    int average;
    scanf("%d", &average);
    printf("%d\n", qualityPoints(average));
}

int qualityPoints(int average){
    switch(average / 10){
        case 10:
            return 4;
        break;
        case 9:
            return 4;
        break;
        case 8:
            return 3;
        break;
        case 7:
            return 2;
        break;
        case 6:
            return 1;
        break;
        default:
            return 0;
        break;
    }
}