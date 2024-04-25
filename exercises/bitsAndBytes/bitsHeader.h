#ifndef BITSHEADER_H_
#define BITSHEADER_H_

/*
  Print 8-bit number in binary representation
*/
void printB( unsigned char byte );

/*
  Validates and shift a 8-bit number n-bits to rigth
*/
unsigned char ShiftBitRigth( unsigned char byte, unsigned char shift );

/*
  Validates and shift a 8-bit number n-bits to left
*/
unsigned char ShiftBitLeft( unsigned char byte, unsigned char shift );

/*
  Set mask of bits in a given number
  Example. number=0b00000011, mask=0b00100000, result=0b00100011
*/
unsigned char SetBitMask( unsigned char byte, unsigned char mask );

/*
  Clear mask of bits in a given number
  Example. number=0b00000011, mask=0b00100001, result=0b00000010
*/
unsigned char ClearBitMask( unsigned char byte, unsigned char mask );

/*
  Toggle mask of bits in a given number
  Example. number=0b00000011, mask=0b00100001, result=0b00100010
*/
unsigned char ToggleBitMask( unsigned char byte, unsigned char mask );

/*
  Set specified bit in a given number
  Example. number=0b00000011, bit=3, result=0b00001011
*/
unsigned char SetBit( unsigned char byte, unsigned char bit );

/*
  clear specified bit in a given number
  Example. number=0b00000011, bit=0, result=0b00000010
*/
unsigned char ClearBit( unsigned char byte, unsigned char bit );

/*
  Toggle specified bit in a given number
  Example. number=0b00001111, bit=1, result=0b00001101
*/
unsigned char ToggleBit( unsigned char byte, unsigned char bit );

/*
  Return specified bit of a given number
  Example. number=0b00001111, bit=1, result=1
*/
unsigned char GetBit( unsigned char byte, unsigned char bit );

#endif