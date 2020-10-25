#include "../Search/dataset.hpp"

class Centroids{
    double **DParray;
    int *centroids;
    int points;

    public:
        Centroids(int, int);
        ~Centroids();
        void initialize(Dataset*, int);
};
