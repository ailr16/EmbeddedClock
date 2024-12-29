#include <stdio.h>
#include "scheduler.h"

#define TASKS_N     3
#define TICK_VAL    100

static Task_t       tasks[ TASKS_N ];
static Scheduler_t  Sche;

void Init_500ms(void);
void Init_1000ms(void);
void Init_1500ms(void);
void Task_500ms(void);
void Task_1000ms(void);
void Task_1500ms(void);

void Print500ms(void);

int main(void){
    unsigned char TaskID1;
    unsigned char TaskID2;
    unsigned char TaskID3;
    
    Sche.tick = TICK_VAL;
    Sche.tasks = TASKS_N;
    Sche.timeout = 6000;
    Sche.taskPtr = tasks;
    Scheduler_Init( &Sche );

    TaskID1 = Scheduler_RegisterTask(&Sche, Init_500ms, Task_500ms, 500);
    TaskID2 = Scheduler_RegisterTask(&Sche, Init_1000ms, Task_1000ms, 1000);
    TaskID3 = Scheduler_RegisterTask(&Sche, Init_1500ms, Task_1500ms, 1500);

    Scheduler_StopTask( &Sche, TaskID2 );

    Scheduler_Start(&Sche);

}

void Init_500ms(void){
    printf("Init task 500 millisecond\n");
}

void Init_1000ms(void){
    printf("Init task 1000 millisecond\n");
}

void Init_1500ms(void){
    printf("Init task 1500 millisecond\n");
}

void Task_500ms(void){
    static int loop = 0;
    printf("This is a counter from task 500ms: %d\n", loop++);
}

void Task_1000ms(void){
    static int loop = 0;    
    printf("This is a counter from task 1000ms: %d\n", loop++);
}

void Task_1500ms(void){
    static int loop = 0;
    printf("This is a counter from task 1500ms: %d\n", loop++);
}



void Print500ms(void){
    long x = 0;
    long tickstart = milliseconds();

    while( x < 10){
        if( (milliseconds() - tickstart) >= 500 ){
            tickstart = milliseconds();
            printf("clocks per second: %ld\n\r", x++);
        };
    }
}