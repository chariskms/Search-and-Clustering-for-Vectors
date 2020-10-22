#include <map>
#include "hash.h"

using namespace std;

class Projection : public HashTable {
    // int bucketDim;
    // int vectorsDim;
    // HashFunction **hashFunctions;
    map<int, int>* hStorage;

    Projection(int, int, int, int);
    ~Projection();
    unsigned int ghash(unsigned char*);
    
};