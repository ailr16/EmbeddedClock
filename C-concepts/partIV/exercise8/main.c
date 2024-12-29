#include <stdio.h>
#include <stdint.h>

uint8_t Validate_Date(uint8_t days, uint8_t month, uint16_t year);

int main(void){
    int day, month, year;
    printf("Enter date: ");
    scanf("%d %d %d", &day, &month, &year);
    printf("Is %d, %d, %d valid?: %d\n", day, month, year, Validate_Date(day,month,year));
}

uint8_t Validate_Date(uint8_t days, uint8_t month, uint16_t year)
{
    uint8_t yearValidation = year > 1899 && year < 2101;
    uint8_t monthValidation = month > 0 && month < 13;
    uint8_t dayValidation;
    
    if(month == 1 || month == 3 || month == 5 || month == 8 || month == 10 || month == 12){
        dayValidation = days > 0 && days < 32;
    }
    if(month == 4 || month == 6 || month == 7 || month == 9 || month == 11){
        dayValidation = days > 0 && days < 31;
    }
    if((year % 4 == 0) && year != 1900 && year != 2100){  //Leap year validation true
        if(month == 2){
            dayValidation = days > 0 && days < 30;
        }
    }
    else{
        if(month == 2)  dayValidation = days > 0 && days < 29;
    }
    return yearValidation && monthValidation && dayValidation;
}