#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include <cstring>
#include <time.h>
#include "dataset.hpp"

using namespace std;

Dataset::Dataset(int magicNumberArg,int numberOfImagesArg, int numberOfColumnsArg,int numberOfRowsArg){
    this->numberOfRows = numberOfRowsArg;
    this->numberOfColumns = numberOfColumnsArg;
    this->magicNumber = magicNumberArg;
    this->numberOfImages = numberOfImagesArg;
    this->numberOfPixels = numberOfColumnsArg*numberOfRowsArg;
    this->images = new unsigned char[numberOfImagesArg*numberOfPixels];
}

Dataset::~Dataset(){
    delete[] images;
}
