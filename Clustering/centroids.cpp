#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string>
#include <cmath>
#include <random>
#include <cfloat>
#include "../Search/metrics.h"
#include "centroids.hpp"

using namespace std;

Centroids::Centroids(int clusters, int n){
    DParray = new double*[2];
    DParray[0] = new double[n];     //D-Distances
    DParray[1] = new double[n];     //P-partial sums
    // memset(DParray[0], 0, n);
    // memset(DParray[1], 0, n);
    centroids = new int[clusters];  //indexes of centroids
    points = n;
}

Centroids::~Centroids(){
    delete[] DParray[0];
    delete[] DParray[1];
    delete[] DParray;
    delete[] centroids;
}

void Centroids::initialize(Dataset *set, int clusters){
    default_random_engine generator;
    uniform_int_distribution<int> distribution(0,points);
    //Choose randomly first centroid
    centroids[0] = distribution(generator);
    cout << "c0: " << centroids[0] << endl;
    for(int i=1; i<clusters; i++){
        int sum = 0.0;
        double max = 0.0;
        for(int j=0; j<points; j++){
            //Find minimum D distance between vector and centroid
            double min = DBL_MAX, manh = 0.0;
            for(int k=0; k<i; k++){
                if(j != centroids[k]){
                    manh = manhattan(set->imageAt(j), set->imageAt(centroids[k]), set->dimension());
                    if(manh<min) min = manh;
                    if(manh>max) max = manh;
                }
                else{
                    min = 0;
                    break;
                }
            }
            DParray[0][j] = min;
        }
        for(int j=0; j<points; j++){
            sum+=(DParray[0][j]/max)*DParray[0][j];
            DParray[1][j] = sum;
        }
        uniform_real_distribution<> distribution(0,DParray[1][points-1]);
        double find_index = distribution(generator);
        int j = 1, index = 0;
        //Find index from random value
        if(DParray[1][0]>=find_index) index = 0;
        else{
            while(j<points){
                if(DParray[1][j-1]<find_index && DParray[1][j]>=find_index){
                    index = j;
                    break;
                }
                j++;
            }
        }
        centroids[i] = index;
        cout << "c" << i << ": " << centroids[i] << endl;
    }
}
