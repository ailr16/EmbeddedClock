/**
 * @file    app_display.h
 * @brief   ****
 *
 * Interfaces for app_display driver:
 * 
 * void Display_Init( void )        Use once in the main file to initialize the display
 * 
 * void Display_Task( void )        Run continuously  to print in the display
 *
 */

#ifndef APP_DISPLAY_H
#define APP_DISPLAY_H


    #include "app_bsp.h"


    /**
     * @brief   Use once to Initialize the app_display
     */
    void Display_Init( void );


    /**
     * @brief   Run continuously. It allows to print data on LCD
     */
    void Display_Task( void );


#endif