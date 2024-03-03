#include "app_bsp.h"

extern void initialise_monitor_handles(void);


DMA_HandleTypeDef DMAHandler = { 0 };

#define BUFFER_SIZE 32 

static uint32_t Buffer[BUFFER_SIZE];
static const uint32_t DataSource[BUFFER_SIZE] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80
};

int main( void )
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    HAL_Init( );
    initialise_monitor_handles();

    __HAL_RCC_GPIOC_CLK_ENABLE( );

    GPIO_InitStruct.Pin   = GPIO_PIN_0;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );

    __HAL_RCC_DMA1_CLK_ENABLE();

    DMAHandler.Instance                 = DMA1_Channel1;
    DMAHandler.Init.Request             = DMA_REQUEST_MEM2MEM;
    DMAHandler.Init.Direction           = DMA_MEMORY_TO_MEMORY;
    DMAHandler.Init.PeriphInc           = DMA_PINC_ENABLE;
    DMAHandler.Init.MemInc              = DMA_MINC_ENABLE;
    DMAHandler.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    DMAHandler.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    DMAHandler.Init.Mode                = DMA_NORMAL;
    DMAHandler.Init.Priority            = DMA_PRIORITY_LOW;
    HAL_DMA_Init( &DMAHandler );
    
    HAL_DMA_Start( &DMAHandler, (uint32_t)&DataSource, (uint32_t)&Buffer, BUFFER_SIZE );
    HAL_Delay( 500u );

    for( uint32_t i = 0; i < BUFFER_SIZE; i++ )
    {
        printf("%x\n", Buffer[i]);
    }

    for( ; ; )
    {
        HAL_GPIO_WritePin( GPIOC, GPIO_PIN_0, SET);
        HAL_Delay( 200u );
        HAL_GPIO_WritePin( GPIOC, GPIO_PIN_0, RESET);
        HAL_Delay( 100u );
    }

    return 0u;
}

