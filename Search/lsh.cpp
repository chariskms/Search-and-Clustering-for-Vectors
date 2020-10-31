#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <cmath>
#include "hash.h"
#include "dataset.hpp"
#include "lshAlgorithms.h"
#define SWAP_INT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))
// ./lsh -d train-images.idx3-ubyte -R 1.0 -q fileq -k 4 -L 5 -o fileo -N 1

using namespace std;

int main(int argc, char** argv){
    if (argc>6 && argc<16){
        char *d=NULL, *q=NULL, *o=NULL, *k=NULL, *l=NULL, *n=NULL, *r=NULL;
        double R=1.0, exec_time;
        int K=4, L=5, N=1;
        for (int i = 0; i<argc; i++){
            if (!strcmp("-d", argv[i])) d = (char*)argv[i+1];   /* -inputfile */
            if (!strcmp("-q", argv[i])) q = (char*)argv[i+1];   /* -queryfile */
            if (!strcmp("-k", argv[i])) k = (char*)argv[i+1];   /* -k */
            if (!strcmp("-L", argv[i])) l = (char*)argv[i+1];   /* -L */
            if (!strcmp("-o", argv[i])) o = (char*)argv[i+1];   /* -o */
            if (!strcmp("-N", argv[i])) n = (char*)argv[i+1];   /* -N */
            if (!strcmp("-R", argv[i])) r = (char*)argv[i+1];   /* -R */
        }

        if(d==NULL || q==NULL || o==NULL){
            cout << "You must run the program with parameters(REQUIRED): –d <input file> –q <query file>" << endl;
            cout << "With additional parameters: –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>" << endl;
            return(0);
        }
        else{
            if (k!=NULL) K = atoi(k);
            if (l!=NULL) L = atoi(l);
            if (n!=NULL) N = atoi(n);
            if (r!=NULL) R = atof(r);

            /* PROGRAM STARTS HERE */
            clock_t tStart = clock();

            int magicNumber = 0,numberOfImages = 0,numberOfRows = 0,numberOfColumns = 0, img=0;
            int numOfpixels;

            //Open train file
            fstream trainInput(d);
            if(!trainInput.is_open()){
                cerr<<"Failed to open input data."<<endl;
                return(0);
            }
            trainInput.read((char*)&magicNumber, 4);
            trainInput.read((char*)&numberOfImages, 4);
            trainInput.read((char*)&numberOfRows, 4);
            trainInput.read((char*)&numberOfColumns, 4);

            //Convert intergers from Big Endian to Little Endian
            magicNumber = SWAP_INT32(magicNumber);
            numberOfImages = SWAP_INT32(numberOfImages);
            numberOfRows = SWAP_INT32(numberOfRows);
            numberOfColumns = SWAP_INT32(numberOfColumns);
            Dataset trainSet(magicNumber, numberOfImages, numberOfColumns, numberOfRows);
            trainInput.read((char*)trainSet.imageAt(0), (trainSet.getNumberOfPixels())*(trainSet.getNumberOfImages()));
            trainInput.close();

            img = numberOfImages;

            //Open query file
            fstream queryInput(q);
            if(!queryInput.is_open()){
                cerr<<"Failed to open input data."<<endl;
                return 0;
            }
            queryInput.read((char*)&magicNumber, 4);
            queryInput.read((char*)&numberOfImages, 4);
            queryInput.read((char*)&numberOfRows, 4);
            queryInput.read((char*)&numberOfColumns, 4);

            //Convert intergers from Big Endian to Little Endian
            magicNumber = SWAP_INT32(magicNumber);
            numberOfImages = SWAP_INT32(numberOfImages);
            numberOfRows = SWAP_INT32(numberOfRows);
            numberOfColumns = SWAP_INT32(numberOfColumns);
            Dataset querySet(magicNumber, numberOfImages, numberOfColumns, numberOfRows);
            queryInput.read((char*)querySet.imageAt(0), (querySet.getNumberOfPixels())*(querySet.getNumberOfImages()));
            queryInput.close();


            ///////////////////////////////////////structure test///////////////////////////////////////
            int bucketsNumber = floor(trainSet.getNumberOfImages()/16);

            // int W = FindW(img, &trainSet);
            // cout << "W is " << W << endl;
            int W = 40000;
            HashFunction** hashFamily = new HashFunction*[trainSet.getNumberOfPixels()];
            for(int i = 0; i < trainSet.getNumberOfPixels();i++){
                hashFamily[i] = NULL;
            }

            HashTable **hashTables = new HashTable*[L];
            for(int i=0; i<L; i++){
                hashTables[i] = new HashTable(trainSet.getNumberOfPixels(),bucketsNumber, K,W,hashFamily);

                for(int j=0; j<img; j++){
                    unsigned int g_hash = (unsigned int)(hashTables[i]->ghash(trainSet.imageAt(j)));
                    hashTables[i]->getBucketArray()[g_hash%bucketsNumber]->addImage(j,g_hash,trainSet.imageAt(j));
                }
            }

            ofstream outputf("out");
            if (!outputf.is_open()){
                cerr<<"Failed to open output data."<<endl;
                return 0;
            }
            clock_t lshAnnStart, lshRngStart, AnnTrueStart, RngTrueStart;
            double lshAnnTime, lshRngTime, trueAnnTime, trueRngTime;
            for(int index = 0; index < 10;index++){
                vector<Neighbor> ANNneighbors;
                vector<Neighbor> RNGneighbors;
                vector<double> ANNtrueDist;
                vector<double> RNGtrueDist;

                lshAnnStart = clock();
                ANNsearch(ANNneighbors,L, N, index, querySet.imageAt(index), &trainSet, hashTables);
                lshAnnTime = (double)(clock() - lshAnnStart)/CLOCKS_PER_SEC;

                AnnTrueStart = clock();
                trueDistance(ANNtrueDist, 0,index, querySet.imageAt(index), &trainSet,hashTables);
                trueAnnTime = (double)(clock() - AnnTrueStart)/CLOCKS_PER_SEC;

                lshRngStart = clock();
                RNGsearch(RNGneighbors, L, R, index, querySet.imageAt(index),&trainSet, hashTables);
                lshRngTime = (double)(clock() - lshRngStart)/CLOCKS_PER_SEC;

                RngTrueStart = clock();
                trueDistance(RNGtrueDist, R,index, querySet.imageAt(index), &trainSet,hashTables);
                trueRngTime = (double)(clock() - RngTrueStart)/CLOCKS_PER_SEC;

                int j = ANNtrueDist.size()-1;
                int printi = 1;
                outputf << "Query: " << index << endl;
                if(ANNneighbors.size() > ANNneighbors.size()-1-N){
                    for(int i=ANNneighbors.size()-1; i>ANNneighbors.size()-1-N; i--){
                        if(j>=0) ANNneighbors[i].printLshNeighbor(printi, ANNtrueDist[j], outputf);
                        j--;
                        printi++;
                    }
                    outputf << "tLSH: " << lshAnnTime << endl;
                    outputf << "tTrue: " << trueAnnTime<< endl<< endl;
                }
                j = RNGtrueDist.size()-1;
                printi = 1;
                outputf << "R-near neighbors:" <<endl;
                if(RNGneighbors.size() > 0){
                    for(int i=RNGneighbors.size()-1; i>= 0; i--){
                        if(j>=0) RNGneighbors[i].printLshNeighbor(printi, RNGtrueDist[j], outputf);
                        j--;
                        printi++;
                    }
                }     
            }  
            outputf.close();  

            for(int i=0; i<trainSet.getNumberOfPixels(); i++){
                if(hashFamily[i]!=NULL){
                    delete hashFamily[i];
                }    
            } 
            delete[] hashFamily;
            for(int i=0; i<L; i++) delete hashTables[i];
            delete[] hashTables;
            ///////////////////////////////////////structure test///////////////////////////////////////


            /* PROGRAM ENDS HERE */
            exec_time = (double)(clock() - tStart)/CLOCKS_PER_SEC;
            cout << "\nExecution time is: "<< exec_time << endl;
        }

    }
    else {
        cout << "You must run the program with parameters(REQUIRED): –d <input file> –q <query file>" << endl;
        cout << "With additional parameters: –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>" << endl;
    }
}
