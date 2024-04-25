/**
 * @file    app_msps.c
 * @brief   ****
 *
 * File with MSP init functions
 *
 */

#include "app_bsp.h"

/**
 *@brief   MspInit function. Configures the system clocks
 */
void HAL_MspInit( void )                        /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    HAL_StatusTypeDef  Status;

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    Status = HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
    assert_error( Status == HAL_OK, PWR_RET_ERROR );            /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    /* Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure
    The frequency set is 64MHz with the internal 16MHz HSI oscilator. According to the formulas:
    fVCO = fPLLIN x ( N / M ) = 16MHz x (8 / 1) = 128MHz
    fPLLP = fVCO / P = 128MHz / 2 = 64MHz
    fPLLQ = fVCO / Q = 128MHz / 2 = 64MHz
    fPLLR = fVCO / R = 128MHz / 2 = 64MHz
    */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSIDiv              = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN            = 8;
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2;

    Status = HAL_RCC_OscConfig( &RCC_OscInitStruct );
    assert_error( Status == HAL_OK, RCC_RET_ERROR );            /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    
    /** Initializes the CPU, AHB and APB buses clocks*/
    RCC_ClkInitStruct.ClockType       = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider   = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider  = RCC_HCLK_DIV2;

    Status = HAL_RCC_ClockConfig( &RCC_ClkInitStruct, FLASH_LATENCY_2 );
    assert_error( Status == HAL_OK, RCC_RET_ERROR );            /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
}

/**
 *@brief   FDCAN Msp Init function. Configures FDCAN peripheral
 *@param   hfdcan FDCAN Handler
 */
/* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
/* cppcheck-suppress misra-c2012-2.7 ; Function provided by ST in HAL */
void HAL_FDCAN_MspInit( FDCAN_HandleTypeDef *hfdcan ){
    GPIO_InitTypeDef GpioCanStruct;

    __HAL_RCC_FDCAN_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GpioCanStruct.Mode = GPIO_MODE_AF_PP;
    GpioCanStruct.Alternate = GPIO_AF3_FDCAN1;
    GpioCanStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GpioCanStruct.Pull = GPIO_NOPULL;
    GpioCanStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( GPIOD, &GpioCanStruct );

    HAL_NVIC_SetPriority( TIM16_FDCAN_IT0_IRQn, 2, 0 );
    HAL_NVIC_EnableIRQ( TIM16_FDCAN_IT0_IRQn );
}


/**
 *@brief   RTC Msp Init function. Configures RTC peripheral
 *@param   hrtc RTC Handler
 */
/* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
/* cppcheck-suppress misra-c2012-2.7 ; Function provided by ST in HAL */
void HAL_RTC_MspInit( RTC_HandleTypeDef *hrtc )
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    HAL_StatusTypeDef Status;

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    Status = HAL_PWREx_ControlVoltageScaling( PWR_REGULATOR_VOLTAGE_SCALE1 );
    assert_error( Status == HAL_OK, PWR_RET_ERROR );            /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_LSEDRIVE_CONFIG( RCC_LSEDRIVE_LOW );

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_NONE;

    Status = HAL_RCCEx_PeriphCLKConfig ( &PeriphClkInitStruct );
    assert_error( Status == HAL_OK, RCC_RET_ERROR );            /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;

    Status = HAL_RCC_OscConfig( &RCC_OscInitStruct );
    assert_error( Status == HAL_OK, RCC_RET_ERROR );            /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    
    Status = HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct );
    assert_error( Status == HAL_OK, RCC_RET_ERROR );            /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    __HAL_RCC_RTC_ENABLE();
    __HAL_RCC_RTCAPB_CLK_ENABLE();

}

/**
 * @brief   SPI Msp Init function. Configures SPI peripheral
 * @param   hspi SPI Handler
 */
/* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
/* cppcheck-suppress misra-c2012-2.7 ; Function provided by ST in HAL */
void HAL_SPI_MspInit( SPI_HandleTypeDef *hspi )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __GPIOD_CLK_ENABLE();
    __SPI1_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_SPI1;

    HAL_GPIO_Init( GPIOD, &GPIO_InitStruct );
}

/**
 * @brief   TIM14 Msp Init function. Configures TIM14 peripheral for PWM
 * @param   htim Timer handlers
 */
/* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
/* cppcheck-suppress misra-c2012-2.7 ; Function provided by ST in HAL */
void HAL_TIM_PWM_MspInit( TIM_HandleTypeDef *htim )
{
    GPIO_InitTypeDef   GPIO_InitStruct;

    __TIM14_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_12;              /*pin C12 as TIM14_CH1*/
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM14;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    __TIM3_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_4;              /*pin B4 as TIM13_CH1*/
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/**
 * @brief   ADC Msp Init function
 * 
 * Init Pins 0, 1, 6 and 7 as analog inputs
 * 
 * @param   hadc ADC handler
 */
/* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
void HAL_ADC_MspInit( ADC_HandleTypeDef* hadc ){
    GPIO_InitTypeDef GPIO_InitStruct;

    __ADC_CLK_ENABLE();
    __GPIOA_CLK_ENABLE();

    (void)hadc;

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
}