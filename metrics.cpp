#include <stdlib.h>
#include <math.h>
#include "metrics.h"

double manhattan(unsigned char* x, unsigned char* y, int d){
    double sum = 0.0;
    for(int i=0; i<d; i++){
        sum = sum + fabs(x[i]-y[i]);
    }
    return sum;
}

double truedistance(unsigned char* x, unsigned char* y, int d){
    double sum = 0.0;
    for(int i=0; i<d; i++){
        sum = sum + pow(fabs(x[i]-y[i]), 2.0);
    }
    return pow(sum, 0.5);
}
