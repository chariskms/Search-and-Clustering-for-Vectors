#include <vector>
#include <iostream>
#include <stdlib.h>
class HashFunction{
    int * s_numbers;
    int d; //d diastasi 
    int w; //w window 
    int M; //M = 2^(32/k)
    int m; //m = 2^(32-3) ,must m > max_xi
    int k; //k number of hashFunctions

    public:

        HashFunction(int, int, int);
        ~HashFunction();
        int hash(unsigned char*);

};

class Bucket{
    int bucketValue;
    std::vector<unsigned char *> images;

    public:
        Bucket(int);
        void addImage(unsigned char *);
        unsigned char * popBackImage();
};

class HashTable{
    int vectorsDim;
    int hashTableSize;      //n/16 number of buckets 
    int numberOfHashFuncs; //k
    HashFunction** hashFunctions;
    Bucket** bucketArray;

    public:
        HashTable(int, int, int, int);
        ~HashTable();
        HashFunction** getHashFunctions(){return hashFunctions;}
        Bucket** getBucketArray(){return bucketArray;}
        int ghash();
};        

