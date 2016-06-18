#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>

#ifndef CACHE_H
#define CACHE_H

struct block{
    int tag;
    int state;
};

class Cache{
public:
    ///Constructor.
    Cache(int blockSize, int CSize, int asoc);

    ///Lee el bloque.
    block read(int index, int asoc);
    ///Escribe en un bloque en la dirección dir.
    void write(int index, int asoc, int tag);
    ///Incrementa el contador del miss o el hit
    void hitRp();
    void missRp();
    void hitWp();
    void missWp();
    ///Devuelve la cantidad de hit o miss.    
    int gethitR();
    int getmissR();
    int gethitW();
    int getmissW();
    ///Cambia el estado de una linea del cache
    void setState(int index, int asoc, int state);
	
private:
    ///Se define los parámetros del cache.
    int blockSize, cacheSize, bNum, asoc, bsets, hitR, missR, hitW, missW, boffset, bindex, btag;
    ///Se define el espacio en memoria que contiene los bloques.
    block **mema;    
};
#endif
