/**
 * @file    app_analogs.h
 * @brief   ****
 *
 * Interfaces for LCD adjustments:
 * 
 * void Analogs_Init( void )                   Initialize ADC, DMA and required TIMER for triggering
 * 
 * uint8_t Analogs_GetIntensity( void )        Get the intensity value readed from POT0
 *
 * uint8_t Analogs_GetContrast( void )         Get the contrast value readed from POT1
 * 
 * int8_t Analogs_GetTemperature( void )       Get the temperature value readed from internal sensor
 * 
 */

#ifndef APP_ANALOGS_H
#define APP_ANALOGS_H


    #include "app_bsp.h"


    void Analogs_Init( void );


    uint8_t Analogs_GetIntensity( void );


    uint8_t Analogs_GetContrast( void );


    int8_t Analogs_GetTemperature( void );


#endif