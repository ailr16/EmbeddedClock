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

int q22(void)
{
    unsigned int r;
    r = 1;
    printf("%d\t%x\t%x\n", r > -4, r, -4);
    for( r = 1; r > -4; r-- )
        printf("Welcome");
}

int main(void)
{
    q22();   
    return 0;
}