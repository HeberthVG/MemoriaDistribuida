#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>

#ifndef PRIME_H
#define PRIME_H

using namespace std;

class Prime{
public:
	Prime(int n);
	///constructor de copia.
    void print();
    
	
	
private:
	int n;
    int *primeV;
    //Se define los parámetros.
};
#endif