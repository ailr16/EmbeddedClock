#include <stdio.h>

int SecondsSince12(int hours, int minutes, int seconds);

int main(void){
    printf("Time 1: %d:%d:%d Seconds from twelve=%d\n", 2, 34, 56, SecondsSince12(2, 34, 56));
    printf("Time 2: %d:%d:%d Seconds from twelve=%d\n", 11, 59, 59, SecondsSince12(11, 59, 59));
    printf("Seconds between Time 1 and Time 2= %d\n", SecondsSince12(11, 59, 59) - SecondsSince12(2, 34, 56));
}

int SecondsSince12(int hours, int minutes, int seconds){
    return hours * 3600 + minutes * 60 + seconds;
}