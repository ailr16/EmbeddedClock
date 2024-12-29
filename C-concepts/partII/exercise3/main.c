#include <stdio.h>
#include <stdbool.h>

int main(void){
    int accountNumber;
    float begginingBalance, totalCharges, totalCredits, allowedCredit, newBalance;

    while(true){

        printf("Enter account number: ");
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