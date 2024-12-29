#ifndef BITSHEADER_H_
#define BITSHEADER_H_

/*
    Return average of given array with specified size
*/
unsigned char average( unsigned char *array, unsigned char size);

/*
    Copy array A to array B
*/
void arrayCopy( unsigned char *arrayA, unsigned char *arrayB, unsigned char size );

/*
    Compare two arrays, if one element is different return 1, otherwise 0
*/
unsigned char arrayCompare( unsigned char *arrayA, unsigned char *arrayB, unsigned char size );

/*
    Return the largest element of a given array
*/
unsigned short largest( unsigned short int *array, unsigned char size );

/*
    Order elements in a given array
*/
void sortArray( unsigned short int *array, unsigned char size );

/*
    Create a stylized string with given hour, minute and seconds
*/
void TimeString( char *string, unsigned char hours, unsigned char minutes, unsigned char seconds);

/*
    Create a stylized string with given month, day, year and weekday
*/
void DateString( char *string, unsigned char month, unsigned char day, unsigned short int year, unsigned char weekday );

#endif