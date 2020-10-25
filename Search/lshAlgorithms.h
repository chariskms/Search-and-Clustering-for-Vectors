#ifndef ALGO
#define ALGO

#include "hash.h"
#include "dataset.hpp"

class Neighbor{
    // int indexq;
    int index;
    double lshDist;
    unsigned char *image;

    public:
        Neighbor(int, double, unsigned char*);
        void printNeighbor(int, double);
        ~Neighbor();
};

int FindW(int, Dataset*);
void ANNsearch(int, int, int, unsigned char*, Dataset*, HashTable**);
void RNGsearch(int, int, unsigned char*, Dataset*, HashTable**);



#endif