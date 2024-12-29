#include <stdio.h>

int main(void) {
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