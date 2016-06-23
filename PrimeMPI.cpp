/*
 Universidad de Costa Rica
 Escuela de Ingeniería Eléctrica
 Estructuras de Computadoras Digitales II
 IE-0521
 Tarea Programada 2
 Carlos Solano       B36685
 Heberth Valverde    B37174
*/

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
    
    rc = MPI_Init(&argc, &argv); //Inicializa los procesos
    
    MPI_Status stat;
    
    MPI_Comm_size(MPI_COMM_WORLD, &procs); //Se asigna la cantidad de procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &id); //Se asigna un id a cada proceso
    MPI_Get_processor_name(host, &largo);
    
    begin=MPI_Wtime(); //Tiempo de inicio 
    
    int prime[n]; //Se define el vector de numeros y se inicializa
    prime[0]=0;
<<<<<<< HEAD
    for(int i=1; i<n;i++) prime[i]=i+1;

    int c=0; //Se iniciliza contador de primos encontrados
    
    //Alterna los numeros que revisa segun el proceso que sea
    for(int i=1+id; prime[i]*prime[i]<=n;i+=procs)
=======
    prime[1]=0;
    for(int i=2; i<n;i++) prime[i]=i;
    
    for(int i=2+id; i*i<=n;i+=procs)
>>>>>>> 1ff01f90dec82e755d5334067f9a3d14e223e1ca
    {
	//Si el numero no se ha borrado (es primo) se borran todos sus multiplos
        if(prime[i]!=0)
        {
<<<<<<< HEAD
            for(int j=i+1; j*prime[i]<=n;j++)
            {
                prime[j*(i+1)-1]=0;
=======
            for(int j=i; j*i<=n;j++)
            {
                prime[j*i]=0;
>>>>>>> 1ff01f90dec82e755d5334067f9a3d14e223e1ca
            }
        }
    }
    
    end=MPI_Wtime(); //Tiempo final 
    
    MPI_Barrier(MPI_COMM_WORLD);
    //Se reciben los datos.
    
    int p[n];
    MPI_Reduce(&prime, p, n, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    
    MPI_Finalize();
    
    double time = end - begin; //Calcula tiempo de computo

    //EL proceso master se encarga de imprimir los resultados
    if(id==0)
    {
        int c=0;
        ///Se crea el archivo de salida
        ofstream outputFile("PrimeNumbersMPI.txt");
        for(int i=0; i<n;i++)
        {
            if(p[i]!=0)  //Se imprimen solo los numeros primos del vector
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

