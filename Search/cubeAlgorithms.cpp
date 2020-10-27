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

void  hyperCubeANNsearch(int bucket, int k, int indexq, unsigned char *q, Dataset *trainSet, Projection* projection){
    unsigned int g_hash = 0;
    double min, manh=0.0;
    vector<Neighbor> neighbors;
    vector<double> trueDist;
    g_hash = bucket; /////////////can with loop////
    min = DBL_MAX;
    vector<imageInfo>* images = projection ->getBucketArray()[g_hash] -> getImageList();
    for (vector<imageInfo>::iterator it = images->begin() ; it != images->end(); ++it){
        //cout << "exw photos " << endl;
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
    //cout << "Query: " << indexq <<"neighbors :" <<neighbors.size()<< endl;
    if(neighbors.size() > 0){
        for(int i=neighbors.size()-1; i>neighbors.size()-1-k; i--){
            if(j>=0) neighbors[i].printNeighbor(printi, trueDist[j]);
            j--;
            printi++;
        }
        // cout << "tLSH: " << 0.0 << endl;
        // cout << "tTrue: " << 0.0 << endl;
        // cout << "R-near neighbors:" <<endl;
        // cout << "???" << endl;
    }    
    return;
}

void hyperCubeRNGsearch(int bucket, double R, int indexq, unsigned char *q, Dataset *trainSet, Projection* projection){
    unsigned int g_hash = 0;
    double manh=0.0;
    vector<Neighbor> neighbors;
    vector<double> trueDist;
    g_hash = bucket; /////////////can with loop////

    vector<imageInfo>* images = projection ->getBucketArray()[g_hash] -> getImageList();
    for (vector<imageInfo>::iterator it = images->begin() ; it != images->end(); ++it){
        //cout << "exw photos " << endl;
        if((*it).ghash == g_hash){
            manh = manhattan((*it).image, q, projection ->getvectorsDim());
            if(manh < R){
                neighbors.push_back(Neighbor((*it).index, manh, (*it).image));
            }
            // if(j>15*L) break;
            // j++;
        }
    }
    //Calculate true distances
    for(int i=0; i<trainSet->getNumberOfImages(); i++){
        manh = manhattan(q, trainSet->imageAt(i),  projection ->getvectorsDim());
        if(manh<R) {
            trueDist.push_back(manh);
        }
    }
    int j = trueDist.size()-1;
    int printi = 1;
    //cout << "Query: " << indexq << endl;
    if(neighbors.size() > 0){
        for(int i=neighbors.size()-1; i>= 0; i--){
            if(j>=0) neighbors[i].printNeighbor(printi, trueDist[j]);
            j--;
            printi++;
        }
        // cout << "tLSH: " << 0.0 << endl;
        // cout << "tTrue: " << 0.0 << endl;
        // cout << "R-near neighbors:" <<endl;
        // cout << "???" << endl;
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


    

void hyperCubeSearch(int R, int probs,int indexq, unsigned char* q, Dataset *trainSet, Projection * projection){
    
    unsigned int qhash = projection->ghash(q);
    int qLength = (int)log2(qhash)+1;

    vector<vector<unsigned int>> combs(probs); ///////////2d hamming combinatios for every distance

    //cout << "distance num :" << combs.size() << endl;
    bool stop = false;
    int i = 0; ///////distance 
    
    while(i < probs && !stop){
        
        if(i == 0){
            //cout << "first in my bucket qhash :" << qhash << endl;
            hyperCubeANNsearch(qhash, projection -> getnumberOfHashFuncs(), indexq, q, trainSet, projection);
            // search
        }else{
            for (int j = 0; j < combs[i-1].size(); j++){
                //search
                hyperCubeANNsearch(combs[i-1][j], projection -> getnumberOfHashFuncs(), indexq, q, trainSet, projection);
            }
        }
        //////////////hamming Distance////////////////////////
        if(i == 0){
            hammingCombinations(qhash, combs[i]);
        }else{
            for (int j = 0; j < combs[i-1].size(); j++){
                hammingCombinations(combs[i-1][j], combs[i]);
            }   
        }
        /////////////////////////////////////////////////////     
        //cout << "dim" << i << endl;
        // if(stop){
        //     break;
        // }
        i++; //raise distance
    }    
}