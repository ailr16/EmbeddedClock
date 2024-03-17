#include <stdio.h>

int q8(void)
{
    int z = 1;
    if (--z, ++z)
        printf("%dWelcome1\n", z);
    else
        printf("Bye1");
    return 0;
}

int main(void)
{
    q8();   
    return 0;
}