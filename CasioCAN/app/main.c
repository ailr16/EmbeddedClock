/**
 * @file    main.c
 * @brief   Project's main file
 * 
 * The project implements a clock using 5 main features:
 * Serial     Reads and decode a message received trough FDCAN peripheral, and validate received data. Stores the data into a buffer
 * Clock      Reads the data from the serial buffer and writes it to the RTC peripheral. Read data of RTC and store it in other buffer
 * Display    Reads data from clock buffer and prints it in the LCD
 * Heartbeat  Blinks a LED to indicate the system is runnnig
 * Watchdog   Resets the system if a failure has ocurred
 *
 */

#include "app_bsp.h"
#include "app_serial.h"
#include "app_clock.h"
#include "app_display.h"

/** 
 * @defgroup <schedulerProperties> Define number of tasks to handle and value of tick in ms
 @{ */

#define TASKS_N     5               /*!< Total tasks to handle */
#define TICK_VAL    10              /*!< Tick size in ms */
#define TIMERS_N    3               /*!< Total timers to handle */

/**
 @} */

static void Heart_Init(void);
static void Heart_Beat(void);
static void Dog_Init(void);
static void Peth_The_Dog(void);

/**
 *@brief   Watchdog Timer Handler
 */
WWDG_HandleTypeDef  WwdgHandler;                                     /* cppcheck-suppress misra-c2012-8.4 ; Used also in app_ints.c */


/**
 *@brief   Scheduler handler
 */
Scheduler_HandleTypeDef  hscheduler = {0};                          /* cppcheck-suppress misra-c2012-8.4 ; Used also in app_clock.c, app_display.c and app_serial.c*/


/**
 *@brief   Main function
 *
 * Initialize scheduler handling structures
 * Use a scheduler to initialize the features (heartbeat, watchdog, serial, clock and display)
 * and execute periodically their tasks (Heart_Beat, Peth_The_Dog, Serial_Task, Clock_Task and
 * Display_Task)
 * 
 *@retval  Returns an Integer (Expected 0 for a successful execution)
 */
int main( void )
{
    HAL_StatusTypeDef Status;

    static Task_TypeDef             tasks[ TASKS_N ];
    static Timer_TypeDef            timers[ TIMERS_N ];

    Status = HAL_Init( );
    assert_error( Status == HAL_OK, HAL_ERROR );                    /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    hscheduler.tick    = TICK_VAL;
    hscheduler.tasks   = TASKS_N;
    hscheduler.taskPtr = tasks;
    hscheduler.timerPtr = timers;
    hscheduler.timers   = TIMERS_N;

    HIL_SCHEDULER_Init( &hscheduler );

    (void)HIL_SCHEDULER_RegisterTask( &hscheduler, Heart_Init,   Heart_Beat,   300UL );
    (void)HIL_SCHEDULER_RegisterTask( &hscheduler, Dog_Init,     Peth_The_Dog, 190UL );
    (void)HIL_SCHEDULER_RegisterTask( &hscheduler, Serial_Init,  Serial_Task,  10UL );
    (void)HIL_SCHEDULER_RegisterTask( &hscheduler, Clock_Init,   Clock_Task,   50UL );
    (void)HIL_SCHEDULER_RegisterTask( &hscheduler, Display_Init, Display_Task, 100UL );
    

    HIL_SCHEDULER_Start( &hscheduler );

    return 0u;
}

/**
 * @brief   Function to take the uC to a Safe State
 * 
 * Disbable interruptions
 * Deinitialize used peripherals as FDCAN and SPI
 * Deinitialize GPIO
 * Disable clocks
 * Set error on leds connected to GPIO portc
 * 
 * @param   file    File where error is located
 * @param   line    Line where error is located
 * @param   error   Error type 
 */
void safe_state( uint8_t *file, uint32_t line, uint8_t error )
{
    /*disable all maskable interrupts*/
    __disable_irq();

    /*Deinitialize FDCAN periph*/
    HAL_GPIO_DeInit( GPIOD, GPIO_PIN_0 | GPIO_PIN_1 );
    __HAL_RCC_FDCAN_CLK_DISABLE();

    /*Deinitialize SPI periph*/
    HAL_GPIO_DeInit( GPIOD, GPIO_PIN_6 | GPIO_PIN_8 );
    __SPI1_CLK_DISABLE();

    /*Deinitialize LED pin*/
    HAL_GPIO_DeInit( GPIOC, GPIO_PIN_0 );

    /*Disable other clocks*/
    __HAL_RCC_SYSCFG_CLK_DISABLE();
    __HAL_RCC_PWR_CLK_DISABLE();
    
    /*disable all timers included the WWDG*/
    __HAL_RCC_RTC_DISABLE();
    __HAL_RCC_TIM6_CLK_DISABLE();
    
    /*output the error code using the leds connected to port C*/

    GPIO_InitTypeDef  GPIO_InitStruct;

    __HAL_RCC_GPIOC_CLK_ENABLE( );

    GPIO_InitStruct.Pin   = 0x00FF;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );

    HAL_GPIO_WritePin( GPIOC, error, GPIO_PIN_SET );

    (void)file;
    (void)line;

    for( ; ; )
    {
        
    }
}


/**
 *@brief   Initialize watchdog timer
 *
 * Initializes Windowed Watchdog Timer and sets its min value reset time to 114ms and max to 1032ms
 * 
 */
void Dog_Init(void)
{
    HAL_StatusTypeDef Status;
    //      tmin_WWDG  = ( 1/32000000 )( 4096 * 128 )*( 127 - 64 )*1000 = 1032ms
    //      tmin_WWDG  = -( 1/32000000 )( 4096 * 128 )*( 120 - 127 )*1000 = 114ms
    //      the worst case requires 1404ms (printing with printf)
    //      the standard case requires 14us (idle state on both machines)
    //      All the PRINT state in app_clock inserts a delay on the microcontroller

    __HAL_RCC_WWDG_CLK_ENABLE();

    WwdgHandler.Instance       = WWDG;
    WwdgHandler.Init.Prescaler = WWDG_PRESCALER_128;
    WwdgHandler.Init.Counter   = 127;
    WwdgHandler.Init.Window    = 120;
    WwdgHandler.Init.EWIMode   = WWDG_EWI_DISABLE;

    Status = HAL_WWDG_Init( &WwdgHandler );
    assert_error( Status == HAL_OK, HAL_RET_ERROR );              /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
}


/**
 *@brief   Reset Watchdog counter
 *
 * Set the WWDG counter to 0 to avoid system reset
 * 
 */
static void Peth_The_Dog(void)
{
    HAL_StatusTypeDef Status;
    //The PRINT state of app_clock inserts a delay on microncontroller, so based on oscilloscope measurements, when we want to refresh the WWDG counter
    //every 1400ms, the following block executes every 1800ms (due to the delay), so with a tickvalue of 800ms actually the block executes every 1400ms
    //enough to cover all the state machines execution and reset the WWDG counter in time

    Status = HAL_WWDG_Refresh( &WwdgHandler );
    assert_error( Status == HAL_OK, HAL_RET_ERROR );        /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
}


/**
 *@brief   Initialize Heartbeat
 *
 * Configure pin 0 of GPIO PORTC as output to use the connected led as a hearbeat indicator
 * 
 */
void Heart_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    __HAL_RCC_GPIOC_CLK_ENABLE( );

    GPIO_InitStruct.Pin   = GPIO_PIN_0;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );

    HAL_GPIO_WritePin( GPIOC, GPIO_PIN_0, GPIO_PIN_RESET );
}


/**
 *@brief   Run Heartbeat
 *
 * Toggle GPIO PORTC pin 0 state as a heartbeat
 * 
 */
void Heart_Beat(void)
{
    HAL_GPIO_TogglePin( GPIOC, GPIO_PIN_0 );
}