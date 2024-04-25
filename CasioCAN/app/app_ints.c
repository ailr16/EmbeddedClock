/**
 * @file    app_ints.c
 * @brief   ****
 *
 * File with interrupt handling functions, check startup_stm32g0b1.S
 *
 */

#include "app_bsp.h"


/**
 *@brief   Handler
 */
void NMI_Handler( void )                        /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    
}


/**
 *@brief   Handler
 */
void HardFault_Handler( void )                  /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    assert_param( 0u );
    assert_error( 0u, HARDFAULT_FUNC_ERROR );   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
}


/**
 *@brief   Handler
 */
void SVC_Handler( void )                        /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{

}


/**
 *@brief   Handler
 */
void PendSV_Handler( void )                     /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{

}


/**
 *@brief   Handler for systick
 */
void SysTick_Handler( void )                    /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    HAL_IncTick( );
}


/**
 *@brief   Handler for FDCAN peripheral
 */
extern FDCAN_HandleTypeDef CANHandler;          /* cppcheck-suppress misra-c2012-8.5 ; CANHandler declared and defined in app_serial.c*/


/**
 *@brief   Interruption handler for FDCAN
 */
void TIM16_FDCAN_IT0_IRQHandler( void )         /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    /**
     *@brief   Interruption handler for FDCAN
     */
    HAL_FDCAN_IRQHandler( &CANHandler );
}


/**
 * @brief   Interruption handler for FDCAN error
 * @param   hfdcan FDCAN handler
 */
void HAL_FDCAN_ErrorCallback( FDCAN_HandleTypeDef *hfdcan )         /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */      
{
    assert_error( 0U, CAN_FUNC_ERROR );         /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    (void)hfdcan;
}


/**
 * @brief   Interruption handler for SPI error
 * @param   hspi SPI Handler
 */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)                 /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    assert_error( 0U, SPI_FUNC_ERROR );         /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    (void)hspi;
}


/**
 *@brief   Struct for WWDG handler
 */
extern WWDG_HandleTypeDef WwdgHandler;


/**
 *@brief   Interruption handler for WWDG early wakeup
 */
void WWDG_IRQHandler( void )                                        /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    HAL_WWDG_IRQHandler( &WwdgHandler );
}


/**
 * @brief   Interruption callback for WWDG early wakeup
 * @param   hwwdg Windowed Wathdog Timer handler
 */
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)            /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    assert_error( 0U, WWDG_FUNC_ERROR );        /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    (void)hwwdg;
}


/**
 * @brief   Interruption handler for ECC error
 * @param   a Required value
 */
void HAL_FLASH_OperationErrorCallback(uint32_t a)               /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    HAL_FLASH_GetError();
    assert_error( 0U, ECC_FUNC_ERROR );         /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    (void)a;
}


/**
 * @brief   Interruption handler for External signals
 * 
 * External Interrupt Handler which is used for button connected to GPIOB PIN5
 * 
 */
void EXTI4_15_IRQHandler( void )                                /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_5 );
}


/**
 *@brief   Interrupt handler for DMA
 */
void DMA1_Channel1_IRQHandler(void)                             /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    /*HAL library functions that attend interrupt on DMA*/
    HAL_DMA_IRQHandler( &DmaHandler );
}


/**
 *@brief   Interrupt handler for DMA
 */
void ADC1_IRQHandler(void)                                      /* cppcheck-suppress misra-c2012-8.4 ; Function provided by ST in HAL */
{
    HAL_ADC_IRQHandler( &AdcHandler );
}