/**
 * @file    app_clock.h
 * @brief   ****
 *
 * Interfaces for app_clock driver:
 * 
 * void Clock_Init( void ) : Use once to Initialize the app_clock. Don't requires any parameter
 * 
 * void Clock_Task( void ) : Run continuosuly. It allows to configure the clock 
 *
 */


#ifndef APP_CLOCK_H
#define APP_CLOCK_H

    #include "app_bsp.h"


    /**
     * @brief   Use once to Initialize the app_clock
     */
    void Clock_Init( void );


    /**
     * @brief   Run continuosuly. It allows to configure the clock
     */
    void Clock_Task( void );

#endif