#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cfloat>
#include <math.h>
#include "dataset.hpp"
#include "metrics.h"
#include "lshAlgorithms.h"
#include "cubeAlgorithms.h"
#include "projection.hpp"
#include "hash.h"

void  ANNsearch(int bucket, int k, int indexq, unsigned char *q, Dataset *trainSet, Projection* projection){
    unsigned int g_hash = 0;
    double min, manh=0.0;
    vector<Neighbor> neighbors;
    vector<double> trueDist;
    g_hash = bucket; /////////////can with loop////
    min = DBL_MAX;
    vector<imageInfo>* images = projection ->getBucketArray()[g_hash] -> getImageList();
    for (vector<imageInfo>::iterator it = images->begin() ; it != images->end(); ++it){
        if((*it).ghash == g_hash){
            manh = manhattan((*it).image, q, projection ->getvectorsDim());
            if(manh < min){
                min = manh;
                neighbors.push_back(Neighbor((*it).index, min, (*it).image));
            }
            // if(j>15*L) break;
            // j++;
        }
    }
    //Calculate true distances
    min = DBL_MAX;
    for(int i=0; i<trainSet->getNumberOfImages(); i++){
        manh = manhattan(q, trainSet->imageAt(i),  projection ->getvectorsDim());
        if(manh<min) {
            min = manh;
            trueDist.push_back(manh);
        }
    }

    int j = trueDist.size()-1;
    int printi = 1;
    cout << "Query: " << indexq << endl;
    for(int i=neighbors.size()-1; i>neighbors.size()-1-k; i--){
        if(j>=0) neighbors[i].printNeighbor(printi, trueDist[j]);
        j--;
        printi++;
    }
    cout << "tLSH: " << 0.0 << endl;
    cout << "tTrue: " << 0.0 << endl;
    cout << "R-near neighbors:" <<endl;
    cout << "???" << endl;
    return;
}

void RNGsearch(int L, int R, unsigned char* q, Dataset *trainSet, HashTable** hashTables){
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
    vector<Neighbor> neighbors;
    double truedist;
    unsigned int g_hash = 0;
    int manh = 0, j = 0;
    for(int i= 0; i < L;i++){
        g_hash = hashTables[i] ->ghash(q);
        vector<imageInfo>* images = hashTables[i] ->getBucketArray()[g_hash%(hashTables[i] ->gethashTableSize())] -> getImageList();
        for (vector<imageInfo>::iterator it = images->begin() ; it != images->end(); ++it){
            if((*it).ghash == g_hash){
                cout << (*it).image << endl;
                manh = manhattan((*it).image, q, hashTables[i] ->getvectorsDim());
                if(manh < R){
                    truedist = truedistance(q, trainSet, hashTables[i]->getvectorsDim());
                    neighbors.push_back(Neighbor((*it).index, manh, (*it).image));
                }
                // if(j>20*L) break;
                // j++;
            }
        }
    }
    int neighbors_limit = neighbors.size();
    for(int i=0; i<neighbors_limit; i++){
        neighbors[i].printNeighbor(i, 0);
    }
    return;
}

void hammingCombinations(int num, vector<unsigned int> & combs){
    // if(distRemain == 0){return;}
    
    int numLength = (int)log2(num)+1;

    int mask = 0;
    unsigned int newComb = 0;
    int bit = 0;
    for(int i = 0; i < numLength; i++){
        mask = 1 << i;
        bit = mask & num;
        if(bit == 0){
            bit = 1;
        }else{
            bit = 0;
        }
        newComb = (num & ~mask) | ((bit << i) & mask);
        //(combs -> at(5)).push_back(newComb);
        combs.push_back(newComb);
        // }else{
        //     hammingCombinations(newComb, combs,distRemain - 1);   
    }
    return;
}


    

void hypercubeSearch(int R, int maxHam, unsigned char* q, Dataset *trainSet, Projection * projection){
    
    unsigned int qhash = projection->ghash(q);
    int qLength = (int)log2(qhash)+1;

    vector<vector<unsigned int>> combs(maxHam); ///////////2d hamming combinatios for every distance

    cout << "distance num :" << combs.size() << endl;
    bool stop = false;
    int i = 0; ///////distance 
    
    while(i < maxHam && !stop){
        
        if(i == 0){
            cout << "first in my bucket qhash" << endl;
            // ANNsearch(qhash, k, int indexq, unsigned char *q, Dataset *trainSet, Projection* projection)
        }else{




        }
        //////////////hamming Distance////////////////////////
        if(i == 0){
            hammingCombinations(qhash, combs[i]);
        }else{
            for (int j = 0; j < combs[i-1].size(); j++)
            {
                hammingCombinations(combs[i-1][j], combs[i]);
            }   
        }
        /////////////////////////////////////////////////////     
        cout << "dim" << i << endl;
        // if(stop){
        //     break;
        // }
        i++; //raise distance
    }    

    
    
    cout << "hypercube" << endl;
    // 1 Project query point to corresponding hypercube vertex.
    // 2 Check points in same vertex and nearby vertices in increasing Hamming
    // distance (=1, then 2, etc), until some threshold reached (next slide).
    // 3 ANN returns closest candidate, range search all p within query ball.


}