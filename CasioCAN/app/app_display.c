/**
 * @file    app_display.c
 * @brief   Source file with definitions of functions for app_display
 *
 * Allows to print date, time and alarm data in LCD
 * 
 * Handle events using a queue shared with app_clock which is used  to
 * retrieve the data must be printed in LCD
 * 
 * Find here the functions that allow to handle a task scheduler:
 * Initialize app_display
 * Run app_display event machine
 * Date and time validation
 * Convert data into string values to print
 * 
 * Read two potentiometers with ADC to adjust the LCD contrast and intensity
 * Read the internal temperature sensor and prints in the LCD
 * Implements second lecture for each potentiometer as safety requirement
 *
 */

#include "app_display.h"
#include "app_analogs.h"


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


/** 
* @defgroup <displayQUEUEDefines> Values for APP_DISPLAY queue
@{ */

#define     DISPLAY_QUEUE_ELEMENTS   90UL               /*!< Max buffered messages */

/**
  @} */


/** 
* @defgroup <AlarmDefines> Values for handling alarms
@{ */

#define     ALARM_SET_DISPLAY       23UL    /*!< Use when alarm status mut be printed in hour parameter                             */

/**
  @} */


/** 
* @defgroup <ADCDefines> Values for ADC handling
@{ */

#define     ADC_CHANNELS_N         5U    /*!< Total ADC channels */
#define     ADC_MAX_DEVIATION      10U   /*!< Max deviation percentage between comparison of same signal measured with 2 ADC channels*/
#define     DMA_ELEMENT_POT0       0U    /*!< Element number where POT0 conversion is stored*/ 
#define     DMA_ELEMENT_POT1       1U    /*!< Element number where POT0 conversion is stored*/ 
#define     DMA_ELEMENT_TEMP       2U    /*!< Element number where Tmperature conversion is stored*/ 
#define     DMA_ELEMENT_POT0SAFE   3U    /*!< Element number where POT0 conversion is stored (used for safety)*/
#define     DMA_ELEMENT_POT1SAFE   4U    /*!< Element number where POT1 conversion is stored (used for safety)*/

/**
  @} */


QUEUE_HandleTypeDef displayQueue = {0};                 /* cppcheck-suppress misra-c2012-8.4 ; Used also in app_clock.c */

extern QUEUE_HandleTypeDef clockQueue;                  /* cppcheck-suppress misra-c2012-8.5 ; clockQueue defined in app_clock.c */


/**
 * @brief   LCD Struct Handler
 *
 */
 LCD_HandleTypeDef LcdHandler = {0};                    /* cppcheck-suppress misra-c2012-8.4 ; Used in app_analogs.c */


static void DateString( char *string, uint8_t month, uint8_t day, uint16_t year, uint8_t weekday );
static void TimeString( char *string, uint8_t hours, uint8_t minutes, uint8_t seconds);
static void AlarmString( char *string, uint8_t hours, uint8_t minutes );

static void ClearString( char *string );
static void AlarmMString( char *string );

static void Display_EventMachine(APP_MsgTypeDef *receivedMessage);

static void Display_LcdTask( void );
static void TemperatureString( int8_t temperatureIn, char *string );

/**
 * @brief   TIM14 Struct handler
 *
 */
static TIM_HandleTypeDef    hTIM14  = {0};


/**
 * @brief   ADC Handler
 */
ADC_HandleTypeDef AdcHandler = {0};                       /* cppcheck-suppress misra-c2012-8.4 ; Used also in app_ints.c */


/**
 * @brief   DMA Handler
 *
 */
DMA_HandleTypeDef DmaHandler = {0};                       /* cppcheck-suppress misra-c2012-8.4 ; Used also in app_ints.c */


/**
 * @brief   TIM3 Struct handler (used PWM function for backlight control)
 *
 */
static TIM_HandleTypeDef hTIM3 = {0};


extern uint32_t AdcData[ADC_CHANNELS_N];

/**
 * @brief   Flag enabled when alarm has been triggered
 *
 */
static uint8_t alarmTriggered = 0U;

/**
 * @brief   Use once to Initialize the app_display
 * 
 * Initialize SPI1 peripheral
 * Initialize LCD
 * Initialize display queue
 * Initialize TIM PWM peripheral with 1kHz with Duty Cycle 50%
 *
 */
void Display_Init( void )
{
    static APP_MsgTypeDef     displayBuffer[DISPLAY_QUEUE_ELEMENTS];
    static SPI_HandleTypeDef  spiAux;
    static TIM_OC_InitTypeDef sConfig = {0};
    HAL_StatusTypeDef         Status;

    __SPI1_CLK_ENABLE();

    spiAux.Instance               = SPI1;
    spiAux.Init.Mode              = SPI_MODE_MASTER;
    spiAux.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;        //APB = 32MHz => SPI = 4MHz
    spiAux.Init.Direction         = SPI_DIRECTION_2LINES;
    spiAux.Init.CLKPhase          = SPI_PHASE_2EDGE;
    spiAux.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    spiAux.Init.DataSize          = SPI_DATASIZE_8BIT;
    spiAux.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    spiAux.Init.NSS               = SPI_NSS_SOFT;
    spiAux.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
    spiAux.Init.TIMode            = SPI_TIMODE_DISABLED;

    LcdHandler.SpiHandler = &spiAux;

    Status = HAL_SPI_Init( LcdHandler.SpiHandler );   //Init SPI
    assert_error( Status == HAL_OK, SPI_RET_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    LcdHandler.BklPin  = GPIO_PIN_4;
    LcdHandler.BklPort = GPIOB;
    LcdHandler.CsPin   = GPIO_PIN_3;
    LcdHandler.CsPort  = GPIOD;
    LcdHandler.RsPin   = GPIO_PIN_4;
    LcdHandler.RsPort  = GPIOD;
    LcdHandler.RstPin  = GPIO_PIN_2;
    LcdHandler.RstPort = GPIOD;

    Status = HEL_LCD_Init( &LcdHandler );
    assert_error( Status == HAL_OK, LCD_RET_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    displayQueue.Buffer   = (void*)displayBuffer;
    displayQueue.Elements = DISPLAY_QUEUE_ELEMENTS;
    displayQueue.Size     = sizeof( APP_MsgTypeDef );

    HIL_QUEUE_Init( &displayQueue );

    /*      Init TIM14 PWM for buzzer       */
    hTIM14.Instance         = TIM14;
    hTIM14.Init.Prescaler   = 2;
    hTIM14.Init.Period      = 16000;                                //1kHz output with APB = 32MHz
    hTIM14.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_PWM_Init( &hTIM14 );

    sConfig.OCMode      = TIM_OCMODE_PWM1;
    sConfig.OCPolarity  = TIM_OCPOLARITY_HIGH;
    sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfig.Pulse       = 8000;                                     //Duty cycle of 50% (16000/2)
    HAL_TIM_PWM_ConfigChannel( &hTIM14, &sConfig, TIM_CHANNEL_1 );


    /*      Init TIM3 for backlight PWM       */
    hTIM3.Instance         = TIM3;
    hTIM3.Init.Prescaler   = 1000;
    hTIM3.Init.Period      = 100;                                //640Hz output with APB = 32MHz
    hTIM3.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_PWM_Init( &hTIM3 );

    sConfig.OCMode      = TIM_OCMODE_PWM1;
    sConfig.OCPolarity  = TIM_OCPOLARITY_HIGH;
    sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfig.Pulse       = 50;                   //Duty cycle of 50% (100/2)
    HAL_TIM_PWM_ConfigChannel( &hTIM3, &sConfig, TIM_CHANNEL_1 );

    HAL_TIM_PWM_Start( &hTIM3, TIM_CHANNEL_1 );

    Analogs_Init();
}

/**
 * @brief   Run continuosuly to print data in LCD
 * 
 * Check if there is a message in the display queue and run the event machine
 * 
 */
void Display_Task( void )
{
  static APP_MsgTypeDef messageStruct = {0};

  while (HIL_QUEUE_IsEmptyISR(&displayQueue, TIM16_FDCAN_IT0_IRQn) == 0U)
  {
    (void)HIL_QUEUE_ReadISR(&displayQueue, &messageStruct, TIM16_FDCAN_IT0_IRQn);
    Display_EventMachine(&messageStruct);
  }
}


/**
 * @brief   Display Machine function
 * 
 * When a message as received through app_clock using the display queue,
 * its type will be used to move in the event machine.
 * 
 * The only state of this machine prints data in the LCD, and requires the
 * message type DISPLAY_MSG_PRINT which is set by app_clock
 * 
 * Prints Date and hour
 * 
 * @param receivedMessage Message from display queue of APP_MsgTypeDef type
 */
void Display_EventMachine(APP_MsgTypeDef *receivedMessage)
{
  HAL_StatusTypeDef Status;
  APP_Messages      messageType;

  static APP_MsgTypeDef    messageToClock = {0};

  static uint8_t    alarmSoundFlag = 0;
  static uint8_t    alarmTimeoutCounter = 0;

  static char str[16];


  messageType = receivedMessage->msg;

  switch( messageType )
  {
    case DISPLAY_MSG_PRINT:

    //Update the data every second

      DateString(str, receivedMessage->tm.tm_mon, receivedMessage->tm.tm_mday, receivedMessage->tm.tm_year, receivedMessage->tm.tm_wday);

      Status = HEL_LCD_SetCursor(&LcdHandler, 0, 1);
      assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

      Status = HEL_LCD_String(&LcdHandler, str);
      assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

      TimeString(str, receivedMessage->tm.tm_hour, receivedMessage->tm.tm_min, receivedMessage->tm.tm_sec);

      Status = HEL_LCD_SetCursor(&LcdHandler, 1, 2);
      assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

      Status = HEL_LCD_String(&LcdHandler, str);
      assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */


    break;

    case DISPLAY_MSG_SETAL:

        (void)strcpy( str, "A  " );                     //Print character A when the alarm is setted

        Status = HEL_LCD_SetCursor(&LcdHandler, 1, 0);
        assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

        Status = HEL_LCD_String( &LcdHandler, str );
        assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    break;

    case DISPLAY_MSG_DISAL:

        alarmTriggered = 0U;
        HAL_TIM_PWM_Start( &hTIM3, TIM_CHANNEL_1 );       //Turn on the backligth
        HAL_TIM_PWM_Stop( &hTIM14, TIM_CHANNEL_1 );     //Turn off the buzzer

        Status = HEL_LCD_SetCursor(&LcdHandler, 1, 0);
        assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

        Status = HEL_LCD_Data( &LcdHandler, '   ' );      //Clear character A when the alarm is setted
        assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    break;

    case DISPLAY_MSG_PRAL:

      if( receivedMessage->tm.tm_hour == ALARM_SET_DISPLAY )    //If an alarm was set, print the info when the button is pressed
      {
        AlarmString(str, receivedMessage->tm.tm_hour_a, receivedMessage->tm.tm_min_a );

        Status = HEL_LCD_SetCursor(&LcdHandler, 1, 3);
        assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

        Status = HEL_LCD_String(&LcdHandler, str);
        assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
      }
      else
      {
        (void)strcpy( str, "ALARM NO CONFIG" );                 //If the alarm has not been set, print alert when the button is pressed

        Status = HEL_LCD_SetCursor(&LcdHandler, 1, 0);
        assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

        Status = HEL_LCD_String(&LcdHandler, str);
        assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
      }

    break;

    case DISPLAY_MSG_ALARM:

      //Indicate that the alarm has been triggered
      alarmTriggered = 1U;
      alarmTimeoutCounter++;

      if( alarmTimeoutCounter == 59U )                  //Disable alarm after 60 seconds 
      {
        alarmTimeoutCounter = 0;
        messageToClock.msg = CLOCK_DIS_ALARM;
        Status = HIL_QUEUE_WriteISR( &clockQueue, &messageToClock, TIM16_FDCAN_IT0_IRQn );
        assert_error(Status == HAL_OK, QUEUE_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
      }

      //Still updating the date
      DateString(str, receivedMessage->tm.tm_mon, receivedMessage->tm.tm_mday, receivedMessage->tm.tm_year, receivedMessage->tm.tm_wday);

      Status = HEL_LCD_SetCursor(&LcdHandler, 0, 1);
      assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

      Status = HEL_LCD_String(&LcdHandler, str);
      assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

      Status = HEL_LCD_SetCursor(&LcdHandler, 1, 0);
      assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

      Status = HEL_LCD_Data( &LcdHandler, ' ' );
      assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

      AlarmMString( str );                            //Print ALARM!!! 

      Status = HEL_LCD_SetCursor(&LcdHandler, 1, 0);
      assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

      Status = HEL_LCD_String(&LcdHandler, str);
      assert_error(Status == HAL_OK, LCD_RET_ERROR); /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

      alarmSoundFlag ^= 1U;

      if( alarmSoundFlag == 0U )                      //Toggle buzzer every second
      {
        HAL_TIM_PWM_Start( &hTIM14, TIM_CHANNEL_1 );
        __HAL_TIM_SET_COMPARE( &hTIM3, TIM_CHANNEL_1, 0 );
      }
      else
      {
        HAL_TIM_PWM_Stop( &hTIM14, TIM_CHANNEL_1 );
        __HAL_TIM_SET_COMPARE( &hTIM3, TIM_CHANNEL_1, Analogs_GetIntensity() );
      }

    break;

    default:
    break;
  }
}

/**
 * @brief   Format date into a string
 * @param   string 8-bit string where the result will be saved
 * @param   month 8-bit value for month, 1 to 12
 * @param   day 8-bit value for month, 1 to 31
 * @param   year 16-bit value for year, 1900 to 2099
 * @param   weekday 8-bit value for week day, 0 to 6
 */
static void DateString( char *string, uint8_t month, uint8_t day, uint16_t year, uint8_t weekday )
{   
    char months[][4] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};        /* cppcheck-suppress misra-c2012-9.4 ; String array for moths */
    char days[][3] = {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"};                                                   /* cppcheck-suppress misra-c2012-9.4 ; String array for days */

    (void) strcpy( &string[0], months[month - 1U] );

    string[3] = ',';
    
    string[4] = ( day / 10U ) + 48U; 
    string[5] = ( day % 10U ) + 48U;

    string[6] = ' ';

    string[7] = ( year / 1000U ) + 48U;
    string[8] = (( year % 1000U ) / 100U) + 48U;
    string[9] = (((year % 1000U ) % 100U) / 10U) + 48U;
    string[10] = (((year % 1000U) % 100U) % 10U) + 48U;
    string[11] = ' ';

    (void) strcpy( &string[12], days[weekday - 1U] );

    string[14] = '\0';
}

/**
 * @brief   Format time into a string
 * @param   string 8-bit string where the result will be saved
 * @param   hours 8-bit value for hours, 0 to 23
 * @param   minutes 8-bit value for minutes, 0 to 59
 * @param   seconds 16-bit value for seconds, 0 to 59
 */
static void TimeString( char *string, uint8_t hours, uint8_t minutes, uint8_t seconds){
    
    ClearString( string );

    string[0] = ' ';
    string[1] = ' ';

    string[1] = ( hours / 10U ) + 48U;
    string[2] = ( hours % 10U ) + 48U;

    string[3] = ':';

    string[4] = ( minutes / 10U ) + 48U;
    string[5] = ( minutes % 10U ) + 48U;

    string[6] = ':';
    
    string[7] = ( seconds / 10U ) + 48U;
    string[8] = ( seconds % 10U ) + 48U;

    string[9] = ' ';
    string[10] = '\0';
}


/**
 * @brief   Format alarm time into a string
 * @param   string 8-bit string where the result will be saved
 * @param   hours 8-bit value for hours, 0 to 23
 * @param   minutes 8-bit value for minutes, 0 to 59
 */
static void AlarmString( char *string, uint8_t hours, uint8_t minutes){
    
    ClearString( string );

    (void)strcpy( string, "ALARM=" );

    string[6] = ( hours / 10U ) + 48U;
    string[7] = ( hours % 10U ) + 48U;

    string[8] = ':';

    string[9] = ( minutes / 10U ) + 48U;
    string[10] = ( minutes % 10U ) + 48U;

    string[11] = ' ';
    string[12] = '\0';
}


/**
 * @brief   'Clear' string
 * 
 * Sets a space char ' ' for all characters of a string
 * 
 * @param   string 8-bit string where the result will be saved
 *
 */
static void ClearString( char *string )
{
  uint8_t i;

  for(i = 0; i < strlen( string ); i++)
  {
    string[i] = ' ';
  }
}


/**
 * @brief   Set alarm message string
 * 
 * Sets the characters "    ALARM!!!" into a string
 * 
 * @param   string 8-bit string where the result will be saved
 *
 */
static void AlarmMString( char *string )
{
  ClearString( string );

  (void)strcpy( string, "    ALARM!!!    " );
}


/**
 * @brief   ADC interrupt callback
 * 
 * Function called when the Analog-to-Digital conversion is complete
 * 
 * According to the ADC configuration, this is triggered every 50ms using TIM2
 * 
 * Internally, call the Display_LcdTask which updates LCD contrast, backlight
 * intensity and temperature.
 * 
 * @param   string 8-bit string where the result will be saved
 *
 */
/* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  (void)hadc;
  Display_LcdTask();

  HAL_ADC_Start_DMA( &AdcHandler, &AdcData[0], ADC_CHANNELS_N );
}

/**
 * @brief   Update backlight intensity, contrast and temperature
 * 
 * Update values of LCD backligth intensity, LCD contrast and
 * internal temperature if changed
 * 
 */
void Display_LcdTask( void )
{
  HAL_StatusTypeDef Status;

  static int8_t  temperature = 0;
  static uint8_t contrast    = 0U;
  static uint8_t intensity   = 0U;

  static int8_t  newTemperature = 0;
  static uint8_t newContrast    = 0U;
  static uint8_t newIntensity   = 0U;

  static char strTemp[5];

  newIntensity   = Analogs_GetIntensity();
  newContrast    = Analogs_GetContrast();
  newTemperature = Analogs_GetTemperature();

  if( intensity != newIntensity )
  {
    intensity = newIntensity;
  }

  if( contrast != newContrast )
  {
    contrast = newContrast;
  }

  if( temperature != newTemperature )
  {
    temperature = newTemperature;
  }

  if( alarmTriggered == 0U )
  {
    contrast = (contrast * 63U) / 15U;
    Status = HEL_LCD_Contrast( &LcdHandler, contrast );
    assert_error( Status == HAL_OK, LCD_PAR_ERROR);                                           /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    __HAL_TIM_SET_COMPARE( &hTIM3, TIM_CHANNEL_1, intensity );

    TemperatureString( temperature, strTemp );                                            /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    Status = HEL_LCD_SetCursor( &LcdHandler, 1U, 12U );
    assert_error( Status == HAL_OK, LCD_PAR_ERROR);                                           /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    Status = HEL_LCD_String( &LcdHandler, strTemp );
    assert_error( Status == HAL_OK, LCD_PAR_ERROR);                                           /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
  }

}


/**
 * @brief   Save temperature value into a string
 * 
 * Convert the temperature value in a char string, detects the special cases
 * if number is greater than 99 to print 3 characters or less than 0 to
 * print 2 characters and the '-' sign
 * 
 * @param temperature Value between -40 and 125
 * @param string Five-element char array
 */
static void TemperatureString( int8_t temperatureIn, char *string )
{
  int8_t temperature;

  temperature = temperatureIn;

  if( temperature < 0 )
  {
    temperature = -temperature;
    string[0] = '-';
    string[1] = (temperature / 10) + 0x30;
    string[2] = (temperature % 10) + 0x30;
    string[3] = 'C';
    string[4] = '\0';
  }
  else if( temperature > 99 )
  {
    string[0] = (temperature / 100) + 0x30;
    string[1] = ((temperature % 100) / 10) + 0x30;
    string[2] = ((temperature % 100) % 10) + 0x30;
    string[3] = 'C';
    string[4] = '\0';
  }
  else
  {
    string[0] = (temperature / 10) + 0x30;
    string[1] = (temperature % 10) + 0x30;
    string[2] = 'C';
    string[3] = ' ';
    string[4] = '\0';
  }
}