#include"PrimeMPI.h"

using namespace std;

int main(int argc, char ** argv) {
    
    clock_t begin = clock();
    
    PrimeMPI newPrime(argc, argv, 5000);
    newPrime.print();
    
    clock_t end = clock();
    double time = double(end - begin) / CLOCKS_PER_SEC;
    cout << "\nTiempo de ejecución  " << time << "s\n";
    return 0;
}