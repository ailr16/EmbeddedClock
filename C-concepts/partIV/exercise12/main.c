#include <stdio.h>

int gcd(int numA, int numB);

int main(void){
    int numA, numB;
    scanf("%d %d", &numA, &numB);
    printf("GCD of %d and %d is %d\n", numA, numB, gcd(numA, numB));
}

int gcd(int numA, int numB){
    int i;
    int max;
    if(numA > numB){
        max = numA;
    }
    else{
        max = numA;
    }
    for(i = max; i > 0; i--){
        if( (numA % i == 0) && (numB % i == 0)){
            break;
        }
    }
    return i;
}