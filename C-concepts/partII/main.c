#include <stdio.h>
#include <stdbool.h>

//  gcc main.c -o main.o && ./main.o

/*
    Exercise 1. Driven miles and gallons.
    Get fuel gallons and driven miles to compute statistics.
    NOTE: Implemented sentinel value to end.
*/
void exercise1(void){    
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


/*
    Exercise 2. Bank account.
    Get values for a bank account.
*/
void exercise2(void){
    int accountNumber;
    float begginingBalance, totalCharges, totalCredits, allowedCredit, newBalance;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    
    printf("Enter Month beggining balance: ");
    scanf("%f", &begginingBalance);

    printf("Enter Month total charges: ");
    scanf("%f", &totalCharges);

    printf("Enter Month total credits: ");
    scanf("%f", &totalCredits);

    printf("Enter Allowed credit: ");
    scanf("%f", &allowedCredit);

    newBalance = begginingBalance + totalCharges - totalCredits;
    
    printf("New balance: %f\n", newBalance);

    if(newBalance > allowedCredit)  printf("Credit Limit Exceeded\n");
}


/*
    Exercise 3. Bank account with sentinel value.
    Get values for a bank account, prints if exceed credit.
    NOTE: Implemented sentinel value to end.
*/
void exercise3(void){
    int accountNumber;
    float begginingBalance, totalCharges, totalCredits, allowedCredit, newBalance;

    while(true){

        printf("Enter account number (-1 to end): ");
        scanf("%d", &accountNumber);
        
        if(accountNumber == -1) break;

        printf("Enter Month beggining balance: ");
        scanf("%f", &begginingBalance);

        printf("Enter Month total charges: ");
        scanf("%f", &totalCharges);

        printf("Enter Month total credits: ");
        scanf("%f", &totalCredits);

        printf("Enter Allowed credit: ");
        scanf("%f", &allowedCredit);

        printf("\n");

        newBalance = begginingBalance + totalCharges - totalCredits;
        
        if(newBalance > allowedCredit){
            printf("Account number: %d\n", accountNumber);
            printf("Credit limit: %f\n", allowedCredit);
            printf("Balance: %f\n", newBalance);
            printf("Credit Limit Exceeded\n\n");
        }
    }
}


/*
    Exercise 4. Compute salary based in sales
    Get sales of an employee an compute salary.
    NOTE: Implemented sentinel value to end.
*/
void exercise4(void){
    float sales;

    while(1){
        printf("Enter sales in dollars (-1 to end): ");
        scanf("%f", &sales);

        if(sales == -1) break;

        printf("Salary is: %f \n\n", 200 + sales*0.09);
    }
}


/*
    Exercise 5. Compute interest charge.
    NOTE: Implemented sentinel value to end.
*/
void exercise5(void) {
    float loanPrincipal, interestRate, loanDays;

    while(1){
        printf("Enter loan principal: ");
        scanf("%f", &loanPrincipal);

        if(loanPrincipal == -1) break;

        printf("Enter interest rate: ");
        scanf("%f", &interestRate);

        printf("Enter loan days: ");
        scanf("%f", &loanDays);

        printf("Interest charge: %f\n\n", loanPrincipal * interestRate * loanDays /365);
    }
}


/*
    Exercise 6. Practicing with pre and post decrement operators.
*/
void exercise6(void){
    int i = 10;
    printf("Sart value i=%d\n", i);
    printf("This line has a predecrement --i=%d\n", --i);
    printf("This line has a predecrement --i=%d\n", --i);
    printf("This line has a postdecrement i--=%d\n", i--);
    printf("The past line had a postdecrement (value is not changed in this line)i=%d\n", i);
    printf("This line has a postdecrement i--=%d\n", i--);
    printf("The past line had a postdecrement (value is not changed in this line)i=%d\n", i);
}


/*
    Exercise 7. Print 1 to 10, 1 increment 
*/
void exercise7(void){
    for(int i=1; i<11; i++) printf("%d   ", i);
    printf("\n");
}


/*
    Exercise 8. Print 1 to 10 table multiplied by 10, 100 and 1000
*/
void exercise8(void){
    printf("N\t10*N\t100*N\t1000*N\n");
    for(int N = 1; N < 11; N++) printf("%d\t%d\t%d\t%d\n", N, 10*N, 100*N, 1000*N);
}


/*
    Exercise 9. Determine if given 5-digit number is palindrome
*/
int exercise9(void){
    int number, auxNumber;
    int digit1, digit2, digit3, digit4, digit5;

    printf("Enter a 5-digit number: ");
    scanf("%d", &number);

    if(number < 10000 || number > 99999){
        printf("%d is not 5-digit number!\n", number);
        return 0;
    }

    digit1 = number / 10000;
    digit2 = (number % 10000) / 1000;
    digit3 = ((number % 10000) % 1000) / 100;
    digit4 = (((number % 10000) % 1000) % 100) / 10;
    digit5= ((((number % 10000) % 1000) % 100) % 10);

    printf("%d %d %d %d %d\n", digit1, digit2, digit3, digit4, digit5);
    if(digit1 == digit5 && digit2 == digit4) printf("%d is palindrome!\n", number);
}


/*
    Exercise 10. Get a 5-bit number in binary and convert to decimal representation
*/
int exercise10(void){
    int number, decimalNumber;
    int digit1, digit2, digit3, digit4, digit5;

    printf("Enter a 5-digit binary number: ");
    scanf("%d", &number);

    digit1 = number / 10000;
    if(digit1 != 0 && digit1 != 1){
        printf("Not a binary number!\n");
        return 0;
    }

    digit2 = (number % 10000) / 1000;
    if(digit2 != 0 && digit2 != 1){
        printf("Not a binary number!\n");
        return 0;
    }

    digit3 = ((number % 10000) % 1000) / 100;
    if(digit3 != 0 && digit3 != 1){
        printf("Not a binary number!\n");
        return 0;
    }

    digit4 = (((number % 10000) % 1000) % 100) / 10;
    if(digit4 != 0 && digit4 != 1){
        printf("Not a binary number!\n");
        return 0;
    }

    digit5= ((((number % 10000) % 1000) % 100) % 10);
    if(digit5 != 0 && digit5 != 1){
        printf("Not a binary number!\n");
        return 0;
    }

    decimalNumber = digit1*16 + digit2*8 + digit3*4 + digit4*2 + digit5;

    printf("Binary Number: %d %d %d %d %d\n", digit1, digit2, digit3, digit4, digit5);
    printf("Decimal: %d\n", decimalNumber);
}


/*
    Exercise 11. Print square of *, size 10x10
    NOTE: Using while
*/
void exercise11(void){
    int i = 1;
    
    while(i < 101){
        printf("*");
        if(i % 10 == 0) printf("\n");
        i++;
    }
}


/*
    Exercise 12. Count 7's in a 5-digit given number
    NOTE: Using while
*/
int exercise12(void){
    int number, totalSevens = 0;
    int digit1, digit2, digit3, digit4, digit5;

    printf("Enter a 5-digit number: ");
    scanf("%d", &number);

    if(number < 10000 || number > 99999){
        printf("%d is not 5-digit number!\n", number);
        return 0;
    }

    digit1 = number / 10000;
    digit2 = (number % 10000) / 1000;
    digit3 = ((number % 10000) % 1000) / 100;
    digit4 = (((number % 10000) % 1000) % 100) / 10;
    digit5= ((((number % 10000) % 1000) % 100) % 10);

    if(digit1 == 7) totalSevens++;
    if(digit2 == 7) totalSevens++;
    if(digit3 == 7) totalSevens++;
    if(digit4 == 7) totalSevens++;
    if(digit5 == 7) totalSevens++;

    printf("%d has %d 7s\n", number, totalSevens);
}


/*
    Exercise 13. Print square pattern of * with given size
    NOTE: Size between 1 and 20
*/
int exercise13(void){
    int side;

    printf("Enter side of square (from 1 to 20): ");
    scanf("%d", &side);

    if(side < 0 && side > 20){
        printf("The side must be between 1 and 20!");
        return 0;
    }

    for(int i = 0; i < side; i++){
        for(int j = 0; j < side; j++){
            printf("*");
        }
        printf("\n");
    }
}

int main(void){
    (void)exercise13();
    return 0;
}