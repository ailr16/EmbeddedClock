#include <stdio.h>

int main(void){
    int totalRows, n;
    
    printf("Enter total rows for diamond pattern (odd number from 1 to 19): ");
    scanf("%d", &totalRows);

    if(totalRows < 1 || totalRows > 19){
        printf("Number must be between 1 and 19!\n");
        return 0;
    }
    
    if(totalRows % 2 == 0){
        printf("%d is not an odd number!\n", totalRows);
        return 0;
    }


    n = totalRows / 2 + totalRows % 2;
    
    int i, j, k;
    for(i=1; i<=n; i++){
        for(j=i; j < n; j++) printf(" ");
        for(k=1; k < 2*i; k++) printf("*");
        printf("\n");
    }

    for(i = n - 1; i>= 1; i--){
        for(j=n; j>i; j--)  printf(" ");
        for(k=1; k < 2*i; k++)  printf("*");
        printf("\n");
    }
}

