#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "hash.h"

using namespace std;

// class HashFunction
HashFunction::HashFunction(int d, int w, int k = 1): d(d), w(w), k(k){
    srand(time(NULL));
    this->M = pow(2, floor(32/k));
    this->m = rand() % (M/2);   //maxi ai < m < M/2
    s_numbers = new int[d];
    for(int i=0; i<d; i++){
        s_numbers[i] = rand() % w;
    }
}

HashFunction::~HashFunction(){
    delete[] s_numbers;
}

int HashFunction::hash(unsigned char* x){
    int h=0, a=0, start_m=m;
    for(int i=0; i<d; i++){
        a = floor((double)(((int)x[d-i-1]) - s_numbers[d-i-1])/w);
        if(i>0){
            h+=((m%M)*((a%M+M)%M))%M;
            m=((m%M)*(start_m%M))%M;    //m=((mprev%M)*(m%M))%M;
        }
        else h+=(a%M);
    }
    return h%M;
}

// class Bucket
Bucket::Bucket(int value){
    bucketValue = value;
}

void Bucket::addImage(unsigned char * image){
    this->images.push_back (image);
}

unsigned char* Bucket::popBackImage(){
    unsigned char * image = NULL;
    if(! this->images.empty()){
        image = this->images.back();
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

int HashTable::ghash(unsigned char * image){
    int shift = floor(32/numberOfHashFuncs);
    int concat = hashFunctions[0]->hash(image);
    cout << "h_0(x): " << concat << endl;
    int hash = 0;
    for(int i=1; i<numberOfHashFuncs; i++){
        hash = hashFunctions[i]->hash(image);
        cout << "h_" << i << "(x): " << hash << endl;
        concat = (concat << shift) | hash;
    }
    return concat;
}

void HashTable::ANNsearch(unsigned char * image){
    // Approximate kNN
    // Input: query q
    // Let b ←Null; db ← ∞; initialize k best candidates and distances;
    // for i from 1 to L do
    //     for each item p in bucket gi(q) do
    //         if dist(q, p) < db = k-th best distance then b ← p; db ← dist(q, p)
    //         end if
    //         if large number of retrieved items (e.g. > 10L) then return b // optional
    //         end if
    //     end for
    //     return b; k best candidates;
    // end for
}

void HashTable::RNGsearch(){
    // Approximate (r, c) Range search
    // Input: r, query q
    //     for i from 1 to L do
    //         for each item p in bucket gi(q) do
    //             if dist(q, p) < r then output p
    //             end if
    //             if large number of retrieved items (e.g. > 20L) then return // optional
    //             end if
    //         end for
    //     end for
    // return
}
