#define VECTOR_SIZE 784

class imageVector{
    int numberOfRows;
    int numberOfColumns;
    //vector<vector<unsigned char>> pixels;
    unsigned char** pixels;
    public:
        imageVector(int numberOfRows,int numberOfColumns, unsigned char * buffer);

        int getNumberOfRows(){return numberOfRows;};
        int getNumberOfColumns(){return numberOfColumns;};

        ~imageVector();

};