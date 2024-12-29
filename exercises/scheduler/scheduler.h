#ifndef SCHEDULER_H_
#define SCHEDULER_H_

typedef struct _task{
    unsigned long period;
    unsigned long elapsed;
    unsigned char active;
    void (*initFunc)(void);
    void (*taskFunc)(void);
} Task_t;

typedef struct _scheduler{
    unsigned long tasks;
    unsigned long tick;
    unsigned long timeout;
    unsigned long tasksCount;
    Task_t *taskPtr;
} Scheduler_t;

void Scheduler_Init( Scheduler_t *hscheduler );
unsigned char Scheduler_RegisterTask( Scheduler_t *hscheduler, void(*InitPtr)(void), void(*TaskPtr)(void), unsigned long Period);
unsigned char Scheduler_StopTask( Scheduler_t *hscheduler, unsigned char task );
unsigned char Scheduler_StartTask( Scheduler_t *hscheduler, unsigned char task );
unsigned char Scheduler_PeriodTask( Scheduler_t *hscheduler, unsigned char task, unsigned long period );
void Scheduler_Start( Scheduler_t *hscheduler );

long milliseconds( void );


#endif