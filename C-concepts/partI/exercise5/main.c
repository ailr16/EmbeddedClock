#include <stdio.h>

float radius;
const float pi = 3.14159;

int main(void){
    printf("Enter the radius: ");
    scanf("%f", &radius);

    printf("Diameter: %f\n", radius*2);
    printf("Circumference: %f\n", 2*pi*radius);
    printf("Area: %f\n", pi*radius*radius);
}