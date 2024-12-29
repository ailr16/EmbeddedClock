#include <stdio.h>

#define TOTAL_ELEMENTS 20

int validateNumber(int number);

int main(void){
    int numbersArray[TOTAL_ELEMENTS];
    int actualNumber;
    int savedNumbers = 0;

    //Save first number
    scanf("%d", &actualNumber);
    actualNumber = validateNumber(actualNumber);    //Verify it's between 10 and 100
    numbersArray[savedNumbers] = actualNumber;
    printf("%d ", actualNumber);
    savedNumbers++;

    for(int i = 1; i < TOTAL_ELEMENTS; i++){
        scanf("%d", &actualNumber);
        actualNumber = validateNumber(actualNumber);    //Verify it's between 10 and 100
        for(int j = 0; j < savedNumbers; j++){
            if(actualNumber == numbersArray[j]){
                break;
            }
            if(j == (savedNumbers - 1)){
                numbersArray[savedNumbers++] = actualNumber;
                printf("%d ", actualNumber);
            }
        }
    }
    
    printf("\n");
}

int validateNumber(int number){
    if(number >= 10 && number <= 100){
        return number;
    }
    else{
        return -1;
    }
}