#ifndef BUFFER_H_
#define BUFFER_H_

typedef struct{
    unsigned char *Buffer;          /* Pointer to array used as buffer     */
    unsigned long Elements;         /* Size of buffer                      */
    unsigned long Head;             /* Pointing to first element in buffer */
    unsigned long Tail;             /* Pointing to last element in buffer  */
    unsigned char Empty;            /* Boolean is buffer empty?            */
    unsigned char Full;             /* Boolean is buffer full?             */
    unsigned long SavedElements;    /* Counter for saved elements          */
} Buffer_t;

/*
    Initialize the buffer. Set head and tail to 0, clear booleans empty and full
    and clear saved elements counter.
*/
void Buffer_Init( Buffer_t *hbuffer );

/*
    Add element to buffer. Update head
*/
void Buffer_Write( Buffer_t *hbuffer, unsigned char data );

/*
    Get element from buffer. Update tail.
*/
unsigned char Buffer_Read( Buffer_t *hbuffer );

/*
    Return boolean is buffer empty?
*/
unsigned char Buffer_IsEmpty( Buffer_t *hbuffer );

#endif