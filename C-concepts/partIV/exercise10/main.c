#include <stdio.h>
#include <stdlib.h>

int game(void);

int main(void){
    char selection = 'y';
    while(selection == 'y'){
        if(game()){
            printf("Would you like to play again?");
            scanf(" %c", &selection);
        }
    }

}

int game(void){
    int number, randomNumber;
    randomNumber = rand() % 100;
    printf("I have a number between 1 and 1000.\n");
    printf("Can you guess my number?\n");
    printf("Please type your first guess.\n");
    do{
        scanf("%d", &number);
        if(number == randomNumber){
            printf("Excellent. You guessed the number\n");
            break;
        }
        else{
            if(number < randomNumber){
                printf("Too low. Try again\n");
            }
            if(number > randomNumber){
                printf("Too high. Try again\n");
            }
        }
    }while(1);
    return 1;
}