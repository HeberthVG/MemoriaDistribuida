/*
    Universidad de Costa Rica
    Escuela de Ingeniería Eléctrica
    Estructuras de Computadoras Digitales II
    IE-0521
    Tarea Programada 2
    Carlos Solano       B36685
    Heberth Valverde    B37174
*/

// Bibioteca de cálculo de números primos.

#include "Prime.h"

using namespace std;


//Se define el constructor.
Prime::Prime(int n)
{
    this->n=n;
    this->primeV=new int [n]();
    this->countPrime = 0;

    for(int i=2; i<=n;++i) this->primeV[i]=i;
    for(int i=2; i*i<=n;++i)
    {
    	if(this->primeV[i]!=0)
    	{
    		for(int j=i; j*i<=n;++j) this->primeV[j*i]=0;
    	}
    }
}


void Prime::print(){
    ///Se crea el archivo de salida
    ofstream outputFile("PrimeNumbers.txt");
    for(int i=2; i<=n;++i)
    {
    	if(primeV[i]!=0)
    	{
			outputFile << i << "\t";
			this->countPrime++;
		}
	}
    outputFile.close();
}

//Método para obtener la lista de primos.

