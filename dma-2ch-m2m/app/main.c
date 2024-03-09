#include "app_bsp.h"
#include <stdio.h>

extern void initialise_monitor_handles(void);

#define BUFFER1_SIZE 32
#define BUFFER2_SIZE 8

/* Handler for DMA1 channel 1 */
DMA_HandleTypeDef DMA1HandlerCh1;

/* Handler for DMA1 channel 2 */
DMA_HandleTypeDef DMA1HandlerCh2;

/* Buffer used as destination of DMA1 Channel 1 with 32 32-bit elements*/
static uint32_t Buffer1[BUFFER1_SIZE];

/* Buffer used as source of DMA1 Channel 1 with 32 32-bit elements*/
static const uint32_t DataSource1[BUFFER1_SIZE] =
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

/* Buffer used as destination for DMA1 Channel 2 with 8 8-bit elements*/
static uint8_t Buffer2[BUFFER2_SIZE];

/* Buffer used as source of DMA1 Channel 2 with 8 8-bit elements*/
static const uint8_t DataSource2[BUFFER2_SIZE] =
{
  0x11, 0x22, 0x33, 0x44,
  0x55, 0x66, 0x77, 0x88
};

int main( void )
{
    /* Initialize HAL */
    HAL_Init( );

    /* Initialize semihosting */
    initialise_monitor_handles();

    /* Enable DMA 1 clock */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* Configure DMA 1 Channel 1 */
    DMA1HandlerCh1.Instance                 = DMA1_Channel1;         /* DMA1 Channel 1                                  */
    DMA1HandlerCh1.Init.Request             = DMA_REQUEST_MEM2MEM;   /* Memory to memory transfer request               */
    DMA1HandlerCh1.Init.Direction           = DMA_MEMORY_TO_MEMORY;  /* Data will be transferred from memory to memory  */
    DMA1HandlerCh1.Init.PeriphInc           = DMA_PINC_ENABLE;       /* Increment source address is enabled             */
    DMA1HandlerCh1.Init.MemInc              = DMA_MINC_ENABLE;       /* Increment destination address is enabled        */
    DMA1HandlerCh1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;   /* Source data is 32-bit width                     */
    DMA1HandlerCh1.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;   /* Destination data is 32-bit width                */
    DMA1HandlerCh1.Init.Mode                = DMA_NORMAL;            /* After finishing last transfer, stop DMA channel */
    DMA1HandlerCh1.Init.Priority            = DMA_PRIORITY_LOW;      /* This DMA channel request config as low priority */
    HAL_DMA_Init( &DMA1HandlerCh1 );

    /* Configure DMA 1 Channel 2 */
    DMA1HandlerCh2.Instance                 = DMA1_Channel2;         /* DMA1 Channel 2                                  */
    DMA1HandlerCh2.Init.Request             = DMA_REQUEST_MEM2MEM;   /* Memory to memory transfer request               */
    DMA1HandlerCh2.Init.Direction           = DMA_MEMORY_TO_MEMORY;  /* Data will be transferred from memory to memory  */
    DMA1HandlerCh2.Init.PeriphInc           = DMA_PINC_ENABLE;       /* Increment source address is enabled             */
    DMA1HandlerCh2.Init.MemInc              = DMA_MINC_ENABLE;       /* Increment destination address is enabled        */
    DMA1HandlerCh2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;   /* Source data is 8-bit width                      */
    DMA1HandlerCh2.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;   /* Destination data is 8-bit width                 */
    DMA1HandlerCh2.Init.Mode                = DMA_NORMAL;            /* After finishing last transfer, stop DMA channel */
    DMA1HandlerCh2.Init.Priority            = DMA_PRIORITY_LOW;      /* This DMA channel request config as low priority */
    HAL_DMA_Init( &DMA1HandlerCh2 );
    
    /* Start DMA1 Channel 1 transfer all elements from Source1 array to destination Buffer1 */
    HAL_DMA_Start( &DMA1HandlerCh1, (uint32_t)&DataSource1, (uint32_t)&Buffer1, BUFFER1_SIZE );

    /* Start DMA1 Channel 2 transfer all elements from Source2 array to destination Buffer2 */
    HAL_DMA_Start( &DMA1HandlerCh2, (uint32_t)&DataSource2, (uint32_t)&Buffer2, BUFFER2_SIZE );

    /* Wait for completion */
    HAL_Delay( 500u );

    /* Print Buffer1 elements (transferred using DMA1 Channel 1) */
    for( uint32_t i = 0; i < ( BUFFER1_SIZE / 4u ); i++ )
    {
        printf("%lx %lx %lx %lx\n", Buffer1[i*4], Buffer1[i*4 + 1], Buffer1[i*4 + 2], Buffer1[i*4 + 3] );
    }

    /* Print Buffer2 elements (transferred using DMA1 Channel 2) */
    for( uint32_t i = 0; i < BUFFER2_SIZE; i++ )
    {
        printf("%x\n", Buffer2[i]);
    }

    return 0u;
}


/* 
    Expected output:
    1020304 5060708 90a0b0c d0e0f10
    11121314 15161718 191a1b1c 1d1e1f20
    21222324 25262728 292a2b2c 2d2e2f30
    31323334 35363738 393a3b3c 3d3e3f40
    41424344 45464748 494a4b4c 4d4e4f50
    51525354 55565758 595a5b5c 5d5e5f60
    61626364 65666768 696a6b6c 6d6e6f70
    71727374 75767778 797a7b7c 7d7e7f80
    11
    22
    33
    44
    55
    66
    77
    88
*/
