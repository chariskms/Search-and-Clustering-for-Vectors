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
        cout << "cluster: " << i << endl;
        int sum = 0.0;
        double max = 0.0;
        for(int j=0; j<points; j++){
            //Find minimum D distance between vector and centroid
            double min = DBL_MAX, manh = 0.0;
            for(int k=0; k<i; k++){
                manh = manhattan(set->imageAt(j), set->imageAt(centroids[k]), set->dimension());
                if(manh<min) min = manh;
                if(manh>max) max = manh;
            }
            DParray[0][j] = min;
        }
        for(int j=0; j<points; j++){
            sum+=(DParray[0][j]/max)*DParray[0][j];
            DParray[1][j] = sum;
        }
        cout << "Array " << DParray[1][points-1] << endl;
        uniform_real_distribution<> distribution(0,DParray[1][points-1]);
        double find_index = distribution(generator);
        bool found = false;
        int left = 0, right = points-1, index = 0;
        //problem
        while(!found){
            if(right>=left){
                index = left+(right-left)/2;
                if(DParray[1][index]>find_index){
                    if(index>1){
                        if(DParray[1][index-1]<=find_index)
                    }
                    right = index-1;
                }
                else if(DParray[1][index]<find_index){
                    left = index+1;
                }
                else found = true;
            }
        }
        cout << index << endl;
        centroids[i] = index;
        cout << "c" << i << ": " << centroids[i] << endl;
    }
}
