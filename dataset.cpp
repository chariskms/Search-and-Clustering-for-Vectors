#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include <cstring>
#include <time.h>
#include "dataset.h"

using namespace std;

Dataset::Dataset(int magicNumber,int numberOfImages, int numberOfColumns,int numberOfRows, unsigned char * buffer){

    numberOfRows = numberOfRows;
    numberOfColumns = numberOfColumns;
    magicNumber = magicNumber;
    numberOfImages = numberOfImages;
    numberOfPixels = numberOfColumns*numberOfRows;
    
    unsigned char* images = new unsigned char[numberOfImages*numberOfPixels];
    memcpy(images, buffer, numberOfImages*numberOfPixels);

}

Dataset::~Dataset(){
    delete[] images; 
}

Dataset::imageAt(int index){

    return index*numberOfPixels;

}