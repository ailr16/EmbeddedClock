#include <stdio.h>

int main(void){
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