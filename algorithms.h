class Neighbor{
    // int indexq;
    int index;
    double lshDist;
    double trueDist;
    unsigned char *image;

    public:
        Neighbor(int, double, double, unsigned char*);
        void printNeighbor(int);
        ~Neighbor();
};

int FindW(int, Dataset*);
void ANNsearch(int, int, int, unsigned char*, HashTable**);
void RNGsearch(int, int, Dataset*, HashTable**);
