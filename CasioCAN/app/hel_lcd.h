/**
 * @file    hel_lcd.h
 * @brief   ****
 *
 * Interfaces for LCD driver:
 * 
 * uint8_t HEL_LCD_Init( LCD_HandleTypeDef *hlcd ) : Use once to Initialize the lcd
 * 
 * void HEL_LCD_MspInit( LCD_HandleTypeDef *hlcd ) : Use once to initialize port pins of SPI peripheral
 * 
 * uint8_t HEL_LCD_Command( LCD_HandleTypeDef *hlcd, uint8_t cmd ) : Send command to LCD
 * 
 * uint8_t HEL_LCD_Data( LCD_HandleTypeDef *hlcd, uint8_t data ) : Send data/character to LCD
 * 
 * uint8_t HEL_LCD_String( LCD_HandleTypeDef *hlcd, char *str ) : Send char string to LCD
 * 
 * uint8_t HEL_LCD_SetCursor( LCD_HandleTypeDef *hlcd, uint8_t row, uint8_t col ) : Set cursor in LCD
 * 
 * void HEL_LCD_Backlight( LCD_HandleTypeDef *hlcd, uint8_t state ) : Turn off, on or toggle LCD backlight
 * 
 * uint8_t HEL_LCD_Contrast( LCD_HandleTypeDef *hlcd, uint8_t contrast ) : Set LCD contrast
 * 
 */

#ifndef HEL_LCD_H
#define HEL_LCD_H

    /** 
    * @defgroup <LCDBacklightStates> Define states of LCD backlight
    @{ */
    
    #define     LCD_OFF     0           /*!< Backlight off          */
    #define     LCD_ON      1           /*!< Backlight on           */
    #define     LCD_TOGGLE  2           /*!< Backlight toogle state */

    /**
    @} */

    #include "app_bsp.h"


    /**
    * @brief   Struct for LCD handling
    */
    typedef struct
    {
        SPI_HandleTypeDef   *SpiHandler;    /**< SPI handler address of the spi to use with the LCD     */
        GPIO_TypeDef        *RstPort;       /**< Port where the pin to control the LCD reset pin is     */
        uint32_t            RstPin;         /**< Pin to control the LCD reset pin                       */
        GPIO_TypeDef        *RsPort;        /**< Port where the pin to control the LCD RS pin           */
        uint32_t            RsPin;          /**< Pin to control the LCD RS pin                          */
        GPIO_TypeDef        *CsPort;        /**< Port where the pin to control the LCD chip select is   */
        uint32_t            CsPin;          /**< Pin to control the LCD chip select pin                 */
        GPIO_TypeDef        *BklPort;       /**< Port where the pin to control the LCD backlight is     */
        uint32_t            BklPin;         /**< Pin to control the LCD backlight pin                   */
    } LCD_HandleTypeDef;


    /**
     * @brief   Use once to Initialize the display
     */
    uint8_t HEL_LCD_Init( LCD_HandleTypeDef *hlcd );


    /**
     * @brief   Display pin Initializaiton
     */
    __weak void HEL_LCD_MspInit( LCD_HandleTypeDef *hlcd );


    /**
     * @brief   Send command to display
     */
    uint8_t HEL_LCD_Command( LCD_HandleTypeDef *hlcd, uint8_t cmd );


    /**
     * @brief   Send data (character) to display
     */
    uint8_t HEL_LCD_Data( LCD_HandleTypeDef *hlcd, uint8_t data );


    /**
     * @brief   Send a string to display
     */
    uint8_t HEL_LCD_String( LCD_HandleTypeDef *hlcd, char *str );


    /**
     * @brief   Set cursor (row, col) of display
     */
    uint8_t HEL_LCD_SetCursor( LCD_HandleTypeDef *hlcd, uint8_t row, uint8_t col );


    /**
     * @brief   Turn on, off or toggle lcd backlight
     */
    void HEL_LCD_Backlight( LCD_HandleTypeDef *hlcd, uint8_t state );


    /**
     * @brief   Adjust LCD contrast
     */
    uint8_t HEL_LCD_Contrast( LCD_HandleTypeDef *hlcd, uint8_t contrast );

#endif