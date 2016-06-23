/*
 Universidad de Costa Rica
 Escuela de Ingeniería Eléctrica
 Estructuras de Computadoras Digitales II
 IE-0521
 Tarea Programada 2
 Carlos Solano       B36685
 Heberth Valverde    B37174
 */

//

#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "mpi.h"

using namespace std;

//Se define el constructor.

int main(int argc, char ** argv)
{
    
    int n=48616;
    int procs, id, largo, rc;
    char host[MPI_MAX_PROCESSOR_NAME];
    double begin, end;
    
    rc = MPI_Init(&argc, &argv);
    
    MPI_Status stat;
    
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Get_processor_name(host, &largo);
    
    begin=MPI_Wtime();
    
    int prime[n];
    prime[0]=0;
    prime[1]=0;
    for(int i=2; i<n;i++) prime[i]=i;
    
    for(int i=2+id; i*i<=n;i+=procs)
    {
        if(prime[i]!=0)
        {
            for(int j=i; j*i<=n;j++)
            {
                prime[j*i]=0;
            }
        }
    }
    
    end=MPI_Wtime();
    
    MPI_Barrier(MPI_COMM_WORLD);
    //Se reciben los datos.
    
    int p[n];
    MPI_Reduce(&prime, p, n, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    
    MPI_Finalize();
    
    double time = end - begin;
    if(id==0)
    {
        int c=0;
        ///Se crea el archivo de salida
        ofstream outputFile("PrimeNumbersMPI.txt");
        for(int i=0; i<n;i++)
        {
            if(p[i]!=0)
            {
                outputFile<< i << "\t";
        	c++;
            }
        }
        cout<<endl;
        cout<<"Se encontraron "<<c<<" números primos."<<endl;
        cout << "\nTiempo de ejecución  " << time << "s\n";
    }  
    
    return 0;
}

//Método para obtener la lista de primos.

