#include <stdio.h>
#include <stdlib.h>

int generateQuestion(void);

int main(void){
    char continueQuestion = 'c';

    while(continueQuestion == 'c'){
        generateQuestion();
        
        printf("Continue (c) or end questions (e)? ");
        scanf(" %c", &continueQuestion);
    }
}

int generateQuestion(void){
    int numA, numB;
    int answer;

    numA = rand() % 10;
    numB = rand() % 10;

    printf("How much is %d times %d?\n", numA, numB);
    
    while(1){
        scanf("%d", &answer);

        if(answer == numA * numB){
            printf("Very good!\n");
            return 1;
        }
        else{
            printf("No. Please try again :-)\n");
        }
    }
}