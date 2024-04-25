/**
 * @file    app_bsp.h
 * @brief   ****
 *
 * Including required libraries and definition of Structs
 *
 */

/*macro to detect erros, wehere if expr is evaluated to false is an error*/
#ifndef BSP_H
#define BSP_H

    #include "stm32g0xx.h"
    #include "hel_lcd.h"
    #include "hil_queue.h"
    #include "hil_scheduler.h"
    #include <stdint.h>
    #include <stdio.h>                  /* cppcheck-suppress misra-c2012-21.6 ; Library used for testing */
    #include <string.h>


    /**
    *@brief   Macro to evaluate error
    */
    #define assert_error(expr, error)         ((expr) ? (void)0U : safe_state((uint8_t *)__FILE__, __LINE__, (error)))


    /**
    *@brief   external link to function that will act as the safe state
    */
    extern void safe_state(uint8_t *file, uint32_t line, uint8_t error);


    /**
     * @brief   Enum for identifying error type
     */
    typedef enum _App_ErrorsCode            
    {                                                                       
        WWDG_RET_ERROR = 1U,        /*!< WDG ERROR        1 */
        PWR_RET_ERROR,              /*!< PWR ERROR        2 */
        RCC_RET_ERROR,              /*!< RCC ERROR        3 */
        HARDFAULT_RET_ERROR,        /*!< HFAULT ERROR     4 */
        ECC_RET_ERROR,              /*!< ECC ERROR        5 */
        CAN_RET_ERROR,              /*!< CAN ERROR        6 */
        RTC_RET_ERROR,              /*!< RTC ERROR        7 */
        SPI_RET_ERROR,              /*!< SPI ERROR        8 */
        LCD_RET_ERROR,              /*!< LCD ERROR        9 */
        HAL_RET_ERROR,              /*!< HAL ERROR        10 */
        CAN_FUNC_ERROR,             /*!< CAN F ERROR      11 */
        HARDFAULT_FUNC_ERROR,       /*!< HFAULT ERROR     12 */
        LCD_PAR_ERROR,              /*!< LCD P ERROR      13 */
        SPI_FUNC_ERROR,             /*!< SPI F ERROR      14 */
        WWDG_FUNC_ERROR,            /*!< WDG F ERROR      15 */
        ECC_FUNC_ERROR,             /*!< ECC F ERROR      16 */
        QUEUE_PAR_ERROR,            /*!< QUEUE P ERROR    17 */
        QUEUE_RET_ERROR,            /*!< QUEUE ERROR      18 */
        SCHEDULER_PAR_ERROR,        /*!< SCH P ERROR      19 */
        SCHEDULER_TIM_ERROR,        /*!< SCH T ERROR      20 */
        ADC_DEVIATION_ERROR         /*!< SCH T ERROR      21 */
    } App_ErrorsCode;                                                       


    /**
     * @brief   Struct for time data (time, date and alarm)
     */
    typedef struct _APP_TmTypeDef 
    {
        uint32_t tm_sec;         /**< seconds,  range 0 to 59          */
        uint32_t tm_min;         /**< minutes, range 0 to 59           */
        uint32_t tm_hour;        /**< hours, range 0 to 23             */
        uint32_t tm_mday;        /**< day of the month, range 1 to 31  */
        uint32_t tm_mon;         /**< month, range 0 to 11             */
        uint32_t tm_year;        /**< years in rage 1900 2100          */
        uint32_t tm_wday;        /**< day of the week, range 0 to 6    */
        uint32_t tm_yday;        /**< day in the year, range 0 to 365  */
        uint32_t tm_isdst;       /**< daylight saving time             */
        uint32_t tm_hour_a;      /**< alarm hour,  range 0 to 23       */
        uint32_t tm_min_a;       /**< alarm minutes,  range 0 to 59    */
    }APP_TmTypeDef;


    /**
     * @brief   Struct for store message data
     */
    typedef struct _NEW_MsgTypeDef
    {
        uint8_t  data[8];        /**< Array of uint8t for data          */
    }NEW_MsgTypeDef;


    /**
     * @brief   Struct for store message type and time data
     */
    typedef struct _APP_MsgTypeDef
    {
        uint8_t msg;            /**< message type, allowed values 0 to 4    */
        APP_TmTypeDef tm;       /**< Struct for store time data             */
    }APP_MsgTypeDef;


    /**
     * @brief   Enum for identifying received mesage type
     */
    typedef enum
    {
        SERIAL_MSG_NONE  = 0U,  /**< Value for None message type            */
        SERIAL_MSG_TIME  = 1U,  /**< Value for Time message type            */
        SERIAL_MSG_DATE  = 2U,  /**< Value for Date message type            */
        SERIAL_MSG_ALARM = 3U,  /**< Value for Alarm message type           */
        SERIAL_MSG_ERROR = 4U,  /**< Value for serial error message type    */
        SERIAL_MSG_OK    = 5U,  /**< Value for serial ok message type       */
        CLOCK_MSG_PRINT  = 6U,  /**< Value for clock print message type     */
        CLOCK_SET_ALARM  = 7U,  /**< Value for set alarm in clock           */
        CLOCK_DIS_ALARM  = 8U,  /**< Value for disable alarm in clock       */
        CLOCK_PRINT_ALARM= 9U,  /**< Value for print setted alarm           */
        DISPLAY_MSG_ALARM= 10U, /**< Value for print alarm trigger indisplay*/
        DISPLAY_MSG_PRINT= 11U, /**< Value for print normal date and time   */
        DISPLAY_MSG_DISAL= 12U, /**< Value for set display in no alarm mode */
        DISPLAY_MSG_SETAL= 13U, /**< Value for set display in alarm mode    */
        DISPLAY_MSG_PRAL = 14U, /**< Value for print alarm status (button)  */
    }APP_Messages;

    /**
     * @brief   Structures used in different files
     */
    extern ADC_HandleTypeDef   AdcHandler;                        /* cppcheck-suppress misra-c2012-8.5 ; Struct declared in  app_display.c*/
    extern DMA_HandleTypeDef   DmaHandler;                        /* cppcheck-suppress misra-c2012-8.5 ; Struct declared in  app_display.c*/
    extern WWDG_HandleTypeDef  WwdgHandler;                       /* cppcheck-suppress misra-c2012-8.5 ; Struct declared in main.c */

#endif

