#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <fstream>
#include "dataset.h"

#define IMAGESIZE 800
#define SWAP_INT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))

// ./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>
// ./lsh -d train-images.idx3-ubyte -R 1.0 -q fileq -k 4 -L 5 -o fileo -N 1

using namespace std;

int main(int argc, char** argv){
    if (argc>6 && argc<16){
        char *d=NULL, *q=NULL, *o=NULL, *k=NULL, *l=NULL, *n=NULL, *r=NULL;
        double R=1.0, exec_time;
        int K=4, L=5, N=1;
        for (int i = 0; i<argc; i++){
            if (!strcmp("-d", argv[i])){
                d = (char*)argv[i+1];                   /* -inputfile */
            }
            if (!strcmp("-q", argv[i])){
                q = (char*)argv[i+1];                   /* -queryfile */
            }
            if (!strcmp("-k", argv[i])){
                k = (char*)argv[i+1];                   /* -k */
            }
            if (!strcmp("-L", argv[i])){
                l = (char*)argv[i+1];                   /* -L */
            }
            if (!strcmp("-o", argv[i])){
                o = (char*)argv[i+1];                   /* -o */
            }
            if (!strcmp("-N", argv[i])){
                n = (char*)argv[i+1];                   /* -N */
            }
            if (!strcmp("-R", argv[i])){
                r = (char*)argv[i+1];                   /* -R */
            }
        }

        if(d==NULL || q==NULL){
            cout << "You must run the program with parameters(REQUIRED): –d <input file> –q <query file>" << endl;
            cout << "With additional parameters: –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>" << endl;
            exit(0);
        }
        else{
            if (k!=NULL) K = atoi(k);
            if (l!=NULL) L = atoi(l);
            if (n!=NULL) N = atoi(n);
            if (r!=NULL) R = atof(r);

            /* PROGRAM STARTS HERE */

            clock_t tStart = clock();
            //cout << "Run program with parameters: ->" << d << " -> " << q << " -> " << K << " -> " << L << " -> " << o << " -> " << N << " -> " << R << endl;

            int magicNumber = 0,numberOfImages = 0,numberOfRows = 0,numberOfColumns = 0;
            int numOfpixels;

            fstream input(d);

            if(!input.is_open()){
                cerr<<"Failed to open input data."<<endl;
                exit(0);
            }

            input.read((char*)&magicNumber, 4);
            input.read((char*)&numberOfImages, 4);
            input.read((char*)&numberOfRows, 4);
            input.read((char*)&numberOfColumns, 4);

            //convert intergers from Big Endian to Little Endian
            magicNumber = SWAP_INT32(magicNumber);
            numberOfImages = SWAP_INT32(numberOfImages);
            numberOfRows = SWAP_INT32(numberOfRows);
            numberOfColumns = SWAP_INT32(numberOfColumns);

            cout << "magic Number: " << magicNumber << endl;
            cout << "number Of Images: " << numberOfImages << endl;
            cout << "number Of Rows: " << numberOfRows << endl;
            cout << "number Of Columns: " << numberOfColumns << endl;

            numOfpixels = numberOfRows*numberOfColumns;

            unsigned char *buffer = new unsigned char[numOfpixels*numberOfImages];

            input.read((char*)buffer, numOfpixels*numberOfImages);

            Dataset dataset(magicNumber, numberOfImages, numberOfRows, numberOfColumns, buffer);

            input.close();

            delete[] buffer;


            //here query input



            //fclose(fileFd);
            /* PROGRAM ENDS HERE */
            exec_time = (double)(clock() - tStart)/CLOCKS_PER_SEC;
            cout << "Execution time is: "<< exec_time << endl;
        }

    }
    else {
        cout << "You must run the program with parameters(REQUIRED): –d <input file> –q <query file>" << endl;
        cout << "With additional parameters: –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>" << endl;
    }


}
