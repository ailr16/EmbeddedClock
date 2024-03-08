#include "app_bsp.h"
#include <stdio.h>

extern void initialise_monitor_handles(void);

#define BUFFER1_SIZE 1

uint32_t buffer1[ BUFFER1_SIZE ];
uint32_t conversionComplete;

/* Handler for DMA1 channel 1 */
DMA_HandleTypeDef Dma1HandlerCh1;

/* Handler for ADC1 */
ADC_HandleTypeDef      AdcHandler;

/* Handler for ADC1 Channel 0*/
ADC_ChannelConfTypeDef AdcChannelConfig;

int main( void )
{
    /* Initialize HAL */
    HAL_Init( );

    /* Initialize semihosting */
    initialise_monitor_handles();

    /* Enable DMA 1 clock */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* Configure DMA 1 Channel 1 */
    Dma1HandlerCh1.Instance                 = DMA1_Channel1;         /* DMA1 Channel 1                                     */
    Dma1HandlerCh1.Init.Request             = DMA_REQUEST_ADC1;      /* Peripheral ADC1 request                            */
    Dma1HandlerCh1.Init.Direction           = DMA_PERIPH_TO_MEMORY;  /* Data will be transferred from peripheral to memory */
    Dma1HandlerCh1.Init.PeriphInc           = DMA_PINC_DISABLE;      /* Increment source address is disabled               */
    Dma1HandlerCh1.Init.MemInc              = DMA_MINC_ENABLE;       /* Increment destination address is enabled           */
    Dma1HandlerCh1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;   /* Source data is 32-bit width                        */
    Dma1HandlerCh1.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;   /* Destination data is 32-bit width                   */
    Dma1HandlerCh1.Init.Mode                = DMA_CIRCULAR;          /* After finishing last transfer, go to first element */
    Dma1HandlerCh1.Init.Priority            = DMA_PRIORITY_LOW;      /* This DMA channel request config as low priority    */
    HAL_DMA_Init( &Dma1HandlerCh1 );

    /* Enable DMA interrupts */
    HAL_NVIC_SetPriority( DMA1_Channel1_IRQn, 2, 0 );
    HAL_NVIC_EnableIRQ( DMA1_Channel1_IRQn );

    /* Configure ADC */
    __HAL_LINKDMA( &AdcHandler, DMA_Handle, Dma1HandlerCh1 );           /* Link DMA Handler to ADC Handler*/
    AdcHandler.Instance                   = ADC1;                       /* Use ADC1 Peripheral */
    AdcHandler.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;   /* APB clock divided by two*/
    AdcHandler.Init.Resolution            = ADC_RESOLUTION8b;           /* 8 bit resolution with a Tconv of 8.5*/
    AdcHandler.Init.ScanConvMode          = ADC_SCAN_SEQ_FIXED;         /* Scan adc channels from 0 to 16 in that order*/
    AdcHandler.Init.DataAlign             = ADC_DATAALIGN_RIGHT;        /* data converter is right alightned*/
    AdcHandler.Init.SamplingTimeCommon1   = ADC_SAMPLETIME_1CYCLE_5;    /* sampling time of 1.5*/
    AdcHandler.Init.ExternalTrigConv      = ADC_SOFTWARE_START;         /* software trigger*/
    AdcHandler.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;        /* only applicable on ISR*/
    AdcHandler.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;   /* data will be overwriten in case is not read it*/
    AdcHandler.Init.DMAContinuousRequests = ENABLE;
    HAL_ADC_Init( &AdcHandler );

    AdcChannelConfig.Channel      = ADC_CHANNEL_0;
    AdcChannelConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;
    AdcChannelConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1; 
    HAL_ADC_ConfigChannel( &AdcHandler, &AdcChannelConfig );

    HAL_ADCEx_Calibration_Start( &AdcHandler );
    
    /* Start DMA1 Channel 1 transfer all elements from Source1 array to destination Buffer1 */
    HAL_ADC_Start_DMA( &AdcHandler, (uint32_t*)buffer1, BUFFER1_SIZE );

    while (1)
    {
        //HAL_ADC_Start( &AdcHandler );                /*trigger conversion*/
        //HAL_ADC_PollForConversion( &AdcHandler, 1u );/*wait untill conversion is performed, around 1.25us*/
        //value = HAL_ADC_GetValue( &AdcHandler );     /*read the digital value*/

        if( conversionComplete )
        {
            printf( "Pot value: %lu\r\n", buffer1[ 0u ] );      /*print the adc value*/
            conversionComplete = 0;
            HAL_ADC_Start_DMA( &AdcHandler, (uint32_t*)buffer1, BUFFER1_SIZE );
        }
    }

    return 0u;
}

void HAL_ADC_ConvCpltCallback( ADC_HandleTypeDef *hadc )
{
    conversionComplete = 1u;
}