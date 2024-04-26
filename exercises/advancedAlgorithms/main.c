#include <stdio.h>
#include <stdlib.h>

//  gcc main.c -o main.o && ./main.o

/*
    Exercise 1.
    Classify employees salary in ranges with array
*/
void exercise1(void){
    #define MAX_ELEMENTS 9

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


/*
    Exercise 2.
    WIP
*/
int validateNumber(int number){
    if(number >= 10 && number <= 100) return number;
    else return -1;
}

void exercise2(void){
    #define TOTAL_ELEMENTS 5

    int numbersArray[TOTAL_ELEMENTS];
    int actualNumber;
    int savedNumbers = 0;

    //Save first number
    scanf("%d", &actualNumber);
    actualNumber = validateNumber(actualNumber);    //Verify it's between 10 and 100
    numbersArray[savedNumbers] = actualNumber;
    printf("%d ", actualNumber);
    savedNumbers++;

    for(int i = 1; i < TOTAL_ELEMENTS; i++){
        scanf("%d", &actualNumber);
        actualNumber = validateNumber(actualNumber);    //Verify it's between 10 and 100
        for(int j = 0; j < savedNumbers; j++){
            if(actualNumber == numbersArray[j]){
                break;
            }
            if(j == (savedNumbers - 1)){
                numbersArray[savedNumbers++] = actualNumber;
                printf("%d ", actualNumber);
            }
        }
    }
    
    printf("\n");
}


/*
    Exercise 3.
    Compute prime numbers in the range 1 to 1000
*/
unsigned char multiple(int numberA, int numberB){
    return numberB % numberA == 0;
}

unsigned char IsPrime(int number){
    for(int i = (number - 1); i > 1; i--){
        if(multiple(i, number)){
            return 0;
        }
    }
    return 1;
}

void InitializeArrayOnes(int *array, int size){
    for(int i = 0; i < size; i++){
        array[i] = 1;
    }
}

void PrintArray(int *array, int size){
    for(int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

void PrintPrimes(int *array, int size){
    printf("1 to %d primes\n", size);
    for(int i = 1; i < size; i++){
        if(array[i] == 1){
            printf("%d ", i);
        }
    }
    printf("\n");
}

void exercise3(void){
    #define ARRAY_SIZE 1000
    int numberArray[ARRAY_SIZE];

    InitializeArrayOnes(numberArray, ARRAY_SIZE);

    for(int i = 2; i < ARRAY_SIZE; i++){
        for(int j = i + 1; j < ARRAY_SIZE; j++){
            if ((j % i) == 0){
                numberArray[j] = 0;
            }
        }
    }
   
    PrintPrimes(numberArray, ARRAY_SIZE);
}


/*
    Exercise 5.
    View normal distribution in random processes
*/
void InitializeArray(int *array, int size){
    for(int i = 0; i < size; i++){
        array[i] = 0;
    }
}

void PrintResults(int *array, int size){
    printf("Sum\tTimes\n");
    for(int i = 0; i < size; i++){
        printf("%d\t%d\n", i+2, array[i]);
    }
}

void exercise5(void){
    #define ARRAY_SIZE5 11

    int dice1, dice2, sum;
    int results[ARRAY_SIZE5];

    InitializeArray(results, ARRAY_SIZE5);

    for(int i = 0; i < 3600; i++){
        dice1 = (rand() % 6) + 1;
        dice2 = (rand() % 6) + 1;
        sum = dice1 + dice2;
        results[sum - 2]++;
    }
    
    PrintResults(results, ARRAY_SIZE5);
}


int main(void){
    exercise5();
    return 0;
}