#include <stdio.h>
#include <string.h>

struct customer{
    char lastName[15];
    char firstName[15];
    int customerNumber;
    struct{
        char phoneNumber[11];
        char address[50];
        char city[15];
        char state[3];
        char zipCode[6];
    } personal;
} customerRecord, *customerPtr;

int main(void){
    customerPtr = &customerRecord;
    strcpy(customerRecord.lastName, "Last");
    strcpy(customerRecord.firstName, "First");
    customerRecord.customerNumber = 16;
    strcpy(customerRecord.personal.phoneNumber, "1234567890");
    strcpy(customerRecord.personal.address, "Fake St");
    strcpy(customerRecord.personal.city, "City");
    strcpy(customerRecord.personal.state, "St");
    strcpy(customerRecord.personal.zipCode, "ZIP");
    
    printf("a) %s\n", customerRecord.lastName);
    printf("b) %s\n", customerPtr->lastName);
    printf("c) %s\n", customerRecord.firstName);
    printf("d) %s\n", customerPtr->firstName);
    printf("e) %d\n", customerRecord.customerNumber);
    printf("f) %d\n", customerPtr->customerNumber);
    printf("g) %s\n", customerRecord.personal.phoneNumber);
    printf("h) %s\n", customerPtr->personal.phoneNumber);
    printf("i) %s\n", customerRecord.personal.address);
    printf("j) %s\n", customerPtr->personal.address);
    printf("k) %s\n", customerRecord.personal.city);
    printf("l) %s\n", customerPtr->personal.city);
    printf("m) %s\n", customerRecord.personal.state);
    printf("n) %s\n", customerPtr->personal.state);
    printf("o) %s\n", customerRecord.personal.zipCode);
    printf("p) %s\n", customerPtr->personal.zipCode);
}