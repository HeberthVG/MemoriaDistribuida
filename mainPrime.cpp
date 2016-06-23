#include"Prime.h"

using namespace std;

int main() {
    
    clock_t begin = clock();
    
    Prime newPrime(48616);

    clock_t end = clock();
    double time = double(end - begin) / CLOCKS_PER_SEC;

    cout << "\nTiempo de ejecución  " << time << "s." << endl;
    newPrime.print();
    cout << "Se encontraron " << newPrime.countPrime << " números primos." <<endl;
    
    return 0;
}
