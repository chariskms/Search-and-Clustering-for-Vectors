#include "hash.h"
#include <stdlib.h>  
#include <time.h>
#include <cmath> 
#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;

// class HashFunction

HashFunction::HashFunction(int d, int w, int k = 1){
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

HashFunction::~HashFunction(){
    delete[] s_numbers;
}

int HashFunction::hash(unsigned char* x){
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

// class Bucket

Bucket::Bucket(int value){
    bucketValue = value;
}

void Bucket::addImage(unsigned char * image){
    this->images.push_back (image);
}

unsigned char * Bucket::popBackImage(){
    unsigned char * image = NULL;
    if(! this->images.empty()){
        image = this->images.back();
        this->images.pop_back();
    } 
    return image;   
}

// class HashTable

HashTable::HashTable(int vectorsDimArg, int hashTableSizeArg, int K, int W){
    this -> vectorsDim = vectorsDimArg;
    this -> hashTableSize = hashTableSizeArg;
    this -> numberOfHashFuncs = K;
    hashFunctions = new HashFunction*[this -> numberOfHashFuncs];

    for(int i=0; i< this -> numberOfHashFuncs; i++){
        hashFunctions[i] = new HashFunction(vectorsDim, W, this -> numberOfHashFuncs);
    }
    
    bucketArray = new Bucket*[hashTableSize];
    for(int i=0; i< this -> hashTableSize; i++){
        bucketArray[i] = new Bucket(i);
    }

}

HashTable::~HashTable(){
    for(int i=0; i< this -> numberOfHashFuncs; i++){
        delete hashFunctions[i];
    }
    delete[] hashFunctions;
    
    for(int i=0; i< this -> hashTableSize; i++){
        delete bucketArray[i];
    }
    delete[] bucketArray;
}

int HashTable::ghash(){
    cout << "g concat under construction!" << endl;
    return 0;
}

