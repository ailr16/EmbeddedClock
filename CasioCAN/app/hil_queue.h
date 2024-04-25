/**
 * @file    hil_queue.h
 * @brief   ****
 *
 * Interfaces for queue handling driver:
 * 
 * void HIL_QUEUE_Init( QUEUE_HandleTypeDef *hqueue )
 * 
 * uint8_t HIL_QUEUE_Write( QUEUE_HandleTypeDef *hqueue, void *data )
 *
 * uint8_t HIL_QUEUE_Read( QUEUE_HandleTypeDef *hqueue, void *data )
 * 
 * uint8_t HIL_QUEUE_IsEmpty( QUEUE_HandleTypeDef *hqueue )
 * 
 * void HIL_QUEUE_Flush( QUEUE_HandleTypeDef *hqueue )
 * 
 * uint8_t HIL_QUEUE_WriteISR( QUEUE_HandleTypeDef *hqueue, void *data, uint8_t isr )
 * 
 * uint8_t HIL_QUEUE_ReadISR( QUEUE_HandleTypeDef *hqueue, void *data, uint8_t isr )
 * 
 * uint8_t HIL_QUEUE_IsEmptyISR( QUEUE_HandleTypeDef *hqueue, uint8_t isr )
 * 
 * void HIL_QUEUE_FlushISR( QUEUE_HandleTypeDef *hqueue, uint8_t isr )
 */

#ifndef HIL_QUEUE_H
#define HIL_QUEUE_H

    #include "app_bsp.h"

    /** 
    * @defgroup <QUEUEStates> Define states for queue
        @{ */

    #define     QUEUE_OK            1           /*!< Successful operation */
    #define     QUEUE_NOT_OK        0           /*!< Fault operation*/

    /**
        @} */


    /** 
    * @defgroup <HandlingInterrupts> Define values used for disabling interrupts in queuing
        @{ */

    #define     DISABLE_ALL_INTERRUPTS      0xFF               /*!< Disable all IRQn */

    /**
        @} */


    /**
     * @brief   Struct for queue handling
     */
    typedef struct
    {
        void        *Buffer;        /**< pointer to reserved memory for buffer          */
        uint32_t    Elements;       /**< elements to store / queue size                 */
        uint8_t     Size;           /**< elements size                                  */
        uint32_t    Head;           /**< pointer to the next element to write           */
        uint32_t    Tail;           /**< pointer to the next element to read            */
        uint8_t     Empty;          /**< empty queue flag                               */
        uint8_t     Full;           /**< full queue flag                                */
        uint32_t    SavedElements;  /**< actual saved elements                          */
        //agregar más elementos si se requieren
    } QUEUE_HandleTypeDef;


    /**
     * @brief   Initializes the queue by setting the head and tail elements to zero, and the values of empty to one and full to zero
     */
    void HIL_QUEUE_Init( QUEUE_HandleTypeDef *hqueue );


    /**
     * @brief   Copies the information referenced by the empty pointer data to the buffer controlled by hqueue
     */
    uint8_t HIL_QUEUE_Write( QUEUE_HandleTypeDef *hqueue, void *data );


    /**
     * @brief   Reads data from the buffer controlled by hqueue, the data is copied into the data type referenced by the empty pointer data
     */
    uint8_t HIL_QUEUE_Read( QUEUE_HandleTypeDef *hqueue, void *data );


    /**
     * @brief   The function returns a one if there are no more elements that can be read from the queue and zero if there is at least one element that can be read.
     */
    uint8_t HIL_QUEUE_IsEmpty( QUEUE_HandleTypeDef *hqueue );

    /**
     * @brief   The function must empty the queue in case it has elements inside it, the information will be discarded
     */
    void HIL_QUEUE_Flush( QUEUE_HandleTypeDef *hqueue );


    /**
     * @brief   Copies the information referenced by the empty pointer data to the buffer controlled by hqueue and operates with interrupt
     */
    uint8_t HIL_QUEUE_WriteISR( QUEUE_HandleTypeDef *hqueue, void *data, uint8_t isr );


    /**
     * @brief   Reads data from the buffer controlled by hqueue, the data is copied into the data type referenced by the empty pointer data  and operates with interrupt
     */
    uint8_t HIL_QUEUE_ReadISR( QUEUE_HandleTypeDef *hqueue, void *data, uint8_t isr );


    /**
     * @brief   The function returns a one if there are no more elements that can be read from the queue and zero if there is at least one element that can be read.
     */
    uint8_t HIL_QUEUE_IsEmptyISR( QUEUE_HandleTypeDef *hqueue, uint8_t isr );


    /**
     * @brief   The function must empty the queue in case it has elements inside it, the information will be discarded and operates wih interrupt
     */
    void HIL_QUEUE_FlushISR( QUEUE_HandleTypeDef *hqueue, uint8_t isr );
#endif

