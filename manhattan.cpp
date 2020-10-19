#include <stdlib.h>
#include <math.h> 
#include "manhattan.h"

double manhattan(unsigned char* x, unsigned char* y, int d){
    double sum = 0;
    for(int i=0; i<d; i++){
        sum = sum + fabs(x[i]-y[i]);
    }
    return sum;
}
