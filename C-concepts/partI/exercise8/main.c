#include <stdio.h>

int largest, smallest, actualNumber;

int main(void){
    printf("Enter a number (1): ");
    scanf("%d", &actualNumber);
    largest = actualNumber;
    smallest = actualNumber;

    printf("Enter another number (2): ");
    scanf("%d", &actualNumber);
    if (actualNumber > largest) largest = actualNumber;
    if (actualNumber < smallest) smallest = actualNumber;

    printf("Enter another number (3): ");
    scanf("%d", &actualNumber);
    if (actualNumber > largest) largest = actualNumber;
    if (actualNumber < smallest) smallest = actualNumber;

    printf("Enter another number (4): ");
    scanf("%d", &actualNumber);
    if (actualNumber > largest) largest = actualNumber;
    if (actualNumber < smallest) smallest = actualNumber;

    printf("Enter another number (5): ");
    scanf("%d", &actualNumber);
    if (actualNumber > largest) largest = actualNumber;
    if (actualNumber < smallest) smallest = actualNumber;

    printf("Largest number: %d\n", largest);
    printf("Smallest number: %d\n", smallest);
}