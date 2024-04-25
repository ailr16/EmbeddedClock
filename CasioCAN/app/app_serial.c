/**
 * @file    app_serial.c
 * @brief   ****
 *
 * app_serial driver source:
 *
 */


#include "app_serial.h"

/** 
  * @defgroup <TimeElement> Define element number for time data
  @{ */

#define     HOUR        1               /*!< Element 1 for Hour */
#define     MINUTES     2               /*!< Element 2 for Hour */
#define     SECONDS     3               /*!< Element 3 for Hour */

/**
  @} */


/** 
  * @defgroup <DateElement> Define element number for date data
  @{ */
#define     DAY         1               /*!< Element 1 for Day */
#define     MONTH       2               /*!< Element 2 for Day */
#define     YEAR_H      3               /*!< Element 3 for Day */
#define     YEAR_L      4               /*!< Element 4 for Day */
/**
  @} */


/** 
  * @defgroup <MonthElement> Define month number
  @{ */
#define     JAN         1               /*!< January   1 */
#define     FEB         2               /*!< February  2 */
#define     MAR         3               /*!< March     3 */
#define     APR         4               /*!< April     4 */
#define     MAY         5               /*!< May       5 */
#define     JUN         6               /*!< June      6 */
#define     JUL         7               /*!< July      7 */
#define     AUG         8               /*!< August    8 */
#define     SEP         9               /*!< September 9 */
#define     OCT         10              /*!< October  10 */
#define     NOV         11              /*!< November 11 */
#define     DEC         12              /*!< December 12 */
/**
  @} */


/*Other defines-----*/

/** 
* @defgroup <OtherDefines> CANTP size and Element for message type
@{ */

#define     CANTP_MSIZE                 8               /*!< 8 bytes size for CANTP            */
#define     M_TYPE                      0               /*!< Element 0 for storing message type */
#define     CAN_SUCCESSFUL_OPERATION    0x55            /*!< Value to transmit if CAN operation was successful */
#define     CAN_INVALID_OPERATION       0xAA            /*!< Value to transmit if CAN operation couldn't performed */

/**
  @} */


/** 
* @defgroup <SerialQueueDefines> Defines for serial queue
@{ */

#define     SERIAL_QUEUE_ELEMENTS   9               /*!< Max buffered messages */

/**
  @} */


/*Functions for app_serial----------------*/

static void     CanTp_SingleFrameTx( uint8_t *data, uint8_t size );
static uint8_t  CanTp_SingleFrameRx( uint8_t *data, uint8_t *size );
static uint8_t  Bcd2Decimal(uint8_t byteBcd);
static void     ClearData( uint8_t *data , uint8_t size);

static void     Serial_EventMachine( NEW_MsgTypeDef *receivedMessage );

/*----------------------------------------*/


/*Time&Date validation functions----------*/

static uint8_t  Validate_Time(uint8_t hour, uint8_t minutes, uint8_t seconds);
static uint8_t  Validate_Date(uint8_t days, uint8_t month, uint16_t year);
static uint8_t  WeekDay(uint8_t days, uint8_t month, uint16_t year);
static uint16_t YearDay( uint8_t days, uint8_t month, uint16_t year );

/*----------------------------------------*/


/*Structs---------------------------------*/

extern FDCAN_HandleTypeDef   CANHandler;            /* cppcheck-suppress misra-c2012-8.5 ; CANHandler used in app_ints.c too */
       FDCAN_HandleTypeDef   CANHandler    = {0};
       
static FDCAN_TxHeaderTypeDef CANTxHeader   = {0};

/*----------------------------------------*/


/*Global variables------------------------*/

static uint8_t RxMessageFlag = 0;
extern QUEUE_HandleTypeDef clockQueue;              /* cppcheck-suppress misra-c2012-8.5 ; clockQueue defined in app_clock.c */

/*----------------------------------------*/

static QUEUE_HandleTypeDef serialQueue = {0};

/**
 * @brief   Use once to Initialize the app_serial
 */
void Serial_Init( void )
{
    FDCAN_FilterTypeDef   CANFilter = {0};
    HAL_StatusTypeDef     Status;

    CANHandler.Instance                  = FDCAN1;
    CANHandler.Init.Mode                 = FDCAN_MODE_NORMAL;
    CANHandler.Init.FrameFormat          = FDCAN_FRAME_CLASSIC;
    CANHandler.Init.ClockDivider         = FDCAN_CLOCK_DIV1;
    CANHandler.Init.TxFifoQueueMode      = FDCAN_TX_FIFO_OPERATION;
    CANHandler.Init.NominalPrescaler     = 20;       //20 for 100kbps, 8 for 250kbps
    CANHandler.Init.NominalSyncJumpWidth = 1;
    CANHandler.Init.NominalTimeSeg1      = 11;
    CANHandler.Init.NominalTimeSeg2      = 4;
    CANHandler.Init.StdFiltersNbr        = 1;

    Status = HAL_FDCAN_Init( &CANHandler );
    assert_error( Status == HAL_OK, CAN_RET_ERROR );        /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    //CAN Filter Configuration
    CANFilter.IdType        = FDCAN_STANDARD_ID;
    CANFilter.FilterIndex   = 0;
    CANFilter.FilterType    = FDCAN_FILTER_MASK;
    CANFilter.FilterConfig  = FDCAN_FILTER_TO_RXFIFO0;
    CANFilter.FilterID1     = 0x111;
    CANFilter.FilterID2     = 0x7FF;
    
    Status = HAL_FDCAN_ConfigFilter( &CANHandler, &CANFilter );
    assert_error( Status == HAL_OK, RCC_RET_ERROR );        /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    Status = HAL_FDCAN_ConfigGlobalFilter( &CANHandler, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE );
    assert_error( Status == HAL_OK, CAN_RET_ERROR );        /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    Status = HAL_FDCAN_Start( &CANHandler );
    assert_error( Status == HAL_OK, CAN_RET_ERROR );        /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    Status = HAL_FDCAN_ActivateNotification( &CANHandler, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0 );
    assert_error( Status == HAL_OK, CAN_RET_ERROR );        /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    //Configure Tx Header
    CANTxHeader.IdType      = FDCAN_STANDARD_ID;
    CANTxHeader.FDFormat    = FDCAN_CLASSIC_CAN;
    CANTxHeader.TxFrameType = FDCAN_DATA_FRAME;
    CANTxHeader.DataLength  = FDCAN_DLC_BYTES_8;

    static NEW_MsgTypeDef serialBuffer[SERIAL_QUEUE_ELEMENTS];

    serialQueue.Buffer   = (void*)serialBuffer;
    serialQueue.Elements = SERIAL_QUEUE_ELEMENTS;
    serialQueue.Size     = sizeof( NEW_MsgTypeDef );

    HIL_QUEUE_Init( &serialQueue );
}


/**
 * @brief   Run continuosuly to receive messages trough CAN and decode data
 * 
 * Check if there is a message in the serial queue and run the event machine
 * 
 */
void Serial_Task( void )
{
    NEW_MsgTypeDef RxData;

    while( HIL_QUEUE_IsEmptyISR( &serialQueue, TIM16_FDCAN_IT0_IRQn ) == 0U )
    {
        (void)HIL_QUEUE_ReadISR( &serialQueue, RxData.data, TIM16_FDCAN_IT0_IRQn );
        Serial_EventMachine( &RxData );
    }
}


/**
 * @brief   Serial Machine function
 * 
 * When a message as received through serial, its type will be used
 * to move in the event machine.
 * 
 * Once the message has been decoded in function of its type (ALARM,
 * DATE or TIME), the data is saved in a struct and pushed into a
 * queue used by the clock machine.
 * 
 * If the received data is not valid the machine moves to the ERROR
 * state which sends a specified error message through CAN, otherwise
 * enter to the OK state and sends a specified ok message by the same
 * bus
 * 
 * * @param receivedMessage Message from serial queue of NEW_MsgTypeDef
 * 
 */
void Serial_EventMachine( NEW_MsgTypeDef *receivedMessage )
{
    HAL_StatusTypeDef   Status;
    APP_Messages        messageType;

    static uint8_t TxData[CANTP_MSIZE];
    static APP_MsgTypeDef messageStruct = {0};

    messageType = receivedMessage->data[M_TYPE];

    switch( messageType )
    {
        case SERIAL_MSG_ERROR:

            messageStruct.msg = SERIAL_MSG_NONE;

            ClearData( TxData, 8 );
            TxData[0] = CAN_INVALID_OPERATION;
            CanTp_SingleFrameTx( TxData, 1 );

        break;

        case SERIAL_MSG_OK:
            
            messageStruct.msg = SERIAL_MSG_NONE;

            ClearData( TxData, 8 );
            TxData[0] = CAN_SUCCESSFUL_OPERATION;
            CanTp_SingleFrameTx( TxData, 1 );

        break;

        case SERIAL_MSG_ALARM:

            messageStruct.msg    = SERIAL_MSG_ERROR;

            messageStruct.tm.tm_hour_a = Bcd2Decimal( receivedMessage->data[HOUR] );
            messageStruct.tm.tm_min_a  = Bcd2Decimal( receivedMessage->data[MINUTES] );

            if( Validate_Time( messageStruct.tm.tm_hour_a, messageStruct.tm.tm_min_a, 0 ) == 1U)
            {
                messageStruct.msg = SERIAL_MSG_ALARM;
                Status = HIL_QUEUE_WriteISR( &clockQueue, &messageStruct, TIM16_FDCAN_IT0_IRQn );
                assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

                messageStruct.msg = SERIAL_MSG_OK;
            }
            
            Status = HIL_QUEUE_WriteISR( &serialQueue, &messageStruct, TIM16_FDCAN_IT0_IRQn );
            
        break;

        case SERIAL_MSG_DATE:

            messageStruct.msg = SERIAL_MSG_ERROR;

            messageStruct.tm.tm_mday = Bcd2Decimal( receivedMessage->data[DAY] );
            messageStruct.tm.tm_mon  = Bcd2Decimal( receivedMessage->data[MONTH] );
            messageStruct.tm.tm_year = ( (Bcd2Decimal( receivedMessage->data[YEAR_H] ) * 100U) + Bcd2Decimal( receivedMessage->data[YEAR_L] ) );
            messageStruct.tm.tm_wday = WeekDay( messageStruct.tm.tm_mday, messageStruct.tm.tm_mon, messageStruct.tm.tm_year );
            messageStruct.tm.tm_yday = YearDay( messageStruct.tm.tm_mday, messageStruct.tm.tm_mon, messageStruct.tm.tm_year );

            if( Validate_Date( messageStruct.tm.tm_mday, messageStruct.tm.tm_mon, messageStruct.tm.tm_year ) == 1U )
            {
                messageStruct.msg = SERIAL_MSG_DATE;
                Status = HIL_QUEUE_WriteISR( &clockQueue, &messageStruct, TIM16_FDCAN_IT0_IRQn );
                assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

                messageStruct.msg = SERIAL_MSG_OK;
            }
            
            Status = HIL_QUEUE_WriteISR( &serialQueue, &messageStruct, TIM16_FDCAN_IT0_IRQn );

        break;

        case SERIAL_MSG_TIME:
            
            messageStruct.msg = SERIAL_MSG_ERROR;

            messageStruct.tm.tm_hour = Bcd2Decimal( receivedMessage->data[HOUR] );
            messageStruct.tm.tm_min  = Bcd2Decimal( receivedMessage->data[MINUTES] );
            messageStruct.tm.tm_sec  = Bcd2Decimal( receivedMessage->data[SECONDS] );

            if( Validate_Time( messageStruct.tm.tm_hour, messageStruct.tm.tm_min, messageStruct.tm.tm_sec) == 1U )
            {
                messageStruct.msg = SERIAL_MSG_TIME;
                Status = HIL_QUEUE_WriteISR( &clockQueue, &messageStruct, TIM16_FDCAN_IT0_IRQn );
                assert_error( Status == 1U, QUEUE_RET_ERROR );                          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

                messageStruct.msg = SERIAL_MSG_OK;
            }

            Status = HIL_QUEUE_WriteISR( &serialQueue, &messageStruct, TIM16_FDCAN_IT0_IRQn );
            
        break;

        default:
        break;
    }
}


/**
 * @brief   Transmit a CAN single frame message
 * @param   data 8-byte message to transmit
 * @param   size 8-bit value for size, 1 to 7
 */
static void CanTp_SingleFrameTx( uint8_t *data, uint8_t size )
{
    HAL_StatusTypeDef Status;
    
    uint8_t i;

    if( (size > 0U) && (size < 8U) )
    {
        for(i = 7U; i >= 1U; i--)
        {
            data[i] = data[i - 1U];
        }
        data[0] = size;

        CANTxHeader.Identifier = 0x122;

        Status = HAL_FDCAN_AddMessageToTxFifoQ( &CANHandler, &CANTxHeader, data );
        assert_error( Status == HAL_OK, CAN_RET_ERROR );        /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    }
}


/**
     * @brief   Decode a received CAN single frame message
     * @param   data 8-byte array to save the decoded message
     * @param   size 8-bit value for size, 1 to 7
     * @retval  1 for successful transmission
     */
static uint8_t CanTp_SingleFrameRx( uint8_t *data, uint8_t *size )
{
    HAL_StatusTypeDef Status;

    uint8_t rxStatus = 0U;
    uint8_t i;
    uint8_t sizeAux;

    static FDCAN_RxHeaderTypeDef CANRxHeader   = {0};

    if( RxMessageFlag == 1U )
    {
        RxMessageFlag = 0U;

        Status = HAL_FDCAN_GetRxMessage( &CANHandler, FDCAN_RX_FIFO0, &CANRxHeader, data );
        assert_error( Status == HAL_OK, CAN_RET_ERROR );            /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

        sizeAux = data[0] & 0xFU;

        if( (sizeAux > 0U) && (sizeAux < 7U) )
        {
            *size = sizeAux;
            for(i = 0U; i < 7U; i++)
            {
                data[i] = data[i + 1U];
            }
            rxStatus = 1U;
        }
    }

    return rxStatus;
}


/**
    *@brief   Callback for received FDCAN message interrupt
    *@param   hfdcan FDCAN Handler
    *@param   RxFifo0ITs FDCAN Interruption flag
    */
/* cppcheck-suppress misra-c2012-2.7 ; Function provided by ST in HAL DONT CHANGE*/
/* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL DONT CHANGE*/
void HAL_FDCAN_RxFifo0Callback( FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs )
{   
    HAL_StatusTypeDef Status;

    static uint8_t size;
    static NEW_MsgTypeDef    RxMessageAux = {0};

    RxMessageFlag = 1U;

    Status = CanTp_SingleFrameRx( RxMessageAux.data, &size );
    assert_error( Status == 1U, CAN_RET_ERROR );                                        /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    Status = HIL_QUEUE_WriteISR( &serialQueue, &RxMessageAux, TIM16_FDCAN_IT0_IRQn );
    assert_error( Status == 1, QUEUE_RET_ERROR );                                       /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
}


/**
    * @brief   Convert BCD to decimal
    * @param   byteBcd BCD value to convert
    * @retval  Decimal format value
    */
static uint8_t Bcd2Decimal(uint8_t byteBcd)
{
    return (byteBcd & 0x0FU) + (byteBcd >> 4) * 10U ;
}


/**
    * @brief   Clear array data
    * @param   data Array which is gonna be cleared
    */
static void ClearData( uint8_t *data, uint8_t size)
{
    App_ErrorsCode x;
    x = CAN_RET_ERROR;
    if(x == CAN_RET_ERROR)
    {
        x = RCC_RET_ERROR;
    }

    for(uint8_t i = 0U; i < size; i++)
    {   
        data[i] = 0U;
    }
}


/*Time&Date validation functions---------------------------------------------------------*/


/**
    * @brief   Validate hour, minutes and seconds
    * @param   hour 8bit value for hour, 0 to 23
    * @param   minutes 8bit value for minutes, 0 to 59
    * @param   seconds 8bit value for seconds, 0 to 59
    * @retval  1 for valid date, 0 invalid date
    */
static uint8_t Validate_Time(uint8_t hour, uint8_t minutes, uint8_t seconds)
{
    uint8_t hourValidation    = 0U;
    uint8_t minutesValidation = 0U;
    uint8_t secondsValidation = 0U;

    if( hour < 24U )
    {
        hourValidation = 1U;
    }

    if( minutes < 60U )
    {
        minutesValidation = 1U;
    }

    if( seconds < 60U )
    {
        secondsValidation = 1U;
    }

    return ( hourValidation && minutesValidation && secondsValidation );
}


/**
    * @brief   Validate date
    * @param   days 8bit value for days, 1 to 31
    * @param   month 8bit value for month, 1 to 12
    * @param   year 8bit value for seconds, 1900 to 2100
    * @retval  1 for valid date, 0 invalid date
    */
static uint8_t Validate_Date(uint8_t days, uint8_t month, uint16_t year)
{
    uint8_t yearValidation = 0;
    uint8_t monthValidation = 0;
    uint8_t dayValidation = 0;

    if( (year > 1899U) && (year < 2101U) )
    {
        yearValidation = 1U;
    }

    if( (month > 0U) && (month < 13U) )
    {
        monthValidation = 1U;
    }
    
    if( (month == (uint8_t)JAN) || (month == (uint8_t)MAR) || (month == (uint8_t)MAY) || (month == (uint8_t)JUL) || (month == (uint8_t)AUG) || (month == (uint8_t)OCT) || (month == (uint8_t)DEC) )
    {
        if( (days > 0U) && (days < 32U) )
        {
            dayValidation = 1U;
        }
    }

    if( (month == (uint8_t)APR) || (month == (uint8_t)JUN) || (month == (uint8_t)SEP) || (month == (uint8_t)NOV) )
    {
        if( (days > 0U) && (days < 31U) )
        {
            dayValidation = 1U;
        }
    }

    if( ((year % 4U) == 0U) && (year != 1900U) && (year != 2100U) )  //Leap year validation
    { 
        if( month == (uint8_t)FEB )
        {
            if( (days > 0U) && (days < 30U) )
            {
                dayValidation = 1U;
            }
        }
    }

    else{
        if( month == (uint8_t)FEB )
        {
            if( (days > 0U) && (days < 29U) )
            {
                dayValidation = 1;
            }
        }
    }

    return ( yearValidation && monthValidation && dayValidation );
}


/**
    * @brief   Get weekday from date
    * @param   days 8bit value for days, 1 to 31
    * @param   month 8bit value for month, 1 to 12
    * @param   year 8bit value for seconds, 1900 to 2100
    * @retval  Value of week day, 0 to 6
    */
static uint8_t WeekDay( uint8_t days, uint8_t month, uint16_t year )
{
    uint16_t  A;
    uint16_t  Y;
    uint16_t  M;
    uint16_t  D = 0U;

    A = (14U - (uint16_t)month) / 12U;
    Y = year - A;
    M = month + (12U * A) - 2U;
    D = ( ((days + Y + (Y / 4U) - (Y / 100U) + (Y / 400U) + (M * 31U) )/ 12U) % 7U );

    return (uint8_t)D;
}


/**
    * @brief   Get weekday from date
    * @param   days 8bit value for days, 1 to 31
    * @param   month 8bit value for month, 1 to 12
    * @param   year 8bit value for seconds, 1900 to 2100
    * @retval  Value of year day, 0 to 366
    */
static uint16_t YearDay( uint8_t days, uint8_t month, uint16_t year )
{
    uint16_t yearDay      = 0U;
    uint8_t  leapYearFlag = 0U;

    if( ((year % 4U) == 0U) && (year != 1900U) && (year != 2100U) )
    {
        leapYearFlag = 1;
    }

    for(uint8_t monthI = 1U; monthI < month; monthI++)
    {
        if( (monthI == (uint8_t)JAN) || (monthI == (uint8_t)MAR) || (monthI == (uint8_t)MAY) || (monthI == (uint8_t)JUL) || (monthI == (uint8_t)AUG) || (monthI == (uint8_t)OCT) || (monthI == (uint8_t)DEC) )
        {
            yearDay += 31U;
        }
        if( (monthI == (uint8_t)APR) || (monthI == (uint8_t)JUN) || (monthI == (uint8_t)SEP) || (monthI == (uint8_t)NOV) )
        {
            yearDay += 30U;
        }
        if( monthI == (uint8_t)FEB )
        {
            if(leapYearFlag == 1U)
            {
                yearDay += 29U;
            }
            else
            {
                yearDay += 28U;
            }
        }
    }

    yearDay += days;
    yearDay--;

    return yearDay;
}
/*---------------------------------------------------------Time&Date validation functions*/
