#ifndef ALGO
#define ALGO

#include <vector>
#include "hash.h"
#include "dataset.hpp"
#include <fstream>

class Neighbor{
    // int indexq;
    int index;
    double lshDist;
    unsigned char *image;

    public:
        Neighbor(int, double, unsigned char*);
        void printLshNeighbor(int, double, std::ofstream&);
        void printCubeNeighbor(int, double, std::ofstream&);
        ~Neighbor();
};

int FindW(int, Dataset*);
void ANNsearch(std::vector<Neighbor>&,int, int, unsigned char*, HashTable**);
void RNGsearch(std::vector<Neighbor>&,int, int, unsigned char*, HashTable**);
void trueDistance(std::vector<double>&, int, unsigned char *, Dataset *, HashTable**);



#endif