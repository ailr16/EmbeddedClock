/**
 * @file    hil_scheduler.h
 * @brief   Header file with declarations for scheduler handling
 *
 * Allows to schedule a specified number of tasks, executing each one with a specified periodicity
 * 
 * Find here Task_TypeDef struct definition which stores task data
 * Find here Scheduler_HandleTypeDef struct definition which allows scheduler handling
 * Find here Timer_TypeDef struct definition which stores timer data
 * Find also all (6) Scheduler handling functions declaration
 * Find all (5) Timer handling functiond declaration
 * 
 */

#ifndef HIL_SCHEDULER_H
#define HIL_SCHEDULER_H

    #include "app_bsp.h"


    typedef struct _task
    {
        uint32_t period;          /* How often the task shopud run in ms */
        uint32_t elapsed;         /* The current elapsed time */
        uint8_t  active;          /* Indicate if the task must be run */
        uint32_t elapsedTimer;    /* Elapsed time measured with timer peripheral*/
        void (*initFunc)(void);   /*pointer to init task function*/
        void (*taskFunc)(void);   /*pointer to task function*/
    }Task_TypeDef;

    typedef struct _Timer_TypeDef
    {
        uint32_t Timeout;           /*!< timer timeout to decrement and reload when the timer is re-started */
        uint32_t Count;             /*!< actual timer decrement count */
        uint32_t StartFlag;         /*!< flag to start timer count */
        void(*callbackPtr)(void);   /*!< pointer to callback function  */
    } Timer_TypeDef;

    typedef struct _scheduler
    {
        uint32_t      tasks;         /*number of task to handle*/
        uint32_t      tick;          /*the time base in ms*/
        uint32_t      tasksCount;    /*internal task counter*/
        Task_TypeDef *taskPtr;  /*Pointer to buffer for the TCB tasks*/
        uint32_t      timers;        /* number of software timers to use */
        uint32_t      timersCount;   /* internal timer counter */
        Timer_TypeDef *timerPtr;     /* Pointer to buffer timer array */
    }Scheduler_HandleTypeDef;


    void HIL_SCHEDULER_Init( Scheduler_HandleTypeDef *hscheduler );


    uint8_t HIL_SCHEDULER_RegisterTask( Scheduler_HandleTypeDef *hscheduler, void (*InitPtr)(void), void (*TaskPtr)(void), uint32_t Period );


    uint8_t HIL_SCHEDULER_StopTask( Scheduler_HandleTypeDef *hscheduler, uint32_t task );

    
    uint8_t HIL_SCHEDULER_StartTask( Scheduler_HandleTypeDef *hscheduler, uint32_t task );

    
    uint8_t HIL_SCHEDULER_PeriodTask( Scheduler_HandleTypeDef *hscheduler, uint32_t task, uint32_t period );


    void HIL_SCHEDULER_Start( Scheduler_HandleTypeDef *hscheduler );

    

    //Software Timer Interfaces

    uint8_t HIL_SCHEDULER_RegisterTimer( Scheduler_HandleTypeDef *hscheduler, uint32_t Timeout, void (*CallbackPtr)(void) );


    uint32_t HIL_SCHEDULER_GetTimer( Scheduler_HandleTypeDef *hscheduler, uint32_t Timer );

    
    uint8_t HIL_SCHEDULER_ReloadTimer( Scheduler_HandleTypeDef *hscheduler, uint32_t Timer, uint32_t Timeout );


    uint8_t HIL_SCHEDULER_StartTimer( Scheduler_HandleTypeDef *hscheduler, uint32_t Timer );


    uint8_t HIL_SCHEDULER_StopTimer( Scheduler_HandleTypeDef *hscheduler, uint32_t Timer );


#endif

