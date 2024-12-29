#include <stdio.h>
#include <stdlib.h>

//gcc main.c -o main.o && ./main.o

/*
    States and events
*/
typedef enum {
    DRIVE,
    NEUTRAL,
    REVERSE,
    TOTAL_STATES                          //Keep this element at the end for count the total states
} MachineState;


typedef struct {
    MachineState stateName;             //State Identifier using the MachineState enum
    void (*stateFunc)(void);            //Pointer to function which perform state statements
} StateNode;


/*
    Function prototypes for each action procedure
*/
void drive();
void neutral();
void reverse();


/*
    Define lookup table
*/
StateNode stateMachine[TOTAL_STATES] = {
    { DRIVE,   drive   },
    { NEUTRAL, neutral },
    { REVERSE, reverse }
};

MachineState currentState = NEUTRAL;

int main(void){
    printf("Starting!!!\n");

    for(;;){
        stateMachine[currentState].stateFunc();
    }

    return 0;
}

void drive (void)
{
    char a;
    printf("State DRIVE - Enter X to pass\n");
    scanf(" %c", &a);
    if( a == 'X' ){
        currentState = NEUTRAL;
    }
    else
    {
        printf("Error. Character must be X\n");
    }
}

void neutral (void)
{
    char a;
    printf("State NEUTRAL - Enter Y to pass\n");
    scanf(" %c", &a);
    if( a == 'Y' ){
        currentState = REVERSE;
    }
    else
    {
        printf("Error. Character must be Y\n");
    }
}


void reverse (void)
{
    char a;
    printf("State REVERSE - Enter Z to pass\n");
    scanf(" %c", &a);
    if( a == 'Z' ){
        currentState = DRIVE;
    }
    else
    {
        printf("Error. Character must be Z\n");
    }
}