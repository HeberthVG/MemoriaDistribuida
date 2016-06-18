/*
    Universidad de Costa Rica
    Escuela de Ingeniería Eléctrica
    Estructuras de Computadoras Digitales II
    IE-0521
    Tarea Programada 2
    Carlos Solano       B36685
    Heberth Valverde    B37174
*/

// Coherencia de datos usando MESI

#include "Cache.h"

using namespace std;


Cache::Cache(int bS, int S, int as)
{
    ///Parametros del cache.
    this->blockSize=bS;
    this->cacheSize=S;
    this->bNum=S/bS;
    this->asoc=as;
    if(asoc>0){
        this->bsets=bNum/as;
    }
    else this->bsets=bNum;
    ///Inicializa los contadores de misses y hits.
    this->hitR=0;
    this->missR=0;
    this->hitW=0;
    this->missW=0;
    ///Cantidad de bits necesarios por bloque.
    this->boffset=log2(bS);
    this->bindex=log2(bsets);
    this->btag=bS-bindex-boffset;
    
    ///Se crea la matriz.
    mema=new block *[bsets];
    for(int a=0;a<bsets;a++)
    {
        mema[a]=new block[as];
    }
    
    ///Inicializar cache.
    for(int a=0;a<bsets;a++)
    {
        for(int b=0;b<as;b++)
        {
	    this->mema[a][b].tag = 0;
	    this->mema[a][b].state = 3;    
        }
    }
}
///Se define el constructor del cache, se inicializa una matriz de structs block que contienen el tag de cada bloque. Se inicializan los parametros del cache y el espacio en memoria.

block Cache::read(int index, int asoc){
    return this->mema[index][asoc];
}
void Cache::write(int index, int asoc, int tag){
    this->mema[index][asoc].tag=tag;
}

///Metodos para incrementar los contadores.
void Cache::hitRp(){
    this->hitR++;
}
void Cache::missRp(){
    this->missR++;
}
void Cache::hitWp(){
    this->hitW++;
}
void Cache::missWp(){
    this->missW++;
}

///Metodos para obtener el valor de los contadores.
int Cache::gethitR(){
    return this->hitR;
}
int Cache::getmissR(){
    return this->missR;
}
int Cache::gethitW(){
    return this->hitW;
}
int Cache::getmissW(){
    return this->missW;
}

///Metodo para cambiar el estado de una linea de cache
void Cache::setState(int index, int asoc, int state) {
    this->mema[index][asoc].state = state;
}
