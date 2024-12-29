#include <stdio.h>

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
    unsigned int p: 1;
};

int main(void){    
    printf("%d\n", sizeof(struct inventory));
    printf("%d\n", sizeof(union data));
    printf("%d\n", sizeof(struct test));
}