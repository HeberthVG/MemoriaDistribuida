#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>

#ifndef CACHE_H
#define CACHE_H

struct block{
    int tag;
};

class Cache{
public:
	Cache(int blockSize, int CSize, int asoc);
	///constructor de copia.
	//~Cache();
	///destructor del objeto.
    block read(int index, int asoc);
    //Lee el bloque.
    void write(int index, int asoc, int tag);
    //Escribe en un bloque en la dirección dir.
    void hitRp();
    void missRp();
    void hitWp();
    void missWp();
    //Incrementa el contador del miss o el hit
    int gethitR();
    int getmissR();
    int gethitW();
    int getmissW();
    //Devuelve la cantidad de hit o miss.
    
	
	
private:
	int blockSize, cacheSize, bNum, asoc, bsets, hitR, missR, hitW, missW, boffset, bindex, btag;
    //Se define los parámetros del cache.
    block **mema;
    //Se define el espacio en memoria que contiene los bloques.
};
#endif
