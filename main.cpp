#include"Cache.h"

using namespace std;

int main(int argc, char* argv[]) {
    
    unsigned int dir, maskIndex, maskBO, maskTag;
    int i, k, remp;
    
    //tanCache, tamB y asoc deberían ser definidos por argumentos al compilar el programa
    int tamCache=256, tamB=32, numB, asoc = 1, bBoffset, bindex, btag, sets;
    char tipo;
    bool nohit;
    
    if (argc==1) {
        cout << "Por favor ingrese los parámetros del caché." << endl;
        return 1;
    }
    if(atoi(argv[1])<1){
        cout << "No es posible crear un caché con asociatividad 0 o menor." << endl;
        return 1;
    }
    else{
        asoc=atoi(argv[1]);
        tamCache=atoi(argv[2]);
        tamB=atoi(argv[3]);
    }
    
    //Parámetros del caché
    numB = tamCache/tamB;
    bBoffset = log2(tamB);
    sets = numB/asoc;
    bindex = log2(sets);
    btag = 32 - bindex - bBoffset;
    
    Cache newCache(tamB, tamCache, asoc);
    //Se crea un nuevo cache.
    
    //cout << "btag " << btag << " bindex " << bindex << " bBoffset " << bBoffset << endl;
    int Boffset, tag, index;
    
    
    ifstream inst("memory.trace");
    if (inst.is_open()) {
         cout << "Procesando archivo .trace ..." << endl;
    } else {
        cout << "No se encuentra el archivo .trace" << endl;
        return 1;
    }
    //Verifica si el archivo .trace se encuentra en el directorio.
    
    //Mascaras para obtener el tag, index y byte-offset de la direccion
    maskBO = pow(2,bBoffset)-1;
    maskIndex = (pow(2,bindex) - 1) * pow(2,bBoffset);
    maskTag = (pow(2,btag) - 1) * pow(2,bBoffset+bindex);
    
    //Se inicializan contadores
    i = 0;
    srand(time(0));
    while (!inst.eof()) {
        i++;
        inst >> hex >> dir;
        //cout << i << ". Direccion: " << dir;
        
        //cout << " Byte-offset: ";
        Boffset = dir & maskBO;
        //cout << Boffset;
        
        //cout << " Index: ";
        index = (dir & maskIndex) / pow(2,bBoffset);
        //cout << index;
        
        //cout << " Tag: ";
        tag = (dir & maskTag) / pow(2,bBoffset+bindex);
        //cout << tag;
        
        inst >> tipo;
        //cout << " Tipo: " << tipo << endl;
        
        nohit = true;
        
        switch (tipo) {
            case 'L':
                for(k=0; k<asoc; k++) {
                    //Revisa si el tag coincide con todos los posible campos
                    if (tag==newCache.read(index, k).tag) {
                        newCache.hitRp();
                        nohit = false;
                        break;
                    }
                }
                //Si el tag no coincide trae todo un nuevo bloque al cache
                if(nohit) {
                    newCache.missRp();
                    remp = rand()%asoc;
                    newCache.write(index, remp, tag);
                    //Reemplaza uno de los bloques al azar
                }
                break;
            case 'S':
                for(k=0; k<asoc; k++) {
                    //Revisa si el tag coincide con todos los posible campos
                    if (tag==newCache.read(index, k).tag) {
                        newCache.hitWp();
                        nohit = false;
                        break;
                    }
                }
                //Si el tag no coincide entonces se escribe en memoria.
                if(nohit) {
                    newCache.missWp();
                    remp = rand()%asoc;
                    newCache.write(index, remp, tag);
                    //Reemplaza uno de los bloques al azar
                }
                break;
                
            default:
                break;
        }
        
        
    }
    
    float missrateR=100*((float)newCache.getmissR()/((float)newCache.getmissR()+(float)newCache.gethitR()));
    float missrateW=100*(float)newCache.getmissW()/((float)newCache.getmissW()+(float)newCache.gethitW());
    float missrateT=100*((float)newCache.getmissR()+(float)newCache.getmissW())/((float)newCache.getmissR()+(float)newCache.gethitR()+(float)newCache.getmissW()+(float)newCache.gethitW());
    //Calcula los miss rate.
    
    cout << "Catidad de hits por reads: " << newCache.gethitR() << endl;
    cout << "Cantidad de misses por reads: " << newCache.getmissR() << endl;
    cout << "Catidad de hits por writes: " << newCache.gethitW() << endl;
    cout << "Cantidad de misses por writes: " << newCache.getmissW() << endl;
    cout << "Miss rate por reads: " << missrateR << "%. "<< endl;
    cout << "Miss rate por writes: " << missrateW << "%. "<< endl;
    cout << "Miss rate total: " << missrateT << "%. "<< endl;
    //Escribe el resultado en consola.
    
    ofstream outputFile("CacheResults_"+to_string(asoc)+"-"+ to_string(tamCache)+"-" +to_string(tamB)+".txt");
    outputFile << "Resultados de un cache con: " << "\n\t*tamaño de cache: " << tamCache << " \n\t*tamaño de bloque: " << tamB << " \n\t*asociatividad: "<< asoc << " way.\n" << endl;
    outputFile << "Catidad de hits por reads: " << newCache.gethitR() << endl;
    outputFile << "Cantidad de misses por reads: " << newCache.getmissR() << endl;
    outputFile << "Catidad de hits por writes: " << newCache.gethitW() << endl;
    outputFile << "Cantidad de misses por writes: " << newCache.getmissW() << endl;
    outputFile << "Miss rate por reads: " << missrateR << "%. "<< endl;
    outputFile << "Miss rate por writes: " << missrateW << "%. "<< endl;
    outputFile << "Miss rate total: " << missrateT << "%. "<< endl;
    //Escribe el resultado en un archivo de texto.
    
    inst.close();
    outputFile.close();
    return 0;
}
