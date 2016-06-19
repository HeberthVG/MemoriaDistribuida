#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "mpi.h"

#ifndef PRIMEMPI_H
#define PRIMEMPI_H

using namespace std;

class PrimeMPI{
public:
	PrimeMPI(int argc, char ** argv, int n);
	///constructor de copia.
    void print();
    
	
	
private:
	int n;
    int *primeV;
    //Se define los parámetros.
};
#endif
