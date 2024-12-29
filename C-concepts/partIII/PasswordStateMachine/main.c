#include <stdio.h>

//gcc main.c -o main.o && ./main.o

#define IDLE_1 19
#define IDLE_2 29

#define FIRST_DIGIT_1 11
#define SECOND_DIGIT_1 12
#define THIRD_DIGIT_1 13
#define FOURTH_DIGIT_1 14
#define FIFTH_DIGIT_1 15
#define OPEN_DOOR_1 16

#define FIRST_DIGIT_2 21
#define SECOND_DIGIT_2 22
#define THIRD_DIGIT_2 23
#define FOURTH_DIGIT_2 24
#define FIFTH_DIGIT_2 25
#define OPEN_DOOR_2 26

#define ERROR_1 17
#define ERROR_2 27

#define ON 1
#define OFF 0


int main(void){
    int digit1, digit2, digit3, digit4, digit5;
    int state = IDLE_1;
    int run = ON;

    while(run == ON){
        switch(state){
            case IDLE_1:
                printf("Enter the password for door 1: ");
                state = FIRST_DIGIT_1;
            break;

            case FIRST_DIGIT_1:
                scanf("%d", &digit1);
                if(digit1 == 3) state = SECOND_DIGIT_1;
                else state = ERROR_1;
            break;

            case SECOND_DIGIT_1:
                scanf("%d", &digit2);
                if(digit2 == 6) state = THIRD_DIGIT_1;
                else state = ERROR_1;
            break;

            case THIRD_DIGIT_1:
                scanf("%d", &digit3);
                if(digit3 == 5) state = FOURTH_DIGIT_1;
                else state = ERROR_1;
            break;

            case FOURTH_DIGIT_1:
                scanf("%d", &digit4);
                if(digit4 == 7) state = FIFTH_DIGIT_1;
                else state = ERROR_1;
            break;

            case FIFTH_DIGIT_1:
                scanf("%d", &digit5);
                if(digit5 == 9) state = OPEN_DOOR_1;
                else state = ERROR_1;
            break;

            case OPEN_DOOR_1:
                printf("The door 1 has been opened!\n");
                printf("----------------\n");
                state = IDLE_2;
            break;

            case ERROR_1:
                printf("Wrong digit!\n");
                state = IDLE_1;
            break;

            case IDLE_2:
                printf("Enter the password for door 2: ");
                state = FIRST_DIGIT_2;
            break;

            case FIRST_DIGIT_2:
                scanf("%d", &digit1);
                if(digit1 == 3) state = SECOND_DIGIT_2;
                else state = ERROR_2;
            break;

            case SECOND_DIGIT_2:
                scanf("%d", &digit2);
                if(digit2 == 6) state = THIRD_DIGIT_2;
                else state = ERROR_2;
            break;

            case THIRD_DIGIT_2:
                scanf("%d", &digit3);
                if(digit3 == 5) state = FOURTH_DIGIT_2;
                else state = ERROR_2;
            break;

            case FOURTH_DIGIT_2:
                scanf("%d", &digit4);
                if(digit4 == 8) state = FIFTH_DIGIT_2;
                else state = ERROR_2;
            break;

            case FIFTH_DIGIT_2:
                scanf("%d", &digit5);
                if(digit5 == 1) state = OPEN_DOOR_2;
                else state = ERROR_2;
            break;

            case OPEN_DOOR_2:
                printf("The door 2 has been opened!\n");
                run = OFF;
            break;

            case ERROR_2:
                printf("Wrong digit!\n");
                state = IDLE_2;
            break;

            default:
            break;
        }
    }
}