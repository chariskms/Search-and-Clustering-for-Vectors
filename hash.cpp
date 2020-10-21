#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <random>
#include "hash.h"

using namespace std;

// class HashFunction
HashFunction::HashFunction(int d, int w, int k = 1): d(d), w(w), k(k){
    static default_random_engine generator;
    uniform_int_distribution<int> distribution(0,w-1);
    // uniform_real_distribution<int> distribution(0,w-1);
    //this->M = pow(2, floor(32/k));
    this-> M = 1 << (32/k);
    this->m = 1 << (M/4);    //maxi ai < m < M/2
    s_numbers = new int[d];
    for(int i=0; i<d; i++){
        s_numbers[i] = distribution(generator);
    }
}

HashFunction::~HashFunction(){
    delete[] s_numbers;
}

int HashFunction::hash(unsigned char* x){
    int h=0, a=0, start_m=m, tmp_m=1;
    for(int i=0; i<d; i++){
        a = floor((double)(((int)x[d-i-1]) - s_numbers[d-i-1])/w);
        // Calculate a mod M for a<0 or a>=0
        if (a<0) a = (a%M+M)%M;
        else a = a%M;

        if(i>0){
            h+=((tmp_m%M)*a)%M;
            tmp_m=((tmp_m%M)*(start_m%M))%M;
        }
        else h+=a;
    }
    
    return h%M;
    
}

// class Bucket
Bucket::Bucket(int value){
    bucketValue = value;
}

void Bucket::addImage(int index,int ghash,unsigned char * image){
    this->images.push_back (imageInfo(index, ghash, image));
}

imageInfo* Bucket::popBackImage(){
    imageInfo* image = NULL;
    if(! this->images.empty()){
        image = &(this->images.back());
        this->images.pop_back();
    }
    return image;
}

// class HashTable
HashTable::HashTable(int v, int hT, int K, int W): vectorsDim(v), hashTableSize(hT), numberOfHashFuncs(K){
    hashFunctions = new HashFunction*[this->numberOfHashFuncs];
    for(int i=0; i<this->numberOfHashFuncs; i++){
        
        hashFunctions[i] = new HashFunction(vectorsDim, W, this->numberOfHashFuncs);
    }
    bucketArray = new Bucket*[hashTableSize];
    for(int i=0; i<this->hashTableSize; i++){
        bucketArray[i] = new Bucket(i);
        
    }

}

HashTable::~HashTable(){
    for(int i=0; i<this->numberOfHashFuncs; i++) delete hashFunctions[i];
    delete[] hashFunctions;

    for(int i=0; i<this->hashTableSize; i++) delete bucketArray[i];
    delete[] bucketArray;
}

unsigned int HashTable::ghash(unsigned char *image){
    int shift = floor(32/numberOfHashFuncs);
    unsigned int concat = hashFunctions[0]->hash(image);
    unsigned int M = hashFunctions[0]->getM();
    unsigned int hash = 0;
    for(int i=1; i<numberOfHashFuncs; i++){
        unsigned hash = hashFunctions[i]->hash(image);
        concat = (concat << shift) | hash;
    }
    return concat;
}
