#include "../Search/dataset.hpp"

class Centroids{
    double **DParray;
    int *centroids;
    int numpoints;
    int numclusters;
    Dataset *set;

    public:
        Centroids(int, int, Dataset*);
        ~Centroids();
        double** getDParray() { return DParray; }
        int* getCentroids() { return centroids; }
        int getNumClusters() { return numclusters; }
        int getNumPoints() { return numpoints; }
        Dataset* getSet() { return set; }
        double minmaxDist(int, unsigned char*);
        void Initialize();
};

class Clusters{
    Centroids *Cntrds;
    std::vector<std::vector<unsigned char>> CntrdsVectors;
    std::vector<std::vector<int>> images;
    std::vector<std::vector<double>> snumbers;

    public:
        Clusters(Centroids*);
        void Clustering(char*, char*, bool, int, int, int, int, int);
        void LSHReverseAssignment(int, int);
        void PROJReverseAssignment(int, int, int);
        void Update();
        void Lloyds();
        void Silhouette();
        void Output(char*, char*, bool);
        ~Clusters();
};
