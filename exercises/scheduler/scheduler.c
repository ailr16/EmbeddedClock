#include <stdio.h>
#include <string.h>
#include <time.h>
#include "scheduler.h"

void Scheduler_Init( Scheduler_t *hscheduler ){
    printf("Tick: %ld\n", hscheduler->tick);
    printf("Timeout: %ld\n", hscheduler->timeout);
    hscheduler->tasksCount = 0;
}

unsigned char Scheduler_RegisterTask( Scheduler_t *hscheduler, void(*InitPtr)(void), void(*TaskPtr)(void), unsigned long Period){
    Task_t aux;

    aux.initFunc = InitPtr;
    aux.taskFunc = TaskPtr;
    aux.period = Period;
    aux.elapsed = 0;
    aux.active = 1;

    hscheduler->taskPtr[hscheduler->tasksCount] = aux;
    hscheduler->tasksCount++;

    return hscheduler->tasksCount;
}

unsigned char Scheduler_StopTask( Scheduler_t *hscheduler, unsigned char task ){
    unsigned char auxReturn = 1;

    if( task <= 0 || task > hscheduler->tasks){
        auxReturn = 0;
    }
    else{
        hscheduler->taskPtr[task - 1].active = 0;
    }

    return auxReturn;
}

unsigned char Scheduler_StartTask( Scheduler_t *hscheduler, unsigned char task ){
    unsigned char auxReturn = 1;

    if( task <= 0 || task > hscheduler->tasks){
        auxReturn = 0;
    }
    else{
        hscheduler->taskPtr[task - 1].active = 1;
    }

    return auxReturn;
}

unsigned char Scheduler_PeriodTask( Scheduler_t *hscheduler, unsigned char task, unsigned long period ){
    unsigned char auxReturn = 1;

    if( task <= 0 ){
        auxReturn = 0;
    }

    return auxReturn;
}

void Scheduler_Start( Scheduler_t *hscheduler ){
    int i, j;
    unsigned long tickstartScheduler;
    unsigned long tick;

    //Init tasks
    for(i = 1; i <= hscheduler->tasks; i++){
        if(hscheduler->taskPtr[i - 1].initFunc != NULL){
            hscheduler->taskPtr[i - 1].initFunc();
        }
    }

    j = 0;
    i = 1;
    
    tickstartScheduler = milliseconds();
    
    while( 1 )
    {
        if( (milliseconds() - tickstartScheduler) >= hscheduler->tick )
        {
            j++;
            tickstartScheduler = milliseconds();
            printf("tick#%d\n", j);

            for(i=1; i<= hscheduler->tasks; i++)
            {
                if( hscheduler->taskPtr[i - 1].elapsed >= hscheduler->taskPtr[i - 1].period )
                {
                    if(hscheduler->taskPtr[i - 1].active)
                    {
                        hscheduler->taskPtr[i - 1].taskFunc();
                    }
                    hscheduler->taskPtr[i - 1].elapsed = 0;
                }
                hscheduler->taskPtr[i - 1].elapsed += hscheduler->tick;
            }

            /*if( hscheduler->taskPtr[i - 1].elapsed >= hscheduler->taskPtr[i - 1].period)
            {
                hscheduler->taskPtr[i - 1].taskFunc();
                printf("task%d elapsed time: %ld\n", i, hscheduler->taskPtr[i - 1].elapsed);
                hscheduler->taskPtr[i - 1].elapsed = 0;
                i++;
                if( i > hscheduler->tasks)
                {
                    i = 1;
                }
            }
            hscheduler->taskPtr[i - 1].elapsed += hscheduler->tick;
            */
        }
        

    }

}


long milliseconds( void ){
    return clock() / ( CLOCKS_PER_SEC / 1000 );
}