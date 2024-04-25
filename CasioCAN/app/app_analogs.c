/**
 * @file    app_analogs.c
 * @brief   Source file with definitions of functions for app_analogs
 * 
 * Find here the functions that allow to read values of two potentiometers with
 * the ADC peripheral:
 *      Analogs_GetIntensity
 *      Analogs_GetContrast
 * 
 * And the function to read the internal temperature sensor:
 *      Analogs_GetTemperature
 * 
 * Use Analog_Init before implementing the three specified functions
 * 
 * NOTE Implements second lecture for each potentiometer as safety requirement
 *
 */

#include "app_analogs.h"

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


/**
 * @brief   Array to store ADC data with DMA
 */
uint32_t AdcData[ADC_CHANNELS_N];             /* cppcheck-suppress misra-c2012-8.4 ; Used in app_display.c */


/**
 * @brief   Initialize peripherals for analog conversion
 * 
 * Initialize the ADC1 with 5 channels, two channels for reading a potentiometer,
 * other two channels for other potentiometer and one channel for internal temp
 * sensor
 * 
 * Initialize DMA1 Channel 1 for copying acquired data from ADC peripheral to
 * memory
 * 
 * Initialize Timer 2 for using as trigger signal of ADC1 (50ms timing rate)
 * 
 * @param   string 8-bit string where the result will be saved
 *
 */
void Analogs_Init( void )
{
  static TIM_HandleTypeDef       hTIM2         = {0};       //Used as trigger for ADC
  static TIM_MasterConfigTypeDef sMasterConfig = {0};
  static ADC_ChannelConfTypeDef  sChanConfig   = {0};

  /*      Init TIM2       */
  __HAL_RCC_TIM2_CLK_ENABLE();

  hTIM2.Instance = TIM2;
  hTIM2.Init.Prescaler = 16000;
  hTIM2.Init.CounterMode = TIM_COUNTERMODE_UP;
  hTIM2.Init.Period = 200;                                  //Reaches the full count every 50ms
  hTIM2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  hTIM2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init( &hTIM2 );

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
  HAL_TIMEx_MasterConfigSynchronization( &hTIM2, &sMasterConfig );

  
  /*      Init DMA        */
  __HAL_RCC_DMA1_CLK_ENABLE();

  DmaHandler.Instance = DMA1_Channel1;
  DmaHandler.Init.Request = DMA_REQUEST_ADC1;
  DmaHandler.Init.Direction = DMA_PERIPH_TO_MEMORY;
  DmaHandler.Init.PeriphInc = DMA_PINC_DISABLE;
  DmaHandler.Init.MemInc = DMA_MINC_ENABLE;
  DmaHandler.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  DmaHandler.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  DmaHandler.Init.Mode = DMA_NORMAL;
  DmaHandler.Init.Priority = DMA_PRIORITY_HIGH;
  HAL_DMA_Init( &DmaHandler );

  __HAL_LINKDMA( &AdcHandler, DMA_Handle , DmaHandler);
  HAL_NVIC_SetPriority( DMA1_Channel1_IRQn, 2, 0 );
  HAL_NVIC_EnableIRQ( DMA1_Channel1_IRQn );


  /*      Init ADC        */
  AdcHandler.Instance = ADC1;
  AdcHandler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  AdcHandler.Init.Resolution = ADC_RESOLUTION_12B;
  AdcHandler.Init.ScanConvMode = ADC_SCAN_SEQ_FIXED;
  AdcHandler.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandler.Init.ContinuousConvMode = DISABLE;
  AdcHandler.Init.DiscontinuousConvMode = DISABLE;
  AdcHandler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING;
  AdcHandler.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T2_TRGO;
  AdcHandler.Init.NbrOfConversion = ADC_CHANNELS_N;
  AdcHandler.Init.DMAContinuousRequests = DISABLE;
  AdcHandler.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  AdcHandler.Init.OversamplingMode = ENABLE;              //Enable oversampling
  AdcHandler.Init.Oversampling.Ratio = ADC_OVERSAMPLING_RATIO_4;
  AdcHandler.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  AdcHandler.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_2;

  HAL_ADC_Init( &AdcHandler );


  /*      Init ADC channels        */
  sChanConfig.Channel = ADC_CHANNEL_0;                            //POT0 connected
  sChanConfig.Rank = ADC_REGULAR_RANK_1;                          //First channel to read
  sChanConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  HAL_ADC_ConfigChannel( &AdcHandler, &sChanConfig );

  sChanConfig.Channel = ADC_CHANNEL_1;                            //POT1 connected
  sChanConfig.Rank = ADC_REGULAR_RANK_2;                          //Second channel to read
  sChanConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  HAL_ADC_ConfigChannel( &AdcHandler, &sChanConfig );

  sChanConfig.Channel = ADC_CHANNEL_TEMPSENSOR;                   //Internal temp sensor
  sChanConfig.Rank = ADC_REGULAR_RANK_3;
  sChanConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  HAL_ADC_ConfigChannel( &AdcHandler, &sChanConfig );

  sChanConfig.Channel = ADC_CHANNEL_6;                            //POT0 extra channel (for safety requirement)
  sChanConfig.Rank = ADC_REGULAR_RANK_4;
  sChanConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  HAL_ADC_ConfigChannel( &AdcHandler, &sChanConfig );

  sChanConfig.Channel = ADC_CHANNEL_7;                            //POT1 extra channel (for safety requirement)
  sChanConfig.Rank = ADC_REGULAR_RANK_5;
  sChanConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  HAL_ADC_ConfigChannel( &AdcHandler, &sChanConfig );

  HAL_ADCEx_Calibration_Start( &AdcHandler );


  /*      Start DMA        */
  HAL_ADC_Start_DMA( &AdcHandler, &AdcData[0], ADC_CHANNELS_N );


  /*      Start Timer        */
  HAL_TIM_Base_Start( &hTIM2 );
}


/**
 * @brief   Get intesnity value from potentiometer
 * 
 * Returns a value from 0 to 100 in 10 increments (0, 10, 20, ..., 90, 100) which represents
 * the LCD backlight intensity (0 for null intensity and 100 for full intensity)
 * 
 * @retval  Value from 0 to 100
 *
 */
uint8_t Analogs_GetIntensity( void )
{
  uint32_t intensity       = 0U;
  uint32_t intensitySafety = 0U;
  uint32_t deviationSafety = 0U;

  intensity       = AdcData[DMA_ELEMENT_POT0];
  intensitySafety = AdcData[DMA_ELEMENT_POT0SAFE];
  deviationSafety = (intensity * ADC_MAX_DEVIATION) / 100U;
  assert_error( ( intensitySafety < (intensity + deviationSafety) ) | ( intensitySafety > (intensity - deviationSafety) ), ADC_DEVIATION_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

  intensity = ((intensity * 10U) / 4095U) * 10U;
  
  return intensity;
}


/**
 * @brief   Get contrast value from potentiometer
 * 
 * Returns a value from 0 to 15 which represents the LCD contrast
 * (0 for null intensity and 15 for all contrast)
 * 
 * @retval  Value from 0 to 15
 *
 */
uint8_t Analogs_GetContrast( void )
{
  uint32_t contrast        = 0U;
  uint32_t contrastSafety  = 0U;
  uint32_t deviationSafety = 0U;

  contrast        = AdcData[DMA_ELEMENT_POT1];
  contrastSafety  = AdcData[DMA_ELEMENT_POT1SAFE];
  deviationSafety = (contrast * ADC_MAX_DEVIATION) / 100U;
  assert_error( ( contrastSafety < (contrast + deviationSafety) ) | ( contrastSafety > (contrast - deviationSafety) ), ADC_DEVIATION_ERROR );             /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

  contrast = (contrast * 15U) / 4095U;
  
  return contrast;
}


/**
 * @brief   Get temperature value
 * 
 * Returns a value from -40 to 125 which represents the temperature in
 * Celsius from internal sensor
 * 
 * @retval  Value from -40 to 125
 *
 */
int8_t Analogs_GetTemperature( void )
{
  return (__HAL_ADC_CALC_TEMPERATURE( 3200, AdcData[DMA_ELEMENT_TEMP], ADC_RESOLUTION12b ) );
}

