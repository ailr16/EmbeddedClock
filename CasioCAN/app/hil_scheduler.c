/**
 * @file    hil_scheduler.c
 * @brief   Source file with definitions of functions for scheduler handling
 * 
 * Allows to schedule a specified number of tasks and timers, executing each one with a specified periodicity
 * 
 * Find here the functions that allow to handle a task scheduler:
 * Initialize scheduler
 * Register task into scheduler
 * Stop execution of a registered task
 * Start execution of a registered task
 * Change periodicity execution of a registered task
 * Start scheduler execution
 * 
 * Register timer into scheduler
 * Get remaining time before timer reaches timeout
 * Reload timeout of a registered timer
 * Start/restart regitered timer counter
 * Stop regitered timer counter
 * 
 */

#include "hil_scheduler.h"


/** 
 * @defgroup <SafetyForTimer> Define values used for timing safety
 @{ */

#define     SAFETY_TIMER_TOLERANCE      10               /*!< Set the max percent error allowed for assure safety at scheduler timing */

/**
 @} */


/**
 * @brief   Struct for timer 6 periph handling
 */
static TIM_HandleTypeDef   TIM6_Handler = {0};


/**
 * @brief   Initialize Scheduler Handler structure
 * 
 * Initialize the hscheduler structure with the values passed as parameters the number of tasks that will be executed,
 * the tick value used as base time, and the address of the array of task TCBs
 *
 * Initialize TIM6 peripheral to measure and assure expected execution timing for each task
 * 
 * 
 * @param   hscheduler Scheduler handler
 */
void HIL_SCHEDULER_Init( Scheduler_HandleTypeDef *hscheduler )
{
    assert_error( hscheduler->tasks   != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->tick    != 0UL,  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->taskPtr != NULL, LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    hscheduler->tasksCount = 0;
    hscheduler->timersCount = 0;

    __HAL_RCC_TIM6_CLK_ENABLE();

    //Add 1ms counter
    TIM6_Handler.Instance = TIM6;
    TIM6_Handler.Init.Prescaler = 0xFA00;                                       //With APB clk setted to 32MHz and the prescaler value 0xFA00, a increment in TIM counter equals to 1ms
    TIM6_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM6_Handler.Init.Period = 0xFFFF;                                          //Max measurement time allowed is 65535ms
    TIM6_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                   //No clock division
    TIM6_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_Base_Init( &TIM6_Handler );
}


/**
 * @brief   Add a task to scheduler
 * 
 * Set the task TCB (Task Control Block) with the required parameters:
 * initialization function address,
 * task function address,
 * and periodicity in milliseconds
 * 
 * @param   hscheduler Scheduler handler
 * @param   InitPtr Pointer to initialization function
 * @param   TaskPtr Pointer to task function
 * @param   Period  Periodicity execution of the task in milliseconds, MUST BE multiple of Scheduler tick value
 */
uint8_t HIL_SCHEDULER_RegisterTask( Scheduler_HandleTypeDef *hscheduler, void (*InitPtr)(void), void (*TaskPtr)(void), uint32_t Period )
{
    uint8_t      returnStatus = 0U;
    Task_TypeDef aux = {0};

    assert_error( hscheduler->tasks   != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->tick    != 0UL,  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->taskPtr != NULL, LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( (Period % hscheduler->tick) == 0UL)
    {
        aux.initFunc = InitPtr;
        aux.taskFunc = TaskPtr;
        aux.elapsed  = 0UL;
        aux.active   = 1U;
        aux.period   = Period;
        hscheduler->taskPtr[hscheduler->tasksCount] = aux;
        hscheduler->tasksCount++;

        returnStatus = hscheduler->tasksCount;
    }
    else{
        returnStatus = 0U;
    }

    return returnStatus;
}


/**
 * @brief   Prevent function from dispatching
 * 
 * After registering a task into the scheduler, it will execute when the scheduler starts,
 * so this function indicates to scheduler which task (with its ID) should not be executed
 * 
 * @param   hscheduler Scheduler handler
 * @param   task ID of the task that will be stopped
 */
uint8_t HIL_SCHEDULER_StopTask( Scheduler_HandleTypeDef *hscheduler, uint32_t task )
{
    uint8_t returnStatus = 0U;

    assert_error( hscheduler->tasks   != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->tick    != 0UL,  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->taskPtr != NULL, LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( task > hscheduler->tasks){
        returnStatus = 0U;
    }
    else{
        hscheduler->taskPtr[task - 1UL].active = 0U;
        returnStatus = 1U;
    }

    return returnStatus;
}


/**
 * @brief   Activate task after stopped
 * 
 * When the function HIL_SCHEDULER_StopTask is used, it prevents task from dispatching when the scheduler is running,
 * so this function reverts that case, use with the task ID that must be executed again
 * 
 * @param   hscheduler Scheduler handler
 * @param   task ID of the task that will be resumed
 */
uint8_t HIL_SCHEDULER_StartTask( Scheduler_HandleTypeDef *hscheduler, uint32_t task )
{
    uint8_t returnStatus = 0U;

    assert_error( hscheduler->tasks   != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->tick    != 0UL,  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->taskPtr != NULL, LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( task > hscheduler->tasks){
        returnStatus = 0U;
    }
    else{
        hscheduler->taskPtr[task - 1UL].active = 1U;
        returnStatus = 1U;
    }

    return returnStatus;
}


/**
 * @brief   Set new task periodicity 
 * 
 * Set a new periodicity for a task (indicated with its ID),
 * it must be multiple of the tick value: otherwise won’t be affected by the new period
 * 
 * @param   hscheduler Scheduler handler
 * @param   task ID of the task that will be modified
 * @param   period  Periodicity execution of the task in milliseconds, MUST BE multiple of Scheduler tick value
 */
uint8_t HIL_SCHEDULER_PeriodTask( Scheduler_HandleTypeDef *hscheduler, uint32_t task, uint32_t period )
{
    uint8_t returnStatus = 0U;

    assert_error( hscheduler->tasks   != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->tick    != 0UL,  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->taskPtr != NULL, LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( task > hscheduler->tasks ){
        returnStatus = 0U;
    }
    else
    {
        if( (period % hscheduler->tick) == 0UL)
        {
            hscheduler->taskPtr[task - 1UL].period = 0U;
            returnStatus = 1U;
        }
    }

    return returnStatus;
}


/**
 * @brief   Start Scheduler execution
 * 
 * With scheduler start, the task init functions will be executed one single time,
 * and run each registered task according to their periodicity in an infinite loop
 * 
 * Validate execution time for each task using the TIM6 peripheral
 * 
 * Run count of all software timers with every tick and if is the case, execute specified
 * callback function
 * 
 * @param   hscheduler Scheduler handler
 */
void HIL_SCHEDULER_Start( Scheduler_HandleTypeDef *hscheduler )
{
    uint32_t i = 0;
    uint32_t tickstartScheduler     = 0;
    uint32_t safetyTimerAux         = 0;
    uint32_t safetyTimerActual      = 0;
    uint32_t safetyTimerElapsed     = 0;
    uint32_t safetyTimerTolerance   = 0;
    uint32_t safetyTimerUpperLimit  = 0;
    uint32_t safetyTimerLowerLimit  = 0;

    assert_error( hscheduler->tasks   != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->tick    != 0UL,  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->taskPtr != NULL, LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    //Init tasks
    for( i = 1UL; i <= hscheduler->tasks; i++){
        hscheduler->taskPtr[i - 1UL].elapsedTimer = 0UL;                            //Set 0 value for measuring time with TIM6

        if( hscheduler->taskPtr[i - 1UL].initFunc != NULL ){
            hscheduler->taskPtr[i - 1UL].initFunc();
        }
    }

    tickstartScheduler = HAL_GetTick();
    
    HAL_TIM_Base_Start_IT( &TIM6_Handler );                     //Start TIM6 counting in base mode

    while( 1 )
    {
        if( (HAL_GetTick() - tickstartScheduler) >= hscheduler->tick )
        {
            tickstartScheduler = HAL_GetTick();
            
            for( i = 1UL; i<= hscheduler->tasks; i++)
            {
                /*  
                    The task struct has the member elapsedTimer, which stores the last measurement time performed by the TIM6 periph

                    Every time a task is executed the counter value of TIM6 is retrieved into safetyTimerActual variable

                    Using the values of task->elapsedTimer and safetyTimerActual it's possible to assure that the task was executed
                    at the expected time with a little margin determined by SAFETY_TIMER_TOLERANCE which affects safetyTimerUpperLimit
                    and safetyTimerLowerLimit
                    
                    Example:
                    The actual task has a period of 100ms. SAFETY_TIMER_MAX_PERCENT_ERROR value is 10
                    For the first time the task is executed, task->elapsedTimer has 0 value
                    When the task has been dispatched, TIM6 has the value 109 (9 seconds more than expected that appears only for the first execution),
                    that means that the measured elapsed time is 109ms (safetyTimerActual - task->elapsedTimer = 109ms - 0 = 109ms)
                    The tolerance is calculated as a percentage of task period determined by SAFETY_TIMER_TOLERANCE, if this value is 10,
                    then 10 percent of 100ms is 10ms
                    The safetyTimerUpperLimit is obtained adding the tolerance to task period: 100ms + 10ms = 110ms
                    The safetyTimerLowerLimit is obtained subtracting the tolerance from task period: 100ms - 10ms = 90ms

                    The actual value of TIM6 counter is stored in task->elapsedTimer for next measurement

                    Finally, if the measured elapsed time is between the lower and upper limit means that the task was executed on time, otherwise
                    that's an error and the system goes into safety state

                    NOTE: There's a special case when the TIM6 counter retireved value is near to its max value (0xFFFF = 65535) because the next value will
                    be so samll, example, if the first retrieved is 65500 and the task period is 200ms, the next value will be 165, and using the expression
                    elapsed time = actual time - last time = 165 - 65500 gives a wrong value, so the line 279 IF ELSE sentence provides a proposed solution
                    to prevent that error
                */

                if( hscheduler->taskPtr[i - 1UL].elapsed >= hscheduler->taskPtr[i - 1UL].period )
                {
                    safetyTimerActual         = TIM6->CNT;                                                         

                    if( hscheduler->taskPtr[i - 1UL].elapsedTimer >= (0xFFFFUL - hscheduler->taskPtr[i - 1UL].period) )         //TIM6 counter value is near to max allowed value 0xFFFF
                    {
                        safetyTimerAux     = 0xFFFFUL - hscheduler->taskPtr[i - 1UL].elapsedTimer;
                        safetyTimerElapsed = safetyTimerActual + safetyTimerAux;                                                //Calculate elapsed time
                    }
                    else{
                        safetyTimerElapsed = safetyTimerActual - hscheduler->taskPtr[i - 1UL].elapsedTimer;
                    }

                    safetyTimerTolerance   = (hscheduler->taskPtr[i - 1UL].period * (uint32_t)SAFETY_TIMER_TOLERANCE) / 100UL;
                    safetyTimerUpperLimit  = hscheduler->taskPtr[i - 1UL].period + safetyTimerTolerance;
                    safetyTimerLowerLimit  = hscheduler->taskPtr[i - 1UL].period - safetyTimerTolerance;

                    hscheduler->taskPtr[i - 1UL].elapsedTimer = safetyTimerActual;                                              //Store the counter value for next execution

                    //Adding 9 ms to upper limit for avoid a false possitive error on first task execution
                    assert_error( (safetyTimerElapsed >= (safetyTimerLowerLimit) && (safetyTimerElapsed <= (safetyTimerUpperLimit + 9UL)) ),  SCHEDULER_TIM_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
                    
                    if( hscheduler->taskPtr[i - 1UL].active == 1U)
                    {
                        hscheduler->taskPtr[i - 1UL].taskFunc();
                    }

                    hscheduler->taskPtr[i - 1UL].elapsed = 0UL;
                }
                hscheduler->taskPtr[i - 1UL].elapsed += hscheduler->tick;
                
            }
            
            for( i = 1UL; i<= hscheduler->timers; i++)              //Timers section
            {
                if( hscheduler->timerPtr[i - 1UL].Count == 0UL )
                {
                    if( hscheduler->timerPtr[i - 1UL].callbackPtr != NULL )
                    {
                        hscheduler->timerPtr[i - 1UL].callbackPtr();
                    }

                }

                if( hscheduler->timerPtr[i - 1UL].StartFlag == 1UL )
                {
                    hscheduler->timerPtr[i - 1UL].Count -= hscheduler->tick;
                }
            }
        }
    }
}


/**
 * @brief   Add a timer to scheduler
 * 
 * Register a new software timer wich will count from Timeout to 0 and execute a callback function
 * passed as CallbackPtr. Returns the timer ID which is a value from 1 to the number of timers
 * registered in the scheduler
 * 
 * 
 * @param   hscheduler  Scheduler handler
 * @param   Timeout     Duration of timer in ms. Initial value of counter
 * @param   CallbackPtr Callback function executed when the counter reaches zero
 * @retval  Timer ID expressed as 8-bit unsigned integer that goes from 1 to number of timers registered in the scheduler
 */
uint8_t HIL_SCHEDULER_RegisterTimer( Scheduler_HandleTypeDef *hscheduler, uint32_t Timeout, void (*CallbackPtr)(void) )
{
    uint8_t        returnStatus = 0U;
    Timer_TypeDef  aux = {0};

    assert_error( hscheduler->tasks    != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->tick     != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->taskPtr  != NULL, SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->timers   != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->timerPtr != NULL, SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( (Timeout % hscheduler->tick) == 0UL)
    {
        aux.callbackPtr = CallbackPtr;
        aux.Timeout     = Timeout;
        aux.Count       = Timeout;
        aux.StartFlag   = 0UL;

        hscheduler->timerPtr[hscheduler->timersCount] = aux;
        hscheduler->timersCount++;

        returnStatus = hscheduler->timersCount;
    }
    else{
        returnStatus = 0U;
    }

    return returnStatus;
}


/**
 * @brief   Get timer current count
 * 
 * Return the current timer pending time in milliseconds. If the specified Timer has not been registered returns zero
 * 
 * @param   hscheduler Scheduler handler
 * @param   Timer      Timer ID
 * @retval  Timer count expressed as 32-bit unsigned integer
 */
uint32_t HIL_SCHEDULER_GetTimer( Scheduler_HandleTypeDef *hscheduler, uint32_t Timer )
{
    uint8_t returnStatus = 0U;

    assert_error( hscheduler->tasks    != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->tick     != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->taskPtr  != NULL, SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->timers   != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->timerPtr != NULL, SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( (Timer > hscheduler->timers) || (Timer == 0UL) ){
        returnStatus = 0U;
    }
    else
    {
        returnStatus = hscheduler->timerPtr[Timer - 1UL].Count;
    }

    return returnStatus;
}


/**
 * @brief   Set new Timeout value in ms
 * 
 * The timer will be reloaded with a new value in milliseconds it will also start the timer,
 * the function does not require the timer to stop first, but if the timer to reload has not
 * been registered no action will be taken and it will return 0, otherwise return 1
 * as an indication of success.
 * 
 * @param   hscheduler Scheduler handler
 * @param   Timer      Timer ID
 * @param   Timeout    New timeout value in ms
 * @retval  1 for successful operation, 0 otherwise
 */
uint8_t HIL_SCHEDULER_ReloadTimer( Scheduler_HandleTypeDef *hscheduler, uint32_t Timer, uint32_t Timeout )
{
    uint8_t        returnStatus = 0U;

    assert_error( hscheduler->tasks    != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->tick     != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->taskPtr  != NULL, SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->timers   != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->timerPtr != NULL, SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( (Timer > hscheduler->timers) || (Timer == 0UL) ){
        returnStatus = 0U;
    }
    else
    {
        if( (Timeout % hscheduler->tick) == 0UL)
        {
            hscheduler->timerPtr[Timer - 1UL].Timeout   = Timeout;
            hscheduler->timerPtr[Timer - 1UL].Count     = Timeout;
            hscheduler->timerPtr[Timer - 1UL].StartFlag = 1UL;

            returnStatus = 1U;
        }
        else
        {
            returnStatus = 0U;
        }
    }
    
    return returnStatus;
}


/**
 * @brief   Start the timer count
 * 
 * When the timer is registered don't starts by default, it is necessary to call this function
 * In the same way, when the timer reaches the timeout, this funcion can be used to restart it
 * If the specified timer has not been registered, the functions returns 0
 * 
 * @param   hscheduler Scheduler handler
 * @param   Timer      Timer ID
 * @retval  1 for successful operation, 0 otherwise
 */
uint8_t HIL_SCHEDULER_StartTimer( Scheduler_HandleTypeDef *hscheduler, uint32_t Timer )
{
    uint8_t        returnStatus = 0U;

    assert_error( hscheduler->tasks    != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->tick     != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->taskPtr  != NULL, SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->timers   != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->timerPtr != NULL, SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( (Timer > hscheduler->timers) || (Timer == 0UL) ){
        returnStatus = 0U;
    }
    else
    {
        hscheduler->timerPtr[Timer - 1UL].StartFlag = 1UL;

        if( hscheduler->timerPtr[Timer - 1UL].Count == 0UL )                        //Restart counter if timeout has been reached
        {
            hscheduler->timerPtr[Timer - 1UL].Count = hscheduler->timerPtr[Timer - 1UL].Timeout;
        }

        returnStatus = 1U;
    }
    
    return returnStatus;
}


/**
 * @brief   Stop the timer count
 * 
 * The function will indicate the timer should not be decremented during each tick occurrence
 * If the specified timer has not been registered, the functions returns 0
 * 
 * @param   hscheduler Scheduler handler
 * @param   Timer      Timer ID
 * @retval  1 for successful operation, 0 otherwise
 */
uint8_t HIL_SCHEDULER_StopTimer( Scheduler_HandleTypeDef *hscheduler, uint32_t Timer )
{
    uint8_t        returnStatus = 0U;

    assert_error( hscheduler->tasks    != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->tick     != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->taskPtr  != NULL, SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->timers   != 0UL,  SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hscheduler->timerPtr != NULL, SCHEDULER_PAR_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( (Timer > hscheduler->timers) || (Timer == 0UL) ){
        returnStatus = 0U;
    }
    else
    {
        hscheduler->timerPtr[Timer - 1UL].StartFlag = 0UL;

        returnStatus = 1U;
    }
    
    return returnStatus;
}