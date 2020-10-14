#include "hash.h"
#include <stdlib.h>  
#include <time.h>
#include <cmath> 


hashFunction::hashFunction(int d, int w, int k = 1){
    this -> d = d;
    this -> w = w;
    this -> k = k;
    this -> M = pow(2, floor(32/(this -> k)));
    this -> m = pow(2, 32 - 3);
    s_numbers = new int[d];

    srand (time(NULL));
    for(int i = 0;i < d;i++){
        s_numbers[i] = rand() % w;
    }


}

hashFunction::~hashFunction(){
    delete[] s_numbers;
}

int hashFunction::hash(unsigned char* x){
    int h = 0;
    int a = 0;
    for(int i =0; i<d;i++){
        a = abs(x[i] - s_numbers[i]);
        a = floor(a/w);
        a = a*pow(this -> m, d-i-1);
        h = h%M + a%M;
    }
    h = h%M;
    return h;

}