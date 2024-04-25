/**
 * @file    app_serial.h
 * @brief   ****
 *
 * Interfaces for app_serial driver:
 * 
 * void Serial_Init( void ) : Use once to Initialize the app_serial. Don't requires any parameter
 * 
 * void Serial_Task( void ) : Run continuosuly. It allows to receive messages through CAN
 *
 */


#ifndef APP_SERIAL_H
#define APP_SERIAL_H


    #include "app_bsp.h"


    /**
     * @brief   Use once to Initialize the app_serial
     */
    void Serial_Init( void );


    /**
     * @brief   Run continuosuly to receive messages trough CAN and decode data
     */
    void Serial_Task( void );


#endif