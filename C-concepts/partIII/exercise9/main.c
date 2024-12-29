#include <stdio.h>

/*
a) 1
b) 0
c) 1
d) 0
e) 1
f) 0
g) 0
h) 1
i) 0
j) 1
*/

int main(void){
    int i = 1, j = 2, k = 3, m = 2;

    printf("a) ");   printf( "%d", i == 1 ); printf("\n");
    printf("b) ");  printf( "%d", j == 3 ); printf("\n");
    printf("c) ");  printf( "%d", i >= 1 && j < 4 );    printf("\n");
    printf("d) ");  printf( "%d", m <= 99 && k < m );   printf("\n");
    printf("e) ");  printf( "%d", j >= i || k == m );   printf("\n");
    printf("f) ");  printf( "%d", k + m < j || 3 - j >= k );    printf("\n");
    printf("g) ");  printf( "%d", !m); printf("\n");
    printf("h) ");  printf( "%d", !( j - m ) ); printf("\n");
    printf("i) ");  printf( "%d", !( k > m ) ); printf("\n");
    printf("j) ");  printf( "%d", !( j > k ) ); printf("\n");
}