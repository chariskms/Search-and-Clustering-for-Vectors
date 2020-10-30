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

void hyperTrueDistance(vector<double>& trueDist, int R, int indexq, unsigned char *q, Dataset *trainSet,  Projection* projection){
    double min, manh=0.0;
    
    if(R > 0){
        for(int i=0; i<trainSet->getNumberOfImages(); i++){
            manh = manhattan(q, trainSet->imageAt(i),  projection ->getvectorsDim());
            if(manh<R) {
                trueDist.push_back(manh);
            }
        }    
    }else{
        min = DBL_MAX;
        for(int i=0; i<trainSet->getNumberOfImages(); i++){
            manh = manhattan(q, trainSet->imageAt(i),  projection ->getvectorsDim());
            if(manh<min) {
                min = manh;
                trueDist.push_back(manh);
            }
        }
    }    

    return;

}
void hyperCubeRNGsearch(vector<Neighbor>& neighbors, int bucket, double R, int indexq, unsigned char *q, Dataset *trainSet, Projection* projection){
    unsigned int g_hash = 0;
    double manh=0.0;
    g_hash = bucket; /////////////can with loop////

    vector<imageInfo>* images = projection ->getBucketArray()[g_hash%(projection ->gethashTableSize())] -> getImageList();
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
    return;
}    

void hyperCubeANNsearch(vector<Neighbor>& neighbors, int bucket, int k, int indexq, unsigned char *q, Dataset *trainSet, Projection* projection){
    unsigned int g_hash = 0;
    double min, manh=0.0;

    
    g_hash = bucket; /////////////can with loop////
    min = DBL_MAX;
    vector<imageInfo>* images = projection ->getBucketArray()[g_hash%(projection ->gethashTableSize())] -> getImageList();
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


    

void hyperCubeSearch(int R,int N, int probs,int indexq, unsigned char* q, Dataset *trainSet, Projection * projection){
    
    unsigned int qhash = projection->ghash(q);
    int qLength = (int)log2(qhash)+1;

    vector<vector<unsigned int>> combs(probs); ///////////2d hamming combinatios for every distance

    //cout << "distance num :" << combs.size() << endl;
    bool stop = false;
    int i = 0; ///////distance 
    
    while(i < probs && !stop){
        
        if(i == 0){
            vector<Neighbor> ANNneighbors;
            vector<Neighbor> RNGneighbors;
            vector<double> ANNtrueDist;
            vector<double> RNGtrueDist;
            //cout << "first in my bucket qhash :" << qhash << endl;
            hyperCubeANNsearch(ANNneighbors,qhash, projection -> getnumberOfHashFuncs(), indexq, q, trainSet, projection);
            hyperCubeRNGsearch(RNGneighbors, qhash, R, indexq, q,trainSet,projection);
            hyperTrueDistance(ANNtrueDist,0,indexq, q, trainSet, projection);
            hyperTrueDistance(RNGtrueDist,R,indexq, q, trainSet, projection);

            int j = ANNtrueDist.size()-1;
            int printn = 1;
            cout << "Query: " << indexq << endl;
            if(ANNneighbors.size() > ANNneighbors.size()-1-N){
                for(int n=ANNneighbors.size()-1; n>ANNneighbors.size()-1-N; n--){
                    if(j>=0) ANNneighbors[n].printNeighbor(printn, ANNtrueDist[j]);
                    j--;
                    printn++;
                }
                cout << "tHyperCube: " << 0.0 << endl;
                cout << "tTrue: " << 0.0 << endl<< endl;
            }
            j = RNGtrueDist.size()-1;
            printn = 1;
            cout << "R-near neighbors:" <<endl;
            if(RNGneighbors.size() > 0){
                for(int n=RNGneighbors.size()-1; n>= 0; n--){
                    if(j>=0) RNGneighbors[n].printNeighbor(printn, RNGtrueDist[j]);
                    j--;
                    printn++;
                }
            } 

        }else{
            for (int z = 0; z < combs[i-1].size(); z++){
                //search
                vector<Neighbor> ANNneighbors;
                vector<Neighbor> RNGneighbors;
                vector<double> ANNtrueDist;
                vector<double> RNGtrueDist;

                hyperCubeANNsearch(ANNneighbors,combs[i-1][z], projection -> getnumberOfHashFuncs(), indexq, q, trainSet, projection);
                hyperCubeRNGsearch(RNGneighbors, combs[i-1][z], R, indexq, q,trainSet,projection);
                hyperTrueDistance(ANNtrueDist,0,indexq, q, trainSet, projection);
                hyperTrueDistance(RNGtrueDist,R,indexq, q, trainSet, projection);

                int j = ANNtrueDist.size()-1;
                int printn = 1;
                cout << "Query: " << indexq << endl;
                if(ANNneighbors.size() > ANNneighbors.size()-1-N){
                    for(int n=ANNneighbors.size()-1; n>ANNneighbors.size()-1-N; n--){
                        if(j>=0) ANNneighbors[n].printNeighbor(printn, ANNtrueDist[j]);
                        j--;
                        printn++;
                    }
                    cout << "tHyperCube: " << 0.0 << endl;
                    cout << "tTrue: " << 0.0 << endl<< endl;
                }
                j = RNGtrueDist.size()-1;
                printn = 1;
                cout << "R-near neighbors:" <<endl;
                if(RNGneighbors.size() > 0){
                    for(int n=RNGneighbors.size()-1; n>= 0; n--){
                        if(j>=0) RNGneighbors[n].printNeighbor(printn, RNGtrueDist[j]);
                        j--;
                        printn++;
                    }
                } 

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