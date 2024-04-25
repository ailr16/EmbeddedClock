/**
 * @file    hil_queue.c
 * @brief   ****
 *
 * queue handling source:
 * 
 * 
 * 
 */

#include "hil_queue.h"


static uint8_t HIL_QUEUE_IsFull( QUEUE_HandleTypeDef *hqueue );


/**
 * @brief   Initializes the queue by setting the head and tail elements to zero, and the values of empty to one and full to zero
 * @param   hqueue Queue Handler
 */
void HIL_QUEUE_Init( QUEUE_HandleTypeDef *hqueue )
{

    assert_error( hqueue->Buffer   != NULL, QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hqueue->Elements != 0U,   QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hqueue->Size     != 0U,   QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    hqueue->Head          = 0U;
    hqueue->Tail          = 0U;
    hqueue->Full          = 0U;
    hqueue->Empty         = 1U;
    hqueue->SavedElements = 0U;
}


/**
 * @brief   Copies the information referenced by the empty pointer data to the buffer controlled by hqueue
 * @param   hqueue Queue Handler
 * @param   data Variable where data will be copied
 */
uint8_t HIL_QUEUE_Write( QUEUE_HandleTypeDef *hqueue, void *data )
{
    uint8_t valueToReturn;

    assert_error( hqueue->Buffer   != NULL, QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hqueue->Elements != 0U,   QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hqueue->Size     != 0U,   QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( data             != NULL, QUEUE_PAR_ERROR );              /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( HIL_QUEUE_IsFull( hqueue ) == 1U ){
        hqueue->Full = 1U;
        valueToReturn = QUEUE_NOT_OK;
    }
    else{
        (void)memcpy(hqueue->Buffer + (hqueue->Size * hqueue->Tail), data, hqueue->Size);                   /* cppcheck-suppress misra-c2012-18.4 ; Writing into memory using pointer */
        hqueue->Tail = (hqueue->Tail + 1U) % hqueue->Elements;
        hqueue->SavedElements++;
        
        valueToReturn = QUEUE_OK;

        if(hqueue->Tail == hqueue->Elements){
            hqueue->Tail = 0U;
        }
    }
    
    return valueToReturn;
}


/**
 * @brief   Reads data from the buffer controlled by hqueue, the data is copied into the data type referenced by the empty pointer data
 * @param   hqueue Queue Handler
 * @param   data Variable where data will be copied
 */
uint8_t HIL_QUEUE_Read( QUEUE_HandleTypeDef *hqueue, void *data )
{
    uint8_t valueToReturn;

    assert_error( hqueue->Buffer   != NULL, QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hqueue->Elements != 0U,   QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hqueue->Size     != 0U,   QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( data             != NULL, QUEUE_PAR_ERROR );              /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( HIL_QUEUE_IsEmpty( hqueue ) == 1U){
        hqueue->Empty = 1U;
        valueToReturn = QUEUE_NOT_OK;
    }
    else{
        (void)memcpy(data, hqueue->Buffer + (hqueue->Size * hqueue->Head), hqueue->Size);                   /* cppcheck-suppress misra-c2012-18.4 ; Writing into memory using pointer */
        hqueue->Head = (hqueue->Head + 1U) % hqueue->Elements;
        hqueue->SavedElements--;

        valueToReturn = QUEUE_OK;

        if(hqueue->Head == hqueue->Elements){
            hqueue->Head = 0U;
        }
    }

    return valueToReturn;
}


/**
 * @brief   The function returns a one if there are no more elements that can be read from the queue and zero if there is at least one element that can be read.
 * @param   hqueue Queue handler
 */
uint8_t HIL_QUEUE_IsEmpty( QUEUE_HandleTypeDef *hqueue )
{
    assert_error( hqueue->Buffer   != NULL, QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hqueue->Elements != 0U,   QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hqueue->Size     != 0U,   QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    return (hqueue->SavedElements == 0U);
}


/**
 * @brief   The function must empty the queue in case it has elements inside it, the information will be discarded
 * @param   hqueue Queue handler 
 */
void HIL_QUEUE_Flush( QUEUE_HandleTypeDef *hqueue )
{
    assert_error( hqueue->Buffer   != NULL, QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hqueue->Elements != 0U,   QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hqueue->Size     != 0U,   QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    
    hqueue->Head          = 0U;
    hqueue->Tail          = 0U;
    hqueue->Full          = 0U;
    hqueue->Empty         = 1U;
    hqueue->SavedElements = 0U;

}


/**
 * @brief   The function returns a one if the queue is full, 0 otherwise
 * @param   hqueue Queue handler
 */
static uint8_t HIL_QUEUE_IsFull( QUEUE_HandleTypeDef *hqueue )
{
    assert_error( hqueue->Buffer   != NULL, QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hqueue->Elements != 0U,   QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    assert_error( hqueue->Size     != 0U,   QUEUE_PAR_ERROR);               /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */
    
    return (hqueue->SavedElements == hqueue->Elements);
}

/**
 * @brief   Copies the information referenced by the empty pointer data to the buffer controlled by hqueue and disable interrupt
 * @param   hqueue Queue Handler
 * @param   data Variable where data will be copied
 * @param   isr Interrupt ID that will be disabled
 */
uint8_t HIL_QUEUE_WriteISR( QUEUE_HandleTypeDef *hqueue, void *data, uint8_t isr )
{
    uint8_t valueToReturn;

    assert_error( data             != NULL, QUEUE_PAR_ERROR );              /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( isr == 0xFFU )                       //Enable/disable all interrupt
    {
        __disable_irq();
        valueToReturn = HIL_QUEUE_Write( hqueue, data );
        __enable_irq();
    }
    else
    {
        if( (isr >= 0U) && (isr <= 30U))          //Validate that isr is a member of IRQn_Type enum
        {  
            HAL_NVIC_DisableIRQ( isr );
            valueToReturn = HIL_QUEUE_Write( hqueue, data );
            HAL_NVIC_EnableIRQ( isr );
        }
        else
        {
            valueToReturn = QUEUE_NOT_OK;
        }
    }
    
    return valueToReturn;
}


/**
 * @brief   Reads data from the buffer controlled by hqueue, the data is copied into the data type referenced by the empty pointer data  and operates with interrupt
 * @param   hqueue Queue Handler
 * @param   data Variable where data will be copied
 * @param   isr Interrupt ID that will be disabled
 */
uint8_t HIL_QUEUE_ReadISR( QUEUE_HandleTypeDef *hqueue, void *data, uint8_t isr )
{
    uint8_t valueToReturn;

    assert_error( data             != NULL, QUEUE_PAR_ERROR );              /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( isr == 0xFFU )                       //Enable/disable all interrupt
    {
        __disable_irq();
        valueToReturn = HIL_QUEUE_Read( hqueue, data );
        __enable_irq();
    }
    else
    {
        if( (isr >= 0U) && (isr <= 30U))          //Validate that isr is a member of IRQn_Type enum
        {  
            HAL_NVIC_DisableIRQ( isr );
            valueToReturn = HIL_QUEUE_Read( hqueue, data );
            HAL_NVIC_EnableIRQ( isr );
        }
        else
        {
            valueToReturn = QUEUE_NOT_OK;
        }
    }
    
    return valueToReturn;
}


/**
 * @brief   The function returns a one if there are no more elements that can be read from the queue and zero if there is at least one element that can be read.
 * @param   hqueue Queue Handler
 * @param   isr Interrupt ID that will be disabled
 */
uint8_t HIL_QUEUE_IsEmptyISR( QUEUE_HandleTypeDef *hqueue, uint8_t isr )
{
    uint8_t valueToReturn;

    assert_error( (isr == 0xFFu) || ( (isr >= 0U) && (isr <= 30U) ), QUEUE_PAR_ERROR);          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( isr == 0xFFU )                       //Enable/disable all interrupt
    {
        __disable_irq();
        valueToReturn = HIL_QUEUE_IsEmpty( hqueue );
        __enable_irq();
    }
    else
    {
        if( (isr >= 0U) && (isr <= 30U))          //Validate that isr is a member of IRQn_Type enum
        {  
            HAL_NVIC_DisableIRQ( isr );
            valueToReturn = HIL_QUEUE_IsEmpty( hqueue );
            HAL_NVIC_EnableIRQ( isr );
        }
        else
        {
            valueToReturn = QUEUE_NOT_OK;
        }
    }
    
    return valueToReturn;
}


/**
 * @brief   The function must empty the queue in case it has elements inside it, the information will be discarded and operates wih interrupt
 * @param   hqueue Queue Handler
 * @param   isr Interrupt ID that will be disabled
 */
void HIL_QUEUE_FlushISR( QUEUE_HandleTypeDef *hqueue, uint8_t isr )
{
    assert_error( (isr == 0xFFu) || ( (isr >= 0U) && (isr <= 30U) ), QUEUE_PAR_ERROR);          /* cppcheck-suppress misra-c2012-11.8 ; Assert error macro */

    if( isr == 0xFFU )                       //Enable/disable all interrupt
    {
        __disable_irq();
        HIL_QUEUE_Flush( hqueue );
        __enable_irq();
    }
    else
    {
        HAL_NVIC_DisableIRQ( isr );
        HIL_QUEUE_Flush( hqueue );
        HAL_NVIC_EnableIRQ( isr );
    }   
}