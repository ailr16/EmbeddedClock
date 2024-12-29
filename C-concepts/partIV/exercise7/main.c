#include <stdio.h>
#include <stdint.h>

uint8_t Validate_Time(uint8_t hour, uint8_t minutes, uint8_t seconds);

int main(void){
    printf("Is %d:%d:%d a valid time?:%d\n", 10, 10, 10, Validate_Time(10, 10, 10));
    printf("Is %d:%d:%d a valid time?:%d\n", 25, 0, 0, Validate_Time(25, 0, 0));
    printf("Is %d:%d:%d a valid time?:%d\n", 0, 60, 0, Validate_Time(0, 60, 0));
    printf("Is %d:%d:%d a valid time?:%d\n", 0, 0, 60, Validate_Time(0, 0, 60));
    printf("Is %d:%d:%d a valid time?:%d\n", 0, 0, 0, Validate_Time(0, 0, 0));
    printf("Is %d:%d:%d a valid time?:%d\n", 23, 59, 59, Validate_Time(23, 59, 59));
}

uint8_t Validate_Time(uint8_t hour, uint8_t minutes, uint8_t seconds){
    return hour < 24 && minutes < 60 && seconds < 60;
}