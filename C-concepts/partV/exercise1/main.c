#include <stdio.h>

#define MAX_ELEMENTS 9


int main(void){
    int countersArray[MAX_ELEMENTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int sales, employeeSalary;

    while(1){
        printf("Enter employee sales (-1 to end program): ");
        scanf("%d", &sales);

        if( sales == -1 ){
            break;
        }

        employeeSalary = 200 + (sales * 0.09);

        printf("Employee Salary: %d\n\n", employeeSalary);

        if(employeeSalary >= 1000){
            countersArray[8]++;
        }
        else{
            countersArray[(employeeSalary / 100) - 2]++;
        }
    }

    printf("Salary range\tEmployees\n");
    for (int i = 0; i < MAX_ELEMENTS - 1; i++){
        printf("$%d-$%d\t%d\n", 100*(i+2), (100*(i+2))+99,countersArray[i]);
    }
    printf("Over $1000\t%d\n", countersArray[8]);
}
