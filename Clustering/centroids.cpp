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
#define FLOOR_CHANGED_POINTS 100
#define MAX_LOOPS 3
#define RADIUS 12000

using namespace std;

Centroids::Centroids(int clusters, int points, Dataset *ptrset){
    DParray = new double*[5];
    DParray[0] = new double[points];     //D-Distances
    DParray[1] = new double[points];     //P-Partial sums
    DParray[2] = new double[points];     //Centroid with min dist
    DParray[3] = new double[points];     //D-Distances with second min dist
    DParray[4] = new double[points];     //Centroid with second min dist
    centroids = new int[clusters];       //Indexes of centroids
    numpoints = points;
    numclusters = clusters;
    set = ptrset;
}

Centroids::~Centroids(){
    delete[] DParray[0];
    delete[] DParray[1];
    delete[] DParray[2];
    delete[] DParray[3];
    delete[] DParray[4];
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
            if(lastcentroid == 0){
                DParray[0][j] = DBL_MAX;
                DParray[2][j] = lastcentroid;
                DParray[3][j] = DBL_MAX;
            }
            if(manh<DParray[0][j]){
                if(DParray[0][j]<DParray[3][j]){
                    DParray[3][j] = DParray[0][j];
                    DParray[4][j] = DParray[2][j];
                }
                DParray[0][j] = manh;
                DParray[2][j] = lastcentroid;
            }
            else{
                if(manh<DParray[3][j]){
                    DParray[3][j] = manh;
                    DParray[4][j] = lastcentroid;
                }
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
    cout << "Initialize Centroids with k-Means++ with indexes: " << endl;
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

void Clusters::Clustering(char* method, char* output, bool complete, int numHashTables, int numHashFunctions, int maxMhypercube, int hypercubeDim, int numprobes){
    cout << "Begin clustering with " << method << " method" << endl;
    if(strcmp(method, "CLASSIC")==0 || strcmp(method, "classic")==0 || strcmp(method, "Classic")==0){
        Lloyds();
    }
    else if(strcmp(method, "LSH")==0 || strcmp(method, "lsh")==0 || strcmp(method, "Lsh")==0){
        //Initialize hashtables & hashfunctions
        cout << "Initialize HashTables and HashFunctions..." << endl;
        int points = Cntrds->getNumPoints();
        int clusters = Cntrds->getNumClusters();
        Dataset *set = Cntrds->getSet();
        int W = 40000;
        int bucketsNumber = floor(points/16);
        HashFunction **hashFamily = new HashFunction*[set->getNumberOfPixels()];
        for(int i=0; i<set->getNumberOfPixels(); i++){
            hashFamily[i] = NULL;
        }
        HashTable **hashTables = new HashTable*[numHashTables];
        for(int i=0; i<numHashTables; i++){
            hashTables[i] = new HashTable(set->getNumberOfPixels(),bucketsNumber,numHashFunctions,W,hashFamily);
            for(int j=0; j<points; j++){
                unsigned int g_hash = (unsigned int)(hashTables[i]->ghash(set->imageAt(j)));
                hashTables[i]->getBucketArray()[g_hash%bucketsNumber]->addImage(j,g_hash,set->imageAt(j));
            }
        }
        LSHReverseAssignment(numHashTables, numHashFunctions, hashTables);
        for(int i=0; i<set->getNumberOfPixels(); i++){
            if(hashFamily[i]!=NULL){
                delete hashFamily[i];
            }
        }
        delete[] hashFamily;
        for(int i=0; i<numHashTables; i++) delete hashTables[i];
        delete[] hashTables;

    }
    else if(strcmp(method, "HYPERCUBE")==0 || strcmp(method, "hypercube")==0 || strcmp(method, "Hypercube")==0){
        cout << "Begin clustering with Hypercube method" << endl;
        // PROJReverseAssignment(int maxMhypercube, int hypercubeDim, int numprobes);
    }
    else{
        cout << "Wrong method. The available methods are: Classic, LSH, Hypercube." << endl;
        return;
    }

    // Silhouette();
    // Output(method, output, complete);
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

    // index = image[from_cluster][what_image]
    int loops = 0;
    int tmpPoint = 0;
    int changedpoints = set->getNumberOfImages();
    //Checking for no-change
    while(changedpoints>FLOOR_CHANGED_POINTS && loops<MAX_LOOPS){
        cout << "IN FOR: " << loops << endl;
        changedpoints = 0;
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
                    changedpoints++;
                }
            }
        }
        cout << "Changed points: " << changedpoints << "\n\n";
        loops++;
    }
}

void Clusters::AssignReverse(vector<vector<Neighbor>> Neighbors, bool initialize){
    int points = Cntrds->getNumPoints();
    int clusters = Cntrds->getNumClusters();
    int *centroids = Cntrds->getCentroids();
    double **DParray = Cntrds->getDParray();
    Dataset *set = Cntrds->getSet();
    unsigned char* CentroidVector = NULL;

    if(initialize){
        for(int i=0; i<clusters; i++) CntrdsVectors.push_back(std::vector<unsigned char>());
    }

    //Unassign all points
    for(int i=0; i<points; i++) DParray[0][i] = -1;
    for(int i=0; i<Neighbors.size(); i++){
        for(int j=0; j<Neighbors[i].size(); j++){
            int index = Neighbors[i][j].getIndex();
            //Check if point is unassigned or it has been already assigned
            if(DParray[0][index]<0){
                DParray[0][index] = Neighbors[i][j].getlshDist();
                DParray[2][index] = i;
            }
            else{
                //If is already assigned then find the nearest Centroid
                if(initialize) CentroidVector = set->imageAt(centroids[i]);
                else CentroidVector = &CntrdsVectors[i][0];
                double manh = manhattan(set->imageAt(index), CentroidVector, set->dimension());
                if(manh<DParray[0][index]){
                    cout << "Change " << manh << "<" << DParray[0][index] << " ~~ " << DParray[2][index] << " -> " << i << endl;
                    DParray[0][index] = manh;
                    DParray[2][index] = i;
                }
            }

        }
    }
    double min = 0, manh = 0;
    //For unassigned points
    for(int i=0; i<points; i++){
        if(DParray[0][i]<0){
            min = DBL_MAX;
            for(int j=0; j<clusters; j++){
                if(initialize) CentroidVector = set->imageAt(centroids[j]);
                else CentroidVector = &CntrdsVectors[j][0];
                manh = manhattan(set->imageAt(i), CentroidVector, set->dimension());
                if(manh<min){
                    DParray[0][i] = manh;
                    DParray[2][i] = j;
                    min = manh;
                }
            }
        }
        // cout << i << ") " << DParray[0][i] << " " << DParray[2][i] << endl;
    }
}

void Clusters::LSHReverseAssignment(int numHashTables, int numHashFunctions, HashTable **hashTables){
    int points = Cntrds->getNumPoints();
    int clusters = Cntrds->getNumClusters();
    int *centroids = Cntrds->getCentroids();
    double **DParray = Cntrds->getDParray();
    Dataset *set = Cntrds->getSet();

    //Initialize images from centroids
    vector<vector<Neighbor>> RNGneighbors;
    for(int i=0; i<clusters; i++){
        RNGneighbors.push_back(std::vector<Neighbor>());
        RNGsearch(RNGneighbors[i], numHashTables, RADIUS, set->imageAt(centroids[i]), hashTables);
    }
    AssignReverse(RNGneighbors, true);
    //Initialize clusters from DParray structure
    int *tmp = Cntrds->getCentroids();
    for(int i=0; i<clusters; i++)
        images.push_back(std::vector<int>());
    for(int i=0; i<points; i++)
        images[DParray[2][i]].push_back(i); //Push image to the centroid with min dist DParray[2][i]

    int loops = 0;
    int tmpPoint = 0;
    int changedpoints = set->getNumberOfImages();

    //Checking for no-change
    while(changedpoints>FLOOR_CHANGED_POINTS && loops<MAX_LOOPS){
        cout << "IN FOR: " << loops << endl;
        changedpoints = 0;
        //Clear neighbors
        for(int i=0; i<RNGneighbors.size(); i++) RNGneighbors[i].clear();

        //Update Centroids from clusters
        Update();

        // Assignment
        for(int i=0; i<clusters; i++)
            RNGsearch(RNGneighbors[i], numHashTables, RADIUS, &CntrdsVectors[i][0], hashTables);
        AssignReverse(RNGneighbors, false);

        //Change points from the old cluster to the new
        for(int i=0; i<clusters; i++){
            for(int j=0; j<images[i].size(); j++){
                int newCluster = DParray[2][images[i][j]];
                if(i!=newCluster){
                    tmpPoint = images[i][j];
                    images[i].erase(images[i].begin()+j);
                    images[newCluster].push_back(tmpPoint);
                    changedpoints++;
                }
            }
        }
        cout << "Changed points: " << changedpoints << "\n\n";
        loops++;
    }
}

// void Clusters::PROJReverseAssignment(){
//     int W = 40000;
//
//     HashFunction** hashFamily = new HashFunction*[trainSet.getNumberOfPixels()];
//     for(int i = 0; i < trainSet.getNumberOfPixels();i++){
//         hashFamily[i] = NULL;
//     }
//     Projection *projection = new Projection(trainSet.getNumberOfPixels(),bucketsNumber, K,W, hashFamily);
//     for(int j=0; j<trainSet.getNumberOfImages(); j++){
//         unsigned int g_hash = (unsigned int)(projection->ghash(trainSet.imageAt(j)));
//         projection->getBucketArray()[g_hash%bucketsNumber]->addImage(j,g_hash,trainSet.imageAt(j));
//     }
//
//     hyperCubeSearch(outputf, R, N, probes, i, querySet.imageAt(i), &trainSet, projection);
//
//
//     for(int i=0; i<trainSet.getNumberOfPixels(); i++){
//         if(hashFamily[i]!=NULL){
//             delete hashFamily[i];
//         }
//     }
//     delete[] hashFamily;
//     delete projection;
// }

void Clusters::Silhouette(){
    int points = Cntrds->getNumPoints();
    int clusters = Cntrds->getNumClusters();
    double **DParray = Cntrds->getDParray();
    Dataset *set = Cntrds->getSet();

    double a;
    double b;
    int index1 = 0;
    int index2 = 0;
    double manh = 0, sum_a = 0, sum_b = 0, totalsum = 0, clusterssum = 0, max = 0, result = 0;

    for(int i=0; i<images.size(); i++){
        snumbers.push_back(std::vector<double>());
        cout << "CLUSTER " << i << "/" << images.size() << endl;
        clusterssum = 0;
        for(int j=0; j<images[i].size(); j++){
            sum_a = 0;
            sum_b = 0;
            // a(i) -> Find average dist from all points in same cluster
            for(int k=0; k<images[i].size(); k++){
                if(j!=k){
                    index1 = images[i][j];
                    index2 = images[i][k];
                    sum_a+= manhattan(set->imageAt(index1), set->imageAt(index2), set->dimension());
                }
            }
            a = (sum_a/(images[i].size()-1));
            int nextBestCluster = DParray[4][j];
            // b(i) -> Find average dist from all points in next best cluster
            for(int k=0; k<images[nextBestCluster].size(); k++){
                index1 = images[i][j];
                index2 = images[nextBestCluster][k];
                sum_b+=manhattan(set->imageAt(index1), set->imageAt(index2), set->dimension());
            }
            b = (sum_b/images[nextBestCluster].size());

            //Calculate s(i) numbers:
            max = a;
            if(a<b) max = b;
            if(max!=0){
                result = (b-a)/max;
            }
            else result = 0;
            snumbers[i].push_back(result);
            totalsum+=result;
            clusterssum+=result;
        }
        cout << "For " << i << " Silhouette is: " << clusterssum/images[i].size() << endl;
    }
    cout << "Silhouette is: " << totalsum/points;
}

void Clusters::Output(char* method, char *output, bool complete){
    ofstream outfile((char*)output);
    if (outfile.is_open()){
        outfile << "Algorithm: "<< method << "\n";
        for(int i=0; i<CntrdsVectors.size(); i++){
            outfile << "CLUSTER-" << i+1 << " {size: " << images[i].size() << ", centroid: ";
            for(int j=0; j<CntrdsVectors[i].size(); j++){
                outfile << (int)CntrdsVectors[i][j] << " ";
            }
            outfile << "}\n\n";
        }


        //Silhouette here


        if(complete){
            for(int i=0; i<CntrdsVectors.size(); i++){
                outfile << "CLUSTER-" << i+1 << " { ";
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
