#include <stdio.h>
#include <stdlib.h>

int game(void);
void validateGuesses(int totalGuesses);

int main(void){
    char selection = 'y';
    while(selection == 'y'){
        if(game()){
            printf("Would you like to play again? ");
            scanf(" %c", &selection);
        }
    }

}

int game(void){
    int number, randomNumber;
    int totalGuesses = 1;

    randomNumber = rand() % 1000;

    printf("I have a number between 1 and 1000.\n");
    printf("Can you guess my number?\n");
    printf("Please type your first guess.\n");

    do{
        scanf("%d", &number);
        if(number == randomNumber){
            printf("Excellent. You guessed the number\n");
            validateGuesses(totalGuesses);
            break;
        }
        else{
            if(number < randomNumber){
                printf("Too low. Try again\n");
                totalGuesses++;
            }
            if(number > randomNumber){
                printf("Too high. Try again\n");
                totalGuesses++;
            }
        }
    }while(1);
    return 1;
}

void validateGuesses(int totalGuesses){
    printf("Total guesses: %d\n", totalGuesses);
    if (totalGuesses < 10){
        printf("Either you know the secret or you got lucky!\n");
    }
    else if (totalGuesses == 10){
        printf("Ahah! You know the secret!\n");
    }
    else if (totalGuesses > 10){
        printf("You should be able to do better!\n");
    }
}