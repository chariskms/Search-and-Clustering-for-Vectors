#include <stdlib.h>
#include <math.h>
#include <cfloat>
#include "metrics.h"

double manhattan(unsigned char* x, unsigned char* y, int d){
    double sum = 0.0;
    for(int i=0; i<d; i++){
        sum = sum + fabs(x[i]-y[i]);
    }
    return sum;
}

double truedistance(unsigned char* x, Dataset *y, int d){
    double min = DBL_MAX, manh = 0.0;
    int images = y->getNumberOfImages();
    for(int i=0; i<images; i++){
        manh = manhattan(x, y->imageAt(i), d);
        if(manh<min) min = manh;
    }
    return min;
}
