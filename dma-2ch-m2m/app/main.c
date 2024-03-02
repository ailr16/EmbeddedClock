/**
 * @file    main.c
 * @brief   **Template Application entry point**
 *
 * The main file is the entry point of the application or any user code, please provide the 
 * proper description of this file according to your own implementation
 * This Demo app only blinks an LED connected to PortA Pin 5
 *
 * @note    Only the files inside folder app will be take them into account when the 
 *          doxygen runs by typing "make docs", index page is generated in
 *          Build/doxigen/html/index.html
 */
#include "app_bsp.h"

/**
 * @brief   **Application entry point**
 *
 * Ptovide the proper description for function main according to your own
 * implementation
 *
 * @retval  None
 */

DMA_HandleTypeDef DMAHandler;

int main( void )
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    DMA_HandleTypeDef DmaHandler;

    HAL_Init( );

    __HAL_RCC_GPIOC_CLK_ENABLE( );

    GPIO_InitStruct.Pin   = GPIO_PIN_0;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );

    DMAHandler.Instance                 = DMA1_Channel1;
    DmaHandler.Init.Request             = DMA_REQUEST_MEM2MEM;
    DMAHandler.Init.Direction           = DMA_MEMORY_TO_MEMORY;
    DmaHandler.Init.PeriphInc           = DMA_PINC_ENABLE;
    DmaHandler.Init.MemInc              = DMA_MINC_ENABLE;
    DmaHandler.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    DmaHandler.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    DmaHandler.Init.Mode                = DMA_NORMAL;
    DmaHandler.Init.Priority            = DMA_PRIORITY_LOW;

    HAL_DMA_Init( &DmaHandler );


    for( ; ; )
    {
        HAL_GPIO_WritePin( GPIOC, GPIO_PIN_0, SET);
        HAL_Delay( 200u );
        HAL_GPIO_WritePin( GPIOC, GPIO_PIN_0, RESET);
        HAL_Delay( 100u );
    }

    return 0u;
}

