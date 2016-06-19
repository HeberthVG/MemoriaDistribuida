#include"Prime.h"

using namespace std;

int main() {
    
    clock_t begin = clock();
    
    Prime newPrime(5000);
    newPrime.print();
    
    clock_t end = clock();
    double time = double(end - begin) / CLOCKS_PER_SEC;
    cout << "\nTiempo de ejecución  " << time << "s\n";
    return 0;
}
