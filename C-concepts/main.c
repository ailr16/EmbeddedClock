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
    
    return 0;
}

int q23(void)
{
    int a = 0, b = 100;
    int i = 0;
    do
    {
        --b;
        a += 2;
        i++;
        printf("%d %d %d\n", i, a, b);
    }while(a < b);
    printf("%d", a - b);

    return 0;
}

int q24(void)
{
    printf("%d", printf("Welcome"));
    return 0;
}

int q32(void)
{
    char c = 100;
    printf("%x", c % 9);

    return 0;
}

int q39(void)
{
    int n = -20;
    printf("%d %d\n", n, !n);
}

int main(void)
{
    q39();
    return 0;
}