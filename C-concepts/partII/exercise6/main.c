#include <stdio.h>

int main(void){
    int i = 10;
    printf("Sart value i=%d\n", i);
    printf("This line has a predecrement --i=%d\n", --i);
    printf("This line has a predecrement --i=%d\n", --i);
    printf("This line has a postdecrement i--=%d\n", i--);
    printf("The past line had a postdecrement (value is not changed in this line)i=%d\n", i);
    printf("This line has a postdecrement i--=%d\n", i--);
    printf("The past line had a postdecrement (value is not changed in this line)i=%d\n", i);
}