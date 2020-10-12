class Dataset{

    int magicNumber;
    int numberOfImages;
    int numberOfRows;
    int numberOfColumns;
    int numberOfPixels;

    unsigned char** images;
    public:
        Dataset(int , int , int ,int , unsigned char ** );
        ~Dataset();

        int getNumberOfRows(){return numberOfRows;};
        int getNumberOfColumns(){return numberOfColumns;};
        int magicNumber(){return magicNumber;};
        int numberOfImages(){return numberOfImages;};
        int numberOfPixels(){return numberOfPixels;};
        unsigned char * imageAt(int);

        

};