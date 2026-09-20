#include "sin_table.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

#define TESTVAL 0.005

int benchmark(){
    volatile long double d;
    int i, j;
    double time1, time2, time3, time4;


    printf("Running benchmark!\n");

    printf("The table occupies %u kB of static memory (rounded down)\n", (sizeof(typeof(*SinTable)) * (SIN_TABLE_SIZE + 1) ) >> 10 );

    printf("Sin of %lf according to sin_table: %lf\n", TESTVAL,  (double)sin_table(TESTVAL));
    printf("Sin of %lf according to sin: %lf\n", TESTVAL, (double)sin(TESTVAL));

    printf("Sin of %lf according to sin_table: %lf\n", (PI/2) - TESTVAL,  (double)sin_table( (PI/2) - TESTVAL));
    printf("Sin of %lf according to sin: %lf\n", (PI/2) - TESTVAL, (double)sin( (PI/2) - TESTVAL));

    printf("Lowest angle value that represents a change in table index: %.32lf\n", LOWEST_ANGLE_DELTA);

    clock_t timeStart = clock();

    j = 0;
    for( i = 0xF0000000; i < 0; i++){
        j++;
        d = sin_table( 0xF0000000 / (double)i);
    }

    time1 = (double)(clock() - timeStart) / CLOCKS_PER_SEC;
    printf("\nsin_table function called %u times in: %lf s\n", j, time1);


    j = 0;
    for( i = 0xF0000000; i < 0; i++){
        j++;
        d = sinl( 0xF0000000 / (double)i);
    }

    time2 = (double)(clock() - timeStart) / CLOCKS_PER_SEC;
    printf("\nmath sinl function called %u times in: %lf s\n", j, time2 - time1);


    j = 0;
    for( i = 0xF0000000; i < 0; i++){
        j++;
        d = sin( 0xF0000000 / (double)i);
    }
    time3 = (double)(clock() - timeStart) / CLOCKS_PER_SEC;
    printf("\nmath sin function called %u times in: %lf s\n", j, time3 - time2);


    j = 0;
    for( i = 0xF0000000; i < 0; i++){
        j++;
        d = sinf( 0xF0000000 / (double)i);
    }
    time4 = (double)(clock() - timeStart) / CLOCKS_PER_SEC;
    printf("\nmath sinf function called %u times in: %lf s\n", j, time4 - time3);

    printf("\nTotal: %lf s\n", time4);
    return 0;
}

int main() {
    return benchmark();
}
