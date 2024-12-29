#include <stdio.h>

int main(void){
    int binario[9];
    printf("Decimal\tOctal\tHex\tBinary\n");
    int i, j, iCopia;
    
    for(i=1;i<257;i++){
        iCopia = i;

        for(j=0; j < 9;j++){
            binario[j] = iCopia % 2;
            iCopia = iCopia / 2;
        }
        
        printf("%d\t%o\t%X\t", i, i, i);
        for(j=8; j>=0; j--) printf("%d", binario[j]);
        printf("\n");
    }
}