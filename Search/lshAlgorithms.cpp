#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cfloat>
#include <math.h>
#include "dataset.hpp"
#include "metrics.h"
#include "lshAlgorithms.h"

#include "hash.h"

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

void  ANNsearch(int L, int k, int indexq, unsigned char *q, Dataset *trainSet, HashTable** hashTables){
    unsigned int g_hash = 0;
    double min, manh=0.0;
    vector<Neighbor> neighbors;
    vector<double> trueDist;
    for(int i=0; i<L; i++){
        g_hash = hashTables[i]->ghash(q);
        min = DBL_MAX;
        vector<imageInfo>* images = hashTables[i] ->getBucketArray()[g_hash%(hashTables[i] ->gethashTableSize())] -> getImageList();
        for (vector<imageInfo>::iterator it = images->begin() ; it != images->end(); ++it){
            if((*it).ghash == g_hash){
                manh = manhattan((*it).image, q, hashTables[i]->getvectorsDim());
                if(manh < min){
                    min = manh;
                    neighbors.push_back(Neighbor((*it).index, min, (*it).image));
                }
                // if(j>15*L) break;
                // j++;
            }
        }
    }
    //Calculate true distances
    min = DBL_MAX;
    for(int i=0; i<trainSet->getNumberOfImages(); i++){
        manh = manhattan(q, trainSet->imageAt(i),  hashTables[0]->getvectorsDim());
        if(manh<min) {
            min = manh;
            trueDist.push_back(manh);
        }
    }

    int j = trueDist.size()-1;
    int printi = 1;
    cout << "Query: " << indexq << endl;
    if(neighbors.size() > 0){
        for(int i=neighbors.size()-1; i>neighbors.size()-1-k; i--){
            if(j>=0) neighbors[i].printNeighbor(printi, trueDist[j]);
            j--;
            printi++;
        }
        cout << "tLSH: " << 0.0 << endl;
        cout << "tTrue: " << 0.0 << endl;
        cout << "R-near neighbors:" <<endl;
        cout << "???" << endl;
    }    
    return;
}

void RNGsearch(int L, int R, int indexq, unsigned char* q, Dataset *trainSet, HashTable** hashTables){
    unsigned int g_hash = 0;
    double manh=0.0;
    vector<Neighbor> neighbors;
    vector<double> trueDist;
    for(int i=0; i<L; i++){
        g_hash = hashTables[i]->ghash(q);
  
        vector<imageInfo>* images = hashTables[i] ->getBucketArray()[g_hash%(hashTables[i] ->gethashTableSize())] -> getImageList();
        for (vector<imageInfo>::iterator it = images->begin() ; it != images->end(); ++it){
            if((*it).ghash == g_hash){
                manh = manhattan((*it).image, q, hashTables[i]->getvectorsDim());
                if(manh < R){
                    neighbors.push_back(Neighbor((*it).index, manh, (*it).image));
                }
                // if(j>15*L) break;
                // j++;
            }
        }
    }
    //Calculate true distances

    for(int i=0; i<trainSet->getNumberOfImages(); i++){
        manh = manhattan(q, trainSet->imageAt(i),  hashTables[0]->getvectorsDim());
        if(manh<R) {
            trueDist.push_back(manh);
        }
    }

    int j = trueDist.size()-1;
    int printi = 1;
    cout << "Query: " << indexq << endl;
    if(neighbors.size() > 0){
        for(int i=neighbors.size()-1; i>= 0; i--){
            if(j>=0) neighbors[i].printNeighbor(printi, trueDist[j]);
            j--;
            printi++;
        }
        cout << "tLSH: " << 0.0 << endl;
        cout << "tTrue: " << 0.0 << endl;
        cout << "R-near neighbors:" <<endl;
        cout << "???" << endl;
    }    
    return;
}    



Neighbor::Neighbor(int i, double l, unsigned char* img): index(i), lshDist(l){
    image = img;
};

void Neighbor::printNeighbor(int i, double trueDist){
    // cout << "Nearest neighbor-" << i << ": " << index << endl;
    // cout << "distanceLSH: " << lshDist << endl;
    // cout << "distanceTrue: " << trueDist << endl;
};

Neighbor::~Neighbor(){  };
