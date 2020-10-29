#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string>
#include <cmath>
#include <random>
#include <cfloat>
#include <algorithm>
#include <vector>
#include <fstream>
#include "../Search/metrics.h"
#include "centroids.hpp"

using namespace std;

Centroids::Centroids(int clusters, int points, Dataset *ptrset){
    DParray = new double*[2];
    DParray[0] = new double[points];     //D-Distances
    DParray[1] = new double[points];     //P-Partial sums
    DParray[2] = new double[points];     //Centroid with min dist
    centroids = new int[clusters];       //Indexes of centroids
    numpoints = points;
    numclusters = clusters;
    set = ptrset;
}

Centroids::~Centroids(){
    delete[] DParray[0];
    delete[] DParray[1];
    delete[] DParray[2];
    delete[] DParray;
    delete[] centroids;
}

double Centroids::minmaxDist(int lastcentroid, unsigned char* otherCentroid){
    double manh = 0.0, max = 0.0;
    unsigned char* pointCentroid = NULL;
    bool assignment=false;
    if(otherCentroid == NULL) pointCentroid = set->imageAt(centroids[lastcentroid]);
    else{
        pointCentroid = otherCentroid;
        assignment = true;
    }
    for(int j=0; j<numpoints; j++){
        if(j != centroids[lastcentroid] || assignment){
            manh = manhattan(set->imageAt(j), pointCentroid, set->dimension());
            if(lastcentroid == 0) DParray[0][j] = DBL_MAX;
            if(manh<DParray[0][j]){
                DParray[0][j] = manh;
                DParray[2][j] = lastcentroid;
            }
            if(manh>max) max = manh;
        }
        else{
            DParray[0][j] = 0;
            DParray[2][j] = lastcentroid;
        }
    }
    return max;
}

void Centroids::Initialize(){
    random_device randf;
    default_random_engine generator(randf());
    // default_random_engine generator;
    uniform_int_distribution<int> distribution(0,numpoints-1);

    //Choose randomly first centroid
    centroids[0] = distribution(generator);
    cout << "c0: " << centroids[0] << endl;
    for(int i=1; i<numclusters; i++){
        int sum = 0.0;
        double max = 0.0;
        //Update distances and return max from all distances
        max = minmaxDist(i-1, NULL);
        for(int j=0; j<numpoints; j++){
            sum+=(DParray[0][j]/max)*DParray[0][j];
            DParray[1][j] = sum;
        }
        uniform_real_distribution<> distribution(0,DParray[1][numpoints-1]);
        double find_index = distribution(generator);
        int j = 1, index = 0;
        //Find index from random value
        if(DParray[1][0]>=find_index) index = 0;
        else{
            while(j<numpoints){
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
    //Update for last centroid to use it in Lloyds
    cout << endl;
    minmaxDist(numclusters-1, NULL);
}

Clusters::Clusters(Centroids* argCntrds) : Cntrds(argCntrds){}

void Clusters::Clustering(char* method, char* output, bool complete){
    //Here output and time exec
    cout << "Begin clustering with " << method << " method" << endl;
    if(strcmp(method, "CLASSIC")==0 || strcmp(method, "classic")==0 || strcmp(method, "Classic")==0){
        Lloyds();
    }
    else if(strcmp(method, "LSH")==0 || strcmp(method, "lsh")==0 || strcmp(method, "Lsh")==0){
        cout << "Begin clustering with LSH method" << endl;
    }
    else if(strcmp(method, "HYPERCUBE")==0 || strcmp(method, "hypercube")==0 || strcmp(method, "Hypercube")==0){
        cout << "Begin clustering with Hypercube method" << endl;
    }
    else{
        cout << "Wrong method. The available methods are: Classic, LSH, Hypercube." << endl;
        return;
    }
    Output(method, output, complete);
}

//Update
void Clusters::Update(){
    Dataset *set = Cntrds->getSet();
    int clusters = Cntrds->getNumClusters();
    int *centroids = Cntrds->getCentroids();
    CntrdsVectors.clear();
    vector<unsigned char> newCentroid;
    vector<double> findmedian;
    unsigned char* tmp = NULL;

    for(int i=0; i<clusters; i++){
        cout << "In cluster: " << i << endl;
        int median = ceil(images[i].size()/2);
        newCentroid.clear();
        for (int k=0; k<set->dimension(); k++){
            int allsame = 0;
            int numsame = 0;
            findmedian.clear();
            for(int j=0; j<images[i].size(); j++){
                //For every point in cluster i
                int index = images[i][j];
                tmp = set->imageAt(index);
                findmedian.push_back((double) tmp[k]);
                //Check if all are same number to optimize median
                if(j==0) numsame = (double) tmp[k];
                if((int) tmp[k] == numsame) allsame++;
            }
            //If all are same number then it doesnt need sort and we can pick the median
            if(allsame != images[i].size()){
                sort(findmedian.begin(), findmedian.end());
            }
            newCentroid.push_back(findmedian[median]);
        }
        CntrdsVectors.push_back(newCentroid);
        centroids[i] = i;
    }
}

//Assignment
void Clusters::Lloyds(){
    int points = Cntrds->getNumPoints();
    int clusters = Cntrds->getNumClusters();
    double **DParray = Cntrds->getDParray();
    Dataset *set = Cntrds->getSet();

    //Initialize Clusters from Centroid initialization
    int *tmp = Cntrds->getCentroids();
    for(int i=0; i<clusters; i++)
        images.push_back(std::vector<int>());
    for(int i=0; i<points; i++)
        images[DParray[2][i]].push_back(i); //Push image to the centroid with min dist DParray[2][i]

    // cout << "before " << endl;
    // for(int i=0; i<points; i++) cout << i << ") Clusters are: " << DParray[2][i] << " with dist: " << DParray[0][i] << endl;

    // index = image[from_cluster][what_image]
    int loops = 0;
    int tmpPoint = 0;
    //Checking for no-change
    while(loops<1){
        cout << "In loop: " << loops << endl;
        //Update Centroids from clusters
        Update();

        // Assignment for all points to the nearest Centroid
        for(int i=1; i<clusters+1; i++)
            Cntrds->minmaxDist(i-1, &CntrdsVectors[i-1][0]);

        //Change points from the old cluster to the new
        for(int i=0; i<clusters; i++){
            for(int j=0; j<images[i].size(); j++){
                int newCluster = DParray[2][images[i][j]];
                if(i!=newCluster){
                    tmpPoint = images[i][j];
                    images[i].erase(images[i].begin()+j);
                    images[newCluster].push_back(tmpPoint);
                }
            }
        }
        loops++;
    }

    // cout << "after " << endl;
    // for(int i=0; i<points; i++) cout << i << ") Clusters are: " << DParray[2][i] << " with dist: " << DParray[0][i] << endl;

}

void Clusters::Silhouette(){
    //Silhouette of a object:
    //a(i)->average distance of i objects in same cluster
    //b(i)->average distance of i to objects in next best cluster
    //s(i)->(b(i)-a(i))/max{a(i), b(i)}

    //Silhouette of a cluster:
    //average of all s(i) in cluster

    //Silhouette of all clusters:
    //average s(i) over i=1,...,n
}

void Clusters::Output(char* method, char *output, bool complete){
    ofstream outfile((char*)output);
    if (outfile.is_open()){
        outfile << "Algorithm: "<< method << "\n";
        for(int i=0; i<CntrdsVectors.size(); i++){
            outfile << "CLUSTER-" << i+1 << "{size: " << CntrdsVectors[i].size() << ", centroid: ";
            for(int j=0; j<CntrdsVectors[i].size(); j++){
                outfile << (int)CntrdsVectors[i][j] << " ";
            }
            outfile << "}\n\n";
        }
        //Silhouette here
        if(complete){
            for(int i=0; i<CntrdsVectors.size(); i++){
                outfile << "CLUSTER-" << i+1 << "{ ";
                for(int j=0; j<CntrdsVectors[i].size(); j++){
                    outfile << (int)CntrdsVectors[i][j] << " ";
                }
                outfile << ", ";
                for(int j=0; j<images[i].size(); j++){
                    outfile << images[i][j];
                    if(j-1<images[i].size()) outfile << ", ";
                }
                outfile << "}\n";
            }
        }


        outfile.close();
    }
    else cout << "Unable to open outout file" << endl;
}

Clusters::~Clusters(){

}
