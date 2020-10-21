#include <map>
#include <hash.h>

class Projection{
    int bucketDim;
    std::map<int, bool> hStorage;

    Projection(int);
    int f(HashFunction**);
    
};