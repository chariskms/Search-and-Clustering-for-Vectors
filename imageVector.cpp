#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include <cstring>
#include <time.h>
#include "imageVector.h"

using namespace std;

imageVector::imageVector(int numberOfRows,int numberOfColumns, unsigned char * buffer){

    numberOfRows = numberOfRows;
    numberOfColumns = numberOfColumns;

    //pixels.resize(numberOfRows, vector<unsigned char>(numberOfColumns, 0));
    pixels = new unsigned char*[numberOfRows];
    for(int i = 0; i < numberOfRows;i++){
        pixels[i] = new unsigned char[numberOfColumns];
        for(int j = 0; j < numberOfColumns;j++){
            pixels[i][j] = buffer[i*numberOfColumns + j];
            
        }      
    }

}

imageVector::~imageVector(){
    for(int i = 0; i < numberOfRows;i++){
        delete[] pixels[i];
           
    }
    delete[] pixels; 


}    