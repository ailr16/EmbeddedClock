/**
 * @file    app_clock.c
 * @brief   Source file with definitions of functions for app_clock
 *
 * Allows to register date, time and alarm time into the RTC peripheral
 * 
 * Handle events using a queue shared with app_display which allows to
 * print data in LCD
 * 
 * Find here the functions that allow to handle a task scheduler:
 * Initialize app_clock
 * Run app_clock event machine
 * Callback function for 1000ms timer
 * EXTI callback for button in PB05
 *
 */


#include "app_clock.h"


/** 
* @defgroup <CLKQUEUEDefines> Values for APP_CLOCK queue
@{ */

#define     CLOCK_QUEUE_ELEMENTS   45UL      /*!< Max buffered messages */

/**
  @} */


/** 
* @defgroup <AlarmDefines> Values for handling alarms
@{ */

#define     ALARM_IS_SET            1U      /*!< Use when an alarm has been set                                                     */
#define     ALARM_IS_UNSET          0U      /*!< Use when an alarm is not set                                                       */
#define     ALARM_TRIGGERED         1U      /*!< Use when an alarm has been triggered                                               */
#define     ALARM_NOT_TRIGGERED     0U      /*!< Use when an alarm is not triggered                                                 */
#define     ALARM_PRINT_LCD         1U      /*!< Use when the alarm status must be printed                                          */
#define     ALARM_NOT_PRINT_LCD     0U      /*!< Use when normal time and date must be printed                                      */
#define     ALARM_SET_DISPLAY       23UL    /*!< Use when alarm status mut be printed in hour parameter                             */
#define     ALARM_NOT_SET_DISPLAY   13UL    /*!< Use when alarm status mut be printed in hour parameter but alarm was not set       */

/**
  @} */


/**
 * @brief  Struct for clock queue
 */
QUEUE_HandleTypeDef clockQueue = {0};               /* cppcheck-suppress misra-c2012-8.4 ; Used also in app_serial.c */


/**
 * @brief  Struct for display queue
 */
extern QUEUE_HandleTypeDef displayQueue;


/**
 * @brief  Struct for handling RTC peripheral
 */
extern RTC_HandleTypeDef RtcHandler;
       RTC_HandleTypeDef RtcHandler = {0};


/**
 *@brief   ID of timer used for display timing
 */
static uint8_t DisplayTimerID = 0;


/**
 *@brief   Scheduler handler. Declaration in main.c
 */
extern Scheduler_HandleTypeDef  hscheduler;


static void PrintDisplay1000ms(void);
static void Clock_EventMachine( APP_MsgTypeDef *receivedMessage );


/**
 *@brief   Print Alarm Status flag
 *
 * Set to ALARM_NOT_PRINT_LCD when the normal date and time must be printed in LCD
 * Set to ALARM_PRINT_LCD when the alarm status must be printed in LCD
 * Initialize PB05 as input for use a button
 * 
 */
static uint8_t printAlarm = ALARM_NOT_PRINT_LCD;


/**
 * @brief   Use once to Initialize the app_clock
 */
void Clock_Init( void )
{
    HAL_StatusTypeDef Status;
    GPIO_InitTypeDef  GPIO_InitStruct;

    RTC_DateTypeDef  sDate   = {0};
    RTC_TimeTypeDef  sTime   = {0};

    static APP_MsgTypeDef clockBuffer[CLOCK_QUEUE_ELEMENTS];            //Buffer for clock queue

    RtcHandler.Instance          = RTC;
    RtcHandler.Init.HourFormat   = RTC_HOURFORMAT_24;
    RtcHandler.Init.AsynchPrediv = 127;
    RtcHandler.Init.SynchPrediv  = 255;
    RtcHandler.Init.OutPut       = RTC_OUTPUT_DISABLE;
    RtcHandler.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RtcHandler.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
    
    Status = HAL_RTC_Init( &RtcHandler );
    assert_error( Status == HAL_OK, RTC_RET_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    sTime.Hours          = 0x12;
    sTime.Minutes        = 0x00;
    sTime.Seconds        = 0x00;
    sTime.SubSeconds     = 0x00;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;

    Status = HAL_RTC_SetTime( &RtcHandler, &sTime, RTC_FORMAT_BCD );
    assert_error( Status == HAL_OK, RTC_RET_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    sDate.Month   = RTC_MONTH_JANUARY;
    sDate.WeekDay = RTC_WEEKDAY_SATURDAY;
    sDate.Date    = 0x16;
    sDate.Year    = 0x99;

    Status = HAL_RTC_SetDate( &RtcHandler, &sDate, RTC_FORMAT_BCD );
    assert_error( Status == HAL_OK, RTC_RET_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    clockQueue.Buffer   = (void*)clockBuffer;
    clockQueue.Elements = CLOCK_QUEUE_ELEMENTS;
    clockQueue.Size     = sizeof( APP_MsgTypeDef );

    HIL_QUEUE_Init( &clockQueue );

    DisplayTimerID = HIL_SCHEDULER_RegisterTimer( &hscheduler, 1000UL, PrintDisplay1000ms );        //Add timer for sending data to display
    (void)HIL_SCHEDULER_StartTimer( &hscheduler, DisplayTimerID );

    __HAL_RCC_GPIOB_CLK_ENABLE( );  

    GPIO_InitStruct.Pin   = GPIO_PIN_5;                         //Init button PB05
    GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );                   //Pin 5 PORTB as rising/falling interrupt

    HAL_NVIC_SetPriority( EXTI4_15_IRQn, 2, 0 );
    HAL_NVIC_EnableIRQ( EXTI4_15_IRQn );                        //Enable interrupt

}


/**
 * @brief   Run continuosuly to configure the clock
 * 
 * Check if there is a message in the clock queue and run the event machine
 * 
 */
void Clock_Task( void )
{
    static APP_MsgTypeDef messageStruct = {0};

    while( HIL_QUEUE_IsEmptyISR( &clockQueue, TIM16_FDCAN_IT0_IRQn ) == 0U )
    {
        (void)HIL_QUEUE_ReadISR( &clockQueue, &messageStruct, TIM16_FDCAN_IT0_IRQn );
        Clock_EventMachine( &messageStruct );
    }
}


/**
 * @brief   Clock Machine function
 * 
 * When a message as received through app_serial using the clock queue,
 * its type will be used to move in the event machine.
 * 
 * Once the message has been decoded in function of its type (ALARM,
 * DATE or TIME), the data is saved in a struct and pushed into a
 * queue used by the display machine.
 * 
 * A CLOCK_MSG_PRINT type indicates that the data of RTC must be pushed
 * to the display queue which will print in LCD
 * 
 * According to alarm status, the machine will move between states to
 * push data into the display queue to indicate if an alarm has been set,
 * disabled or triggered
 * 
 * @param receivedMessage Message from clock queue of APP_MsgTypeDef type
 */
void Clock_EventMachine( APP_MsgTypeDef *receivedMessage )
{
    HAL_StatusTypeDef Status;
    APP_Messages      messageType;

    static APP_MsgTypeDef clockMessage = {0};
    static uint8_t setAlarmFlag        = ALARM_IS_UNSET;                //Start with no alarm
    static uint8_t alarmFlag           = ALARM_NOT_TRIGGERED;           //Start with the alarm not triggered

    RTC_DateTypeDef  sDate   = {0};
    RTC_AlarmTypeDef sAlarm  = {0};
    RTC_TimeTypeDef  sTime   = {0};
    
    static uint8_t   dateYearH  = 19;

    messageType = receivedMessage->msg;

    switch( messageType )
    {
        case SERIAL_MSG_ALARM:

            sAlarm.Alarm                    = RTC_ALARM_A;
            sAlarm.AlarmMask                = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_SECONDS;    //Ignore weekday and seconds
            sAlarm.AlarmSubSecondMask       = RTC_ALARMSUBSECONDMASK_ALL;                           //Igonre subseconds
            sAlarm.AlarmDateWeekDaySel      = RTC_ALARMDATEWEEKDAYSEL_DATE;
            sAlarm.AlarmDateWeekDay         = 1;
            sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
            sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
            
            setAlarmFlag = ALARM_IS_SET;

            sAlarm.AlarmTime.Hours   = receivedMessage->tm.tm_hour_a;
            sAlarm.AlarmTime.Minutes = receivedMessage->tm.tm_min_a;
            
            Status = HAL_RTC_SetAlarm_IT( &RtcHandler, &sAlarm, RTC_FORMAT_BIN );
            assert_error( Status == HAL_OK, RTC_RET_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

            clockMessage.msg = CLOCK_SET_ALARM;                             //Message to indicate to display that the alarm was set
            
            if( alarmFlag == ALARM_TRIGGERED )                              //Stop the alarm when a message has been received
            {
                clockMessage.msg = CLOCK_DIS_ALARM;                         //Message to stop display alarm triggered indicators
            }

            Status = HIL_QUEUE_WriteISR( &clockQueue, &clockMessage, TIM16_FDCAN_IT0_IRQn );
            assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

        break;

        case SERIAL_MSG_DATE:

            sDate.Date  = receivedMessage->tm.tm_mday;
            sDate.Month = receivedMessage->tm.tm_mon;
            sDate.Year  = receivedMessage->tm.tm_year % 100UL;
            dateYearH   = receivedMessage->tm.tm_year / 100UL;

            Status = HAL_RTC_SetDate( &RtcHandler, &sDate, RTC_FORMAT_BIN);
            assert_error( Status == HAL_OK, RTC_RET_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

            if( alarmFlag == ALARM_TRIGGERED )                              //Disable the alarm when a message has been received
            {
                clockMessage.msg = CLOCK_DIS_ALARM;
                Status = HIL_QUEUE_WriteISR( &clockQueue, &clockMessage, TIM16_FDCAN_IT0_IRQn );
                assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
            }

            clockMessage.msg = CLOCK_MSG_PRINT;
            Status = HIL_QUEUE_WriteISR( &clockQueue, &clockMessage, TIM16_FDCAN_IT0_IRQn );
            assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

        break;

        case SERIAL_MSG_TIME:

            sTime.Hours   = receivedMessage->tm.tm_hour;
            sTime.Minutes = receivedMessage->tm.tm_min;
            sTime.Seconds = receivedMessage->tm.tm_sec;

            Status = HAL_RTC_SetTime( &RtcHandler, &sTime, RTC_FORMAT_BIN);
            assert_error( Status == HAL_OK, RTC_RET_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
            
            if( alarmFlag == ALARM_TRIGGERED )                              //Disable the alarm when a message has been received
            {
                clockMessage.msg = CLOCK_DIS_ALARM;
                Status = HIL_QUEUE_WriteISR( &clockQueue, &clockMessage, TIM16_FDCAN_IT0_IRQn );
                assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
            }

            clockMessage.msg = CLOCK_MSG_PRINT;
            Status = HIL_QUEUE_WriteISR( &clockQueue, &clockMessage, TIM16_FDCAN_IT0_IRQn );
            assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

        break;

        case CLOCK_SET_ALARM:

            clockMessage.msg = DISPLAY_MSG_SETAL;                          //With this message the display will print 'A' character when the alarm was set
            Status = HIL_QUEUE_WriteISR( &displayQueue, &clockMessage, TIM16_FDCAN_IT0_IRQn );
            assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

        break;

        case CLOCK_DIS_ALARM:
            
            Status = HAL_RTC_DeactivateAlarm( &RtcHandler, RTC_ALARM_A );
            assert_error( Status == HAL_OK, RTC_RET_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

            setAlarmFlag     = ALARM_IS_UNSET;
            alarmFlag        = ALARM_NOT_TRIGGERED;

            clockMessage.msg = DISPLAY_MSG_DISAL;                           //With this message the display erase 'A' character when the alarm was disabled
            Status = HIL_QUEUE_WriteISR( &displayQueue, &clockMessage, TIM16_FDCAN_IT0_IRQn );
            assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

        break;

        case CLOCK_PRINT_ALARM:

            Status = HAL_RTC_GetAlarm( &RtcHandler, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN );
            assert_error( Status == HAL_OK, RTC_RET_ERROR );            /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

            if( alarmFlag == ALARM_TRIGGERED )                          //Disable alarm if the button was pressed and alarm was triggered
            {
                clockMessage.msg = CLOCK_DIS_ALARM;
                Status = HIL_QUEUE_WriteISR( &clockQueue, &clockMessage, TIM16_FDCAN_IT0_IRQn );
                assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
            }
            else
            {
                if( setAlarmFlag == ALARM_IS_SET )                      //Print set alarm time when the button is pressed
                {
                    clockMessage.tm.tm_hour_a = sAlarm.AlarmTime.Hours;
                    clockMessage.tm.tm_min_a  = sAlarm.AlarmTime.Minutes;
                    clockMessage.tm.tm_hour   = ALARM_SET_DISPLAY;

                    clockMessage.msg = DISPLAY_MSG_PRAL;
                    Status = HIL_QUEUE_WriteISR( &displayQueue, &clockMessage, TIM16_FDCAN_IT0_IRQn );
                    assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
                }
                else
                {
                    clockMessage.tm.tm_hour = ALARM_NOT_SET_DISPLAY;    //Print message indicating alarm has not been set
                    clockMessage.msg = DISPLAY_MSG_PRAL;
                    Status = HIL_QUEUE_WriteISR( &displayQueue, &clockMessage, TIM16_FDCAN_IT0_IRQn );
                    assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
                }
            }

        break;

        case CLOCK_MSG_PRINT:

            Status = HAL_RTC_GetTime( &RtcHandler, &sTime, RTC_FORMAT_BIN );
            assert_error( Status == HAL_OK, RTC_RET_ERROR );            /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

            Status = HAL_RTC_GetDate( &RtcHandler, &sDate, RTC_FORMAT_BIN );
            assert_error( Status == HAL_OK, RTC_RET_ERROR );            /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

            clockMessage.tm.tm_hour = sTime.Hours;
            clockMessage.tm.tm_min  = sTime.Minutes;
            clockMessage.tm.tm_sec  = sTime.Seconds;
            clockMessage.tm.tm_year = sDate.Year + ( dateYearH * 100U );
            clockMessage.tm.tm_mday = sDate.Date;
            clockMessage.tm.tm_wday = sDate.WeekDay;
            clockMessage.tm.tm_mon  = sDate.Month;

            clockMessage.msg = DISPLAY_MSG_PRINT;

            if( setAlarmFlag == ALARM_IS_SET)                               //Verify if an alarm has been set
            {
                Status = HAL_RTC_PollForAlarmAEvent( &RtcHandler, 10 );     //Check if alarm has been activated
                if( Status == HAL_OK )
                {
                    alarmFlag = ALARM_TRIGGERED;                            //The alarm has been triggered
                }

                if( alarmFlag == ALARM_TRIGGERED)
                {
                    clockMessage.msg = DISPLAY_MSG_ALARM;                   //Indicate the alarm trigger to display
                }
            }
            
            Status = HIL_QUEUE_WriteISR( &displayQueue, &clockMessage, TIM16_FDCAN_IT0_IRQn );

        break;

        default:
        break;  

    }
}


/**
 *@brief   Callback function for display timer
 *
 * Set flag which indicates the time to update data to show on LCD
 * 
 */
void PrintDisplay1000ms(void)
{
    HAL_StatusTypeDef Status;
    static APP_MsgTypeDef clockMessage = {0};

    if( printAlarm == ALARM_NOT_PRINT_LCD )
    {
        clockMessage.msg = CLOCK_MSG_PRINT;
        Status = HIL_QUEUE_WriteISR( &clockQueue, &clockMessage, TIM16_FDCAN_IT0_IRQn );
        assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    }
    
    Status = HIL_SCHEDULER_StartTimer( &hscheduler, DisplayTimerID );
    assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
}


/**
 * @brief   EXTI Callback when button is released
 * @param   GPIO_Pin Pin of GPIO to validate
 */
void HAL_GPIO_EXTI_Rising_Callback( uint16_t GPIO_Pin )                         /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    (void)GPIO_Pin;
    printAlarm = ALARM_NOT_PRINT_LCD;
}


/**
 * @brief   EXTI Callback when button is pressed
 * @param   GPIO_Pin Pin of GPIO to validate
 */
void HAL_GPIO_EXTI_Falling_Callback( uint16_t GPIO_Pin )                        /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    HAL_StatusTypeDef Status;
    static APP_MsgTypeDef clockMessage = {0};

    (void)GPIO_Pin;

    printAlarm = ALARM_PRINT_LCD;

    clockMessage.msg = CLOCK_PRINT_ALARM;
    Status = HIL_QUEUE_WriteISR( &clockQueue, &clockMessage, TIM16_FDCAN_IT0_IRQn );
    assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
}
