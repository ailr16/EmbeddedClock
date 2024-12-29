#include <stdio.h>

//  gcc main.c -o main.o && ./main.o

/*
    Exercise 1.
    Get two numbers and print sum, product, difference, quotient and remainder.
*/
void exercise1(void){
    int num1, num2, sum, product, difference, quotient, remainderVar;

    printf("Enter a number: ");
    scanf("%d", &num1);

    printf("Enter another number: ");
    scanf("%d", &num2);

    printf("Number 1 is %d , and number 2 is %d \n", num1, num2);

    sum = num1 + num2;
    product = num1 * num2;
    difference = num1 - num2;
    quotient = num1 / num2;
    remainderVar = num1 % num2;
    
    printf("sum = %d, difference = %d, product = %d, quotient = %d, remainder = %d\n", sum, difference, product, quotient, remainderVar);
}


/*
    Exercise 2.
    Print "1 2 3 4" using three different methods
*/
void exercise2(void){
    printf("1 2 3 4\n");

    printf("%d %d %d %d\n", 1, 2, 3 ,4);
    
    printf("1 ");
    printf("2 ");
    printf("3 ");
    printf("4\n");
}


/*
    Exercise 3.
    Get two numbers and print the result of comparison
*/
void exercise3(void){
    int num1, num2;

    printf("Enter a number: ");
    scanf("%d", &num1);

    printf("Enter another number: ");
    scanf("%d", &num2);

    if (num1 > num2) printf("%d is larger\n", num1);
    if (num2 > num1) printf("%d is larger\n", num2);
    if (num1 == num2) printf("These numbers are equal\n");
}

/*
    Exercise 4.
    Get three numbers and print sum, average, product, largest and smallest
*/
void exercise4(void){
    int num1, num2, num3, largest, smallest;

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


/*
    Exercise 5.
    Get circle radius and print diameter, circumference and area
*/
void exercise5(void){
    float radius;
    const float pi = 3.14159;

    printf("Enter the radius: ");
    scanf("%f", &radius);

    printf("Diameter: %f\n", radius*2);
    printf("Circumference: %f\n", 2*pi*radius);
    printf("Area: %f\n", pi*radius*radius);
}


/*
    Exercise 6.
    Print fixed patterns
*/
void exercise6(void){
    printf("*********       ***         *             *\n");
    printf("*       *     *     *      ***          *   *\n");
    printf("*       *     *     *     *****       *       *\n");
    printf("*       *     *     *       *       *           *\n");
    printf("*       *     *     *       *     *               *\n");
    printf("*       *     *     *       *       *           *\n");
    printf("*       *     *     *       *         *       *\n");
    printf("*       *     *     *       *           *   *\n");
    printf("*********       ***         *             *\n");

    printf("*********\n");
    printf("*       *\n");
    printf("*       *\n");
    printf("*       *\n");
    printf("*       *\n");
    printf("*       *\n");
    printf("*       *\n");
    printf("*       *\n");
    printf("*********\n");

    printf("  ***\n");
    printf("*     *\n");
    printf("*     *\n");
    printf("*     *\n");
    printf("*     *\n");
    printf("*     *\n");
    printf("*     *\n");
    printf("*     *\n");
    printf("  *** \n");

    printf("  *\n");
    printf(" ***\n");
    printf("*****\n");
    printf("  *\n");
    printf("  *\n");
    printf("  *\n");
    printf("  *\n");
    printf("  *\n");
    printf("  *\n");

    printf("        *\n");
    printf("      *   *\n");
    printf("    *       *\n");
    printf("  *           *\n");
    printf("*               *\n");
    printf("  *           *\n");
    printf("    *       *\n");
    printf("      *   *\n");
    printf("        *\n");
}


/*
    Exercise 7.
    Print fixed patterns
*/
void exercise7(void){
    printf( "\n**\n**\n****\n*****\n" );
}


/*
    Exercise 8.
    Get 5 numbers and find the largest and smallest
*/
void exercise8(void){
    int largest, smallest, actualNumber;

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


/*
    Exercise 9.
    Get a number and determine if it's odd or even
*/
void exercise9(void){
    int number;

    printf("Enter a number: ");
    scanf("%d", &number);
    
    if (number % 2 == 0)    printf("%d is an even number\n", number);
    if (number % 2 != 0)    printf("%d is an odd number\n", number);
}


/*
    Exercise 10.
    Print rotated letter using patterns
*/
void exercise10(void){
    printf("AAAAAAA\n");
    printf("    A  AA\n");
    printf("    A    A\n");
    printf("    A  AA\n");
    printf("AAAAAAA\n");
    printf("\n");
    printf("I         I\n");
    printf("IIIIIIIIIII\n");
    printf("I         I\n");
    printf("\n");
    printf("LLLLLLLLLLL\n");
    printf("L\n");
    printf("L\n");
    printf("L\n");
    printf("\n");
    printf("RRRRRRRRRRR\n");
    printf("     RR   R\n");
    printf("    R R   R\n");
    printf("   R  R   R\n");
    printf("  R     R\n");
    printf("R\n");
}


/*
    Exercise 11.
    Get two numbers and determine if are multiple
*/
void exercise11(void){
    int number1, number2;

    printf("Enter two numbers: ");
    scanf("%d %d", &number1, &number2);

    if (number2 % number1 == 0) printf("%d is multiple of %d\n", number2, number1);
    else printf("Not multiple\n");
}


/*
    Exercise 12.
    Print pattern using newline char
*/
void exercise12(void){
    printf("* * * * * * * *\n");
    printf(" * * * * * * * *\n");
    printf("* * * * * * * *\n");
    printf(" * * * * * * * *\n");
    printf("* * * * * * * *\n");
    printf(" * * * * * * * *\n");
    printf("* * * * * * * *\n");
    printf(" * * * * * * * *\n");

    printf("\n");
    printf("* * * * * * * *\n * * * * * * * *\n* * * * * * * *\n * * * * * * * *\n* * * * * * * *\n * * * * * * * *\n* * * * * * * *\n * * * * * * * *\n");
}


/*
    Exercise 13.
    Get a 5-digit number and print each digit
*/
void exercise13(void){
    int number;

    printf("Enter a 5-digit number: ");
    scanf("%d", &number);

    if (number < 10000 || number > 99999)   printf("%d is not a 5-digit number!\n", number);
    else{
        printf("%d ", number / 10000);
        printf("%d ", (number % 10000) / 1000);
        printf("%d ", ((number % 10000) % 1000) / 100);
        printf("%d ", (((number % 10000) % 1000) % 100) / 10);
        printf("%d\n", (((number % 10000) % 1000) % 100) % 10);
    }
}


/*
    Exercise 14.
    Print square and cube of from 0 to 10 using loops
*/
void exercise14(void){
    printf("\n");
    printf("number\tsquare\tcube\n");
    for (int i = 0; i < 11; i++){
        printf("%d\t%d\t%d\n",i, i*i, i*i*i);
    }
}

int main(void){
    exercise14();
    return 0;
}