#ifndef ALGO
#define ALGO

#include <vector>
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
void ANNsearch(std::vector<Neighbor>&,int, int, int, unsigned char*, Dataset*, HashTable**);
void RNGsearch(std::vector<Neighbor>&,int, int, int, unsigned char*, Dataset*, HashTable**);
void trueDistance(std::vector<double>&, int, int, unsigned char *, Dataset *, HashTable**);



#endif