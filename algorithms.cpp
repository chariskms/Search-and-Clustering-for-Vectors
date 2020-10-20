#include <stdlib.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <cfloat>
#include "hash.h"
#include "dataset.hpp"
#include "metrics.h"
#include "algorithms.h"

using namespace std;

int FindW(int samples, Dataset *set){
    double sum=0.0, min, tmp;
    int d = set->dimension(), img = set->getNumberOfImages();
    for(int i=0; i<samples; i++){
        min=0.0;
        for(int j=0; j<img; j++){
            if(i!=j){
                if(min!=0.0){
                    tmp = manhattan(set->imageAt(i), set->imageAt(j), d);
                    if(tmp<min) min = tmp;
                }
                else min = manhattan(set->imageAt(i), set->imageAt(j), d);
            }
        }
        sum+=min;
    }
    return sum/samples;
}

void ANNsearch(int L, int k, int buckets, unsigned char *q, HashTable** hashTables){
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

    int g_hash = 0, j;
    double min, manh=0.0, truedist=0.0;
    vector<Neighbor> neighbors;
    for(int i=0; i<L; i++){
        g_hash = hashTables[i]->ghash(q);
        j=0;
        min = DBL_MAX;
        vector<unsigned char *>* images = hashTables[i]->getBucketArray()[g_hash%buckets]->getImageList();
        for (vector<unsigned char *>::iterator it = images->begin() ; it != images->end(); ++it){
            manh = manhattan(*it, q, hashTables[i]->getvectorsDim());
            if(manh < min){
                min = manh;
                truedist = truedistance(*it, q, hashTables[i]->getvectorsDim());
                neighbors.push_back(Neighbor(i, min, truedist, *it));
            }
            if(j>15*L) break;
            j++;
        }
    }

    //print vector
    int neighbors_limit = k;
    if(neighbors.size()<k) neighbors_limit = neighbors.size();
    for(int i=0; i<neighbors_limit; i++){
        neighbors[i].printNeighbor(i);
    }
}

void RNGsearch(int R,int L, Dataset* query, HashTable** hashTables){
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
    unsigned char* q = query->imageAt(0);
    int bucket = 0;
    for(int i= 0; i < L;i++){
        bucket = hashTables[i] ->ghash(q);
        vector<unsigned char *>* images = hashTables[i] ->getBucketArray()[bucket] -> getImageList();
        for (vector<unsigned char *>::iterator it = images->begin() ; it != images->end(); ++it){
            cout << *it << endl;
            if(manhattan(*it, q, hashTables[i] ->getvectorsDim()) < R){
                cout << "Found one item" << endl;
            }
        }
    }
}


Neighbor::Neighbor(int i, double l, double t, unsigned char* img): index(i), lshDist(l), trueDist(t){
    image = img;
};

void Neighbor::printNeighbor(int i){
    cout << "Nearest neighbor-" << i << ": " << index << endl;
    cout << "distanceLSH: " << lshDist << endl;
    cout << "distanceTrue: " << trueDist << endl;
    cout << endl;
}

Neighbor::~Neighbor(){  }
