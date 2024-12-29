#include <stdio.h>

int num1, num2, num3, largest, smallest;

int main(void){
    printf("Enter three numbers: ");
    scanf("%d %d %d", &num1, &num2, &num3);
    printf("Sum is %d\n", num1 + num2 + num3);
    printf("Average is %d\n", (num1 + num2 + num3)/3);
    printf("Product is %d\n", num1 * num2 * num3);
    
    if(num1 > num2 && num1 > num3) largest = num1;
    if(num2 > num1 && num2 > num3) largest = num2;
    if(num3 > num1 && num3 > num1) largest = num3;

    if(num1 < num2 && num1 < num3) smallest = num1;
    if(num2 < num1 && num2 < num3) smallest = num2;
    if(num3 < num1 && num3 < num1) smallest = num3;

    printf("Smallest is %d\n", smallest);
    printf("Largest is %d\n", largest);

}