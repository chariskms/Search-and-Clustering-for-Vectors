#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <fstream>
// #include "dataset.hpp"
#define IMAGESIZE 800
#define SWAP_INT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))
// ./cube -d train-images.idx3-ubyte -q fileq -k 3 -M 10 -probes 2 -ο output -Ν 1 -R 1.0

using namespace std;

int main(int argc, char** argv){
    if (argc>6 && argc<18){
        char *d=NULL, *q=NULL, *o=NULL, *k=NULL, *m=NULL, *pr=NULL, *n=NULL, *r=NULL;
        double R=1.0, exec_time;
        int K=3, M=10, probes=2, N=1;
        for (int i = 0; i<argc; i++){
            if (!strcmp("-d", argv[i])) d = (char*)argv[i+1];   /* -inputfile */
            if (!strcmp("-q", argv[i])) q = (char*)argv[i+1];   /* -queryfile */
            if (!strcmp("-k", argv[i])) k = (char*)argv[i+1];   /* -k */
            if (!strcmp("-M", argv[i])) m = (char*)argv[i+1];   /* -M */
            if (!strcmp("-probes", argv[i])) pr = (char*)argv[i+1]; /* -probes */
            if (!strcmp("-o", argv[i])) o = (char*)argv[i+1];   /* -o */
            if (!strcmp("-N", argv[i])) n = (char*)argv[i+1];  /* -probes */
            if (!strcmp("-R", argv[i])) r = (char*)argv[i+1];   /* -R */
        }

        if(d==NULL || q==NULL){
            cout << "You must run the program with parameters(REQUIRED): –d <input file> –q <query file>" << endl;
            cout << "With additional parameters: –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>" << endl;
            exit(0);
        }
        else{
            if (k!=NULL) K = atoi(k);
            if (m!=NULL) M = atoi(m);
            if (n!=NULL) N = atoi(n);
            if (pr!=NULL) probes = atoi(pr);
            if (r!=NULL) R = atof(r);

            /* PROGRAM STARTS HERE */
            clock_t tStart = clock();

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
