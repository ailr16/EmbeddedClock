/**------------------------------------------------------------------------------------------------
 * Archivo con la funciones de las incilaizaciones auxiliares de la libreria
-------------------------------------------------------------------------------------------------*/
#include "app_bsp.h"

void HAL_MspInit( void )
{
    
}

void HAL_ADC_MspInit( ADC_HandleTypeDef *hadc )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __ADC_CLK_ENABLE();
    __GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin  = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
}