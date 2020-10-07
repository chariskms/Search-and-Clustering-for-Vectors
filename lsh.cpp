#include <iostream>
#include <stdlib.h>

// ./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>

using namespace std;

int main(int argc, char** argv){
    // Αν τα k, L δεν δίνονται, το πρόγραμμα χρησιμοποιεί default τιμές k=4, L=5, Ν=1, R=1.0.
    if (argc>6 && argc<16){
        char *d=NULL, *q=NULL, *o=NULL, *k=NULL, *l=NULL, *n=NULL, *r=NULL;
        for (int i=0; i<argc; i++){
            if (!strcmp("-d", argv[j]) && vars[0]){
                d = (char*)argv[j+1];                   /* -inputfile */
            }
        }
    }
    else {
        cout << "The parameters are: –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>" << endl;
        cout << "The necessary parameters are: –d <input file> –q <query file> myFile" << endl;
    }


}
