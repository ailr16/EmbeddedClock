/**
 * @file    hel_lcd.c
 * @brief   ****
 *
 * LCD driver source
 * 
 */

#include "hel_lcd.h"


/**
 * @brief   Use once to Initialize the display
 * @param   hlcd LCD Handler
 * @retval  1 for successful operation, 0 otherwise
 */
uint8_t HEL_LCD_Init( LCD_HandleTypeDef *hlcd )
{
    uint32_t returnState = 0;

    assert_error( (hlcd->BklPin  > GPIO_PIN_0) && (hlcd->BklPin  < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPin   > GPIO_PIN_0) && (hlcd->CsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPin   > GPIO_PIN_0) && (hlcd->RsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->BklPort > GPIOA)      && (hlcd->BklPort < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPort  > GPIOA)      && (hlcd->CsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPort  > GPIOA)      && (hlcd->RsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->SpiHandler), LCD_PAR_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    HEL_LCD_MspInit( hlcd );            //Init LCD pins

    HAL_GPIO_WritePin( hlcd->BklPort, hlcd->BklPin, GPIO_PIN_SET );     //Enable backlight

    HAL_GPIO_WritePin( hlcd->CsPort,  hlcd->CsPin,  GPIO_PIN_SET );
    HAL_GPIO_WritePin( hlcd->RstPort, hlcd->RstPin, GPIO_PIN_RESET );

    HAL_Delay( 2 );

    HAL_GPIO_WritePin( hlcd->RstPort, hlcd->RstPin, GPIO_PIN_SET );

    HAL_Delay( 20 );

    returnState &= HEL_LCD_Command( hlcd, 0x30 );

    HAL_Delay( 2 );

    returnState |= HEL_LCD_Command( hlcd, 0x30 );
    returnState |= HEL_LCD_Command( hlcd, 0x30 );
    returnState |= HEL_LCD_Command( hlcd, 0x39 );    //8 bit, 1 line, normal font, extension instruction
    returnState |= HEL_LCD_Command( hlcd, 0x14 );    //bias 1/5, 183hz
    returnState |= HEL_LCD_Command( hlcd, 0x56 );    //booster ON, contrast+8
    returnState |= HEL_LCD_Command( hlcd, 0x6D );    //follower ON, rab2, rab0

    HAL_Delay( 200 );

    returnState |= HEL_LCD_Command( hlcd, 0x70 );    //contrast
    returnState |= HEL_LCD_Command( hlcd, 0x0C );    //display on
    returnState |= HEL_LCD_Command( hlcd, 0x06 );    //display on
    returnState |= HEL_LCD_Command( hlcd, 0x01 );    //display on

    HAL_Delay( 1 );
    
    return returnState;
}


/**
 * @brief   Display pin Initializaiton
 * @param   hlcd LCD Handler
 */
/* cppcheck-suppress misra-c2012-8.7 ; This function could be implemented outside this file */
__weak void HEL_LCD_MspInit( LCD_HandleTypeDef *hlcd )
{
    GPIO_InitTypeDef GPIO_Init_Struct;

    __HAL_RCC_GPIOB_CLK_ENABLE( );
    __HAL_RCC_GPIOD_CLK_ENABLE( );

    //PD02, Reset       PD03 CS         PD04 RS command
    GPIO_Init_Struct.Pin   = hlcd->RstPin | hlcd->CsPin | hlcd-> RsPin;
    GPIO_Init_Struct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_Init_Struct.Pull  = GPIO_NOPULL;
    GPIO_Init_Struct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init( hlcd->RstPort, &GPIO_Init_Struct );

    //PB04 Backligth
    GPIO_Init_Struct.Pin   = hlcd->BklPin;
    GPIO_Init_Struct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_Init_Struct.Pull  = GPIO_NOPULL;
    GPIO_Init_Struct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init( hlcd->BklPort, &GPIO_Init_Struct );
}


/**
 * @brief   Send command to display
 * @param   hlcd LCD Handler
 * @param   cmd  8-bit Command
 * @retval  1 for successful transmission, 0 otherwise
 */
uint8_t HEL_LCD_Command( LCD_HandleTypeDef *hlcd, uint8_t cmd )
{
    uint8_t transmitStatus;
    uint8_t TxBuffer[1];

    assert_error( (hlcd->BklPin  > GPIO_PIN_0) && (hlcd->BklPin  < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPin   > GPIO_PIN_0) && (hlcd->CsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPin   > GPIO_PIN_0) && (hlcd->RsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->BklPort > GPIOA)      && (hlcd->BklPort < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPort  > GPIOA)      && (hlcd->CsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPort  > GPIOA)      && (hlcd->RsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->SpiHandler), LCD_PAR_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    TxBuffer[0] = cmd;

    HAL_GPIO_WritePin( hlcd->RsPort, hlcd->RsPin, GPIO_PIN_RESET );               //RS = 0
    HAL_GPIO_WritePin( hlcd->CsPort, hlcd->CsPin, GPIO_PIN_RESET );               //CS = 0

    transmitStatus = HAL_SPI_Transmit( hlcd->SpiHandler, TxBuffer, 1, 500 );

    HAL_GPIO_WritePin( hlcd->CsPort, hlcd->CsPin, GPIO_PIN_SET );                 //CS = 1
    
    return transmitStatus;
}


/**
 * @brief   Send data (character) to display
 * @param   hlcd LCD Handler
 * @param   data  8-bit data
 * @retval  1 for successful transmission, 0 otherwise
 */
/* cppcheck-suppress misra-c2012-8.7 ; This function could be implemented outside this file */
uint8_t HEL_LCD_Data( LCD_HandleTypeDef *hlcd, uint8_t data )
{
    uint8_t transmitStatus;
    uint8_t TxBuffer[1];

    assert_error( (hlcd->BklPin  > GPIO_PIN_0) && (hlcd->BklPin  < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPin   > GPIO_PIN_0) && (hlcd->CsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPin   > GPIO_PIN_0) && (hlcd->RsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->BklPort > GPIOA)      && (hlcd->BklPort < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPort  > GPIOA)      && (hlcd->CsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPort  > GPIOA)      && (hlcd->RsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->SpiHandler), LCD_PAR_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    TxBuffer[0] = data;

    HAL_GPIO_WritePin( hlcd->RsPort, hlcd->RsPin, GPIO_PIN_SET );
    HAL_GPIO_WritePin( hlcd->CsPort, hlcd->CsPin, GPIO_PIN_RESET );

    transmitStatus = HAL_SPI_Transmit( hlcd->SpiHandler, TxBuffer, 1, 500 );
    
    HAL_GPIO_WritePin( hlcd->CsPort, hlcd->CsPin, GPIO_PIN_SET );                 //CS = 1

    return transmitStatus;
}


/**
 * @brief   Send a string to display
 * @param   hlcd LCD Handler
 * @param   str  8-bit String
 * @retval  1 for successful transmission, 0 otherwise
 */
uint8_t HEL_LCD_String( LCD_HandleTypeDef *hlcd, char *str )
{
    uint8_t transmitStatus = 0;
    uint8_t i = 0;
    char actualChar;

    assert_error( (hlcd->BklPin  > GPIO_PIN_0) && (hlcd->BklPin  < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPin   > GPIO_PIN_0) && (hlcd->CsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPin   > GPIO_PIN_0) && (hlcd->RsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->BklPort > GPIOA)      && (hlcd->BklPort < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPort  > GPIOA)      && (hlcd->CsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPort  > GPIOA)      && (hlcd->RsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->SpiHandler), LCD_PAR_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    actualChar = str[0];
    while(actualChar != '\0')
    {
        transmitStatus |= HEL_LCD_Data( hlcd, actualChar );
        i++;
        actualChar = str[i];
    }

    return transmitStatus;
}


/**
 * @brief   Set cursor (row, col) of display
 * @param   hlcd LCD Handler
 * @param   row  Row value, 0 or 1
 * @param   col  Col value, 0 to 15
 * @retval  1 for successful transmission, 0 otherwise
 */
uint8_t HEL_LCD_SetCursor( LCD_HandleTypeDef *hlcd, uint8_t row, uint8_t col )
{
    uint8_t transmitStatus = 0;
    uint8_t command;
    
    assert_error( (hlcd->BklPin  > GPIO_PIN_0) && (hlcd->BklPin  < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPin   > GPIO_PIN_0) && (hlcd->CsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPin   > GPIO_PIN_0) && (hlcd->RsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->BklPort > GPIOA)      && (hlcd->BklPort < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPort  > GPIOA)      && (hlcd->CsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPort  > GPIOA)      && (hlcd->RsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->SpiHandler), LCD_PAR_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( (row > 1U) || (col > 15U) )
    {
        transmitStatus = 1U;
    }
    else
    {
        command = 0x80U | ( (row * 40U) + col);
        transmitStatus = HEL_LCD_Command( hlcd, command );
    }

    return transmitStatus;
}


/**
 * @brief   Turn on, off or toggle lcd backlight
 * @param   hlcd LCD Handler
 * @param   state LCD_OFF, LCD_ON or LCD_TOGGLE
 */
void HEL_LCD_Backlight( LCD_HandleTypeDef *hlcd, uint8_t state )
{
    assert_error( (hlcd->BklPin  > GPIO_PIN_0) && (hlcd->BklPin  < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPin   > GPIO_PIN_0) && (hlcd->CsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPin   > GPIO_PIN_0) && (hlcd->RsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->BklPort > GPIOA)      && (hlcd->BklPort < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPort  > GPIOA)      && (hlcd->CsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPort  > GPIOA)      && (hlcd->RsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->SpiHandler), LCD_PAR_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    switch(state)
    {
        case LCD_OFF:
            HAL_GPIO_WritePin( hlcd->BklPort, hlcd->BklPin, GPIO_PIN_SET );
        break;

        case LCD_ON:
            HAL_GPIO_WritePin( hlcd->BklPort, hlcd->BklPin, GPIO_PIN_RESET );
        break;

        case LCD_TOGGLE:
            HAL_GPIO_TogglePin( hlcd->BklPort, hlcd->BklPin );
        break;

        default:
        break;
    }
}


/**
 * @brief   Adjust LCD contrast
 * @param   hlcd LCD Handler
 * @param   contrast 6-bit Contrast value, 0 to 63
 * @retval  1 for successful transmission, 0 otherwise
 */
uint8_t HEL_LCD_Contrast( LCD_HandleTypeDef *hlcd, uint8_t contrast )
{
    uint8_t returnStatus = 0;
    uint8_t contrastHighByte;
    uint8_t contrastLowByte;

    assert_error( (hlcd->BklPin  > GPIO_PIN_0) && (hlcd->BklPin  < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPin   > GPIO_PIN_0) && (hlcd->CsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPin   > GPIO_PIN_0) && (hlcd->RsPin   < GPIO_PIN_15),  LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->BklPort > GPIOA)      && (hlcd->BklPort < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->CsPort  > GPIOA)      && (hlcd->CsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->RsPort  > GPIOA)      && (hlcd->RsPort  < GPIOF),        LCD_PAR_ERROR );                   /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( (hlcd->SpiHandler), LCD_PAR_ERROR );                /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( contrast > 63U )
    {
        returnStatus = 1;
    }

    else
    {
        contrastHighByte = 0x50U | 0x04U | ( (contrast & 0x30U) >> 4U );
        contrastLowByte = 0x70U | (contrast & 0x0FU);

        returnStatus |= HEL_LCD_Command( hlcd, contrastHighByte );
        returnStatus |= HEL_LCD_Command( hlcd, contrastLowByte );
    }
    
    return returnStatus;
}