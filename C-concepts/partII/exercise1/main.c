#include <stdio.h>

int main(void){    
    float overallSum = 0;
    float actualGallons = 0, actualMilesDriven;

    int i = 0;
    while(actualGallons != -1){
        printf("Enter the gallons used (-1 to end): ");
        scanf("%f", &actualGallons);

        if (actualGallons == -1)    break;

        printf("Enter the miles driven: ");
        scanf("%f", &actualMilesDriven);

        printf("The miles / gallon for this tank was %f\n\n", actualMilesDriven / actualGallons);
        overallSum = overallSum + actualMilesDriven / actualGallons;
        i++;
    }
    printf("The overall average miles / gallon was: %f", overallSum / i);
}