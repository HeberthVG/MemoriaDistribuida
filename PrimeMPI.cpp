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

#include "PrimeMPI.h"

using namespace std;

//Se define el constructor.

PrimeMPI::PrimeMPI(int argc, char ** argv, int n)
{

		this->n=n;
		
		int procs, id, largo, rc;
		char host[MPI_MAX_PROCESSOR_NAME];
	
		rc = MPI_Init(&argc, &argv);
		
		MPI_Status stat;
		
		MPI_Comm_size(MPI_COMM_WORLD, &procs);
		MPI_Comm_rank(MPI_COMM_WORLD, &id);
		MPI_Get_processor_name(host, &largo);
		
		this->n=n;
		if(id==0)
		{
   			primeV=new int [n]();
   			for(int i=2; i<=n;i++) primeV[i]=i;	
   		}
   		
   		MPI_Bcast(&primeV, n, MPI_INT, 0, MPI_COMM_WORLD);
   		
   		for(int i=2+id; i*i<=n;i+=procs)
   		{
    		if(this->primeV[i]!=0)
    		{
    			for(int j=i; j*i<=n;++j) 
    			{
    				primeV[j*i]=0;
    			}
    		}
    	}
    	
    	MPI_Barrier(MPI_COMM_WORLD);
    	//Se reciben los datos.
    	MPI_Reduce(&primeV, primeV, n, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
    
    	MPI_Finalize();
    	this->primeV=primeV;
}

void PrimeMPI::print(){
    cout<<"Imprimiendo los numeros primos...\n";
	for(int i=2; i<=n;++i)
    {
    	if(primeV[i]!=0)
    	{
			cout << ' ' << i;
		}
	}
	cout<<endl;
}

//Método para obtener la lista de primos.

