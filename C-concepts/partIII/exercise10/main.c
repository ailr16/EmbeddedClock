#include <stdio.h>

/*  Year    Population(billion)     Increment
    2011    7.07                    -
    2012    7.16                    0.09
    2013    7.25                    0.09
    2014    7.34                    0.09
    2015    7.43                    0.09
    2016    7.51                    0.08
    2017    7.60                    0.09
    2018    7.68                    0.08
    2019    7.76                    0.08
    2020    7.84                    0.08
    2021    7.91                    0.07
                                    AVG 0.084
    population(year)=rate*year+b
    b = population - rate * year
    year = (population - b) / rate

*/

int main(void){
    float rate = 0.084;
    float b = 7.91 - rate * 2021;

    float actualPopulation = 8.03;
    int yearDoublePopulation;

    printf("Year\tPopulation\tIncrement\n");

    for(int year=1; year<76; year++){
        printf("%d\t%f\t%f\n", year, rate*(2021+year)+b, (rate*(2021+year)+b) - (rate*(2020+year)+b));
    }

    yearDoublePopulation = (actualPopulation*2 - b) / rate;
    printf("in %d population will double the current's year (%f vs %f)\n", yearDoublePopulation, actualPopulation, actualPopulation*2);
}