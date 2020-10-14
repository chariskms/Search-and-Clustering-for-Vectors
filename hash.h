class hashFunction{
    int * s_numbers;
    int d; //d diastasi 
    int w; //w window 
    int M; //M = 2^(32/k)
    int m; //m = 2^(32-3) ,must m > max_xi
    int k; //k number of hashFunctions

    public:

        hashFunction(int, int, int);

        ~hashFunction();

        int hash(unsigned char*);



};