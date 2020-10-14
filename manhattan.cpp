#include <stdlib.h> 

int manhattan(unsigned char* x, unsigned char* y, int d){
    int sum = 0;
    for(int i=0; i < d; i++){
        sum = sum + abs(x[i]-y[i]);
    }
    return sum;
}