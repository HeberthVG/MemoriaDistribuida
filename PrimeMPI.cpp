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
		
		int *primeVloc;
		this->n=n;
		if(id==0)
		{
   			primeV=new int [n]();
   			for(int i=2; i<=n;i++) primeV[i]=i;	
   		}
   		else
   		{
   			primeVloc=new int [n]();
   			for(int i=2; i<=n;i++) primeVloc[i]=i;	
   		}
   		
   		for(int i=2; i<=n;i++) this->primeV[i]=i;
   		for(int i=2+id; i*i<=n;i+procs)
   		{
    		if(this->primeV[i]!=0)
    		{
    			for(int j=i; j*i<=n;++j) 
    			{
    				if(id!=0)
    				{
    					primeVloc[j*i]=0;
    				}
    				else
    				{
    					primeV[j*i]=0;
    				}
    			}
    		}
    	}
    	
    	//Se reciben los datos.
    	if(id!=0)
    	{
    		MPI_Send(&primeVloc, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
    	}
    	else
    	{
    		for(int i=1; i<procs; i++)
    		{
    			MPI_Recv(&primeVloc, n, MPI_INT, i, 0, MPI_COMM_WORLD, &stat);
    			for(int j=0; j<=n; j++)
    			{
    				if(primeVloc[j]==0)
    				{
    					primeV[j]=0;
    				}
    				
    			}
			}	
    	}
    
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

