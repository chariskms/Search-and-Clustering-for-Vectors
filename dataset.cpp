#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include <cstring>
#include <time.h>
#include "dataset.hpp"

using namespace std;

Dataset::Dataset(int magicNumber,int numberOfImages, int numberOfColumns,int numberOfRows){
    numberOfRows = numberOfRows;
    numberOfColumns = numberOfColumns;
    magicNumber = magicNumber;
    numberOfImages = numberOfImages;
    numberOfPixels = numberOfColumns*numberOfRows;

    unsigned char* images = new unsigned char[numberOfImages*numberOfPixels];

}

Dataset::~Dataset(){
    delete[] images;
}

inline unsigned char * Dataset::imageAt(int index){
    return &images[index*numberOfPixels];
}
