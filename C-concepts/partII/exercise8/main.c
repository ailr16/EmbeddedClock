#include <stdio.h>

int main(void){
    printf("N\t10*N\t100*N\t1000*N\n");
    for(int N = 1; N < 11; N++)    printf("%d\t%d\t%d\t%d\n", N, 10*N, 100*N, 1000*N);
}