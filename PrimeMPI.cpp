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
    clock_t begin = clock();
    
    int n=5000;
    int procs, id, largo, rc;
    char host[MPI_MAX_PROCESSOR_NAME];
    
    rc = MPI_Init(&argc, &argv);
    
    MPI_Status stat;
    
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Get_processor_name(host, &largo);
    
    int prime[n];
    if(id==0)
    {
        prime[0]=0;
        prime[1]=0;
        for(int i=2; i<=n;i++) prime[i]=i;
    }
    
    MPI_Bcast(&prime, n, MPI_INT, 0, MPI_COMM_WORLD);
    
    for(int i=2+id; i*i<=n;i+=procs)
    {
        if(prime[i]!=0)
        {
            for(int j=i; j*i<=n;++j)
            {
                prime[j*i]=0;
            }
        }
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    //Se reciben los datos.
    int p[n];
    MPI_Reduce(&prime, p, n, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
    
    MPI_Finalize();

    if(id==0)
    {
        cout<<"Imprimiendo los numeros primos...\n";
        for(int i=2; i<=n;++i)
        {
            if(p[i]!=0)
            {
                cout << ' ' << i;
            }
        }
        cout<<endl;
    }
    clock_t end = clock();
    double time = double(end - begin) / CLOCKS_PER_SEC;
    cout << "\nTiempo de ejecución  " << time << "s\n";
    return 0;
}

//Método para obtener la lista de primos.

