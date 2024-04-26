#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  gcc main.c -o main.o && ./main.o

/*
    Exercise 1.
    Using sizeof() operator for user defined types
*/
struct inventory{
    char partName[30];
    int partNumber;
    float price;
    int stock;
    int reorder;
};

union data{
    char c;
    short s;
    long b;
    float f;
    double d;
};

struct address{
    char streetAddress[25];
    char city[20];
    char state[3];
    char zipCode[6];
};

struct student{
    char firstName[15];
    char lastName[15];
    struct address homeAddress;
};

struct test{
    unsigned int a : 1;
    unsigned int b : 1;
    unsigned int c : 1;
    unsigned int d : 1;
    unsigned int e : 1;
    unsigned int f : 1;
    unsigned int g : 1;
    unsigned int h : 1;
    unsigned int i : 1;
    unsigned int j : 1;
    unsigned int k : 1;
    unsigned int l : 1;
    unsigned int m : 1;
    unsigned int n : 1;
    unsigned int o : 1;
    unsigned int p : 1;
};

void exercise1(void){    
    printf("inventory %d\n", sizeof(struct inventory));
    printf("data %d\n", sizeof(union data));
    printf("address %d\n", sizeof(struct address));
    printf("student %d\n", sizeof(struct student));
    printf("test %d\n", sizeof(struct test));
}


/*
    Exercise 2.
    Working with structs and strings
*/
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

void exercise2(void){
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


/*
    Exercise 3.
    Unpack bytes of struct with union
*/
struct StructPack{
    unsigned long word;     //8bytes
    unsigned char byte;     //1byte
    unsigned short hword;   //2bytes
};

void UnpackingBytes( struct StructPack *message, unsigned char *array){
    union{
        unsigned long messageWord;
        unsigned char arrayWord[4];
    } getBytesFromWord = {.messageWord = message->word};

    union{
        unsigned short messagehWord;
        unsigned char arrayhWord[2];
    } getBytesFromhWord = {.messagehWord = message->hword};

    array[0] = getBytesFromWord.arrayWord[0];
    array[1] = getBytesFromWord.arrayWord[1];
    array[2] = getBytesFromWord.arrayWord[2];
    array[3] = getBytesFromWord.arrayWord[3];
    array[4] = message->byte;
    array[5] = getBytesFromhWord.arrayhWord[0];
    array[6] = getBytesFromhWord.arrayhWord[1];
}

void printUCharArray(unsigned char array[], int size){
    for(int i = (size - 1); i >= 0; i--){
        printf("%X", array[i]);
    }
    printf("\n");
}

void exercise3(void){
    unsigned char testArray[7];

    struct StructPack a = {.word = 0x1011121356657887, 
                            .byte = 0x22,
                            .hword = 0x3883};    

    UnpackingBytes(&a, testArray);
    printUCharArray(testArray, 7);
}


/*
    Exercise 4.
    Packing bytes with structs and unions
*/
struct StructPack1{
    unsigned long word1;
    unsigned long word2;
    unsigned char byte;
    unsigned short hword1;
};

void PackingBytes( unsigned char *array, struct StructPack1 *message ){
    union{
        unsigned long word1;
        unsigned char messageWord1[4];
    } packWord1 = {.messageWord1[0] = array[0],
                   .messageWord1[1] = array[1],
                   .messageWord1[2] = array[2],
                   .messageWord1[3] = array[3]
                  };

    union{
        unsigned long word2;
        unsigned char messageWord2[4];
    } packWord2 = {.messageWord2[0] = array[7],
                   .messageWord2[1] = array[8],
                   .messageWord2[2] = array[9],
                   .messageWord2[3] = array[10]
                  };

    union{
        unsigned short hword1;
        unsigned char messagehword1[2];
    } packHword1 = {.messagehword1[0] = array[5],
                    .messagehword1[1] = array[6]
                   };
    
    message->word1 = packWord1.word1;
    message->word2 = packWord2.word2;
    message->hword1 = packHword1.hword1;
    message->byte = array[4];
}

void exercise4(void){
    unsigned char testArray[11] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};
    struct StructPack1 a;

    PackingBytes(testArray, &a);
    printf("word1 = %lX\n", a.word1);
    printf("word2 = %lX\n", a.word2);
    printf("byte = %lX\n", a.byte);
    printf("hword1 = %lX\n", a.hword1);
}

int main(void){
    exercise4();
    return 0;
}