#include"Cache.h"

using namespace std;

int main(int argc, char* argv[]) {
    
    unsigned int dir, maskIndexL1, maskIndexL2, maskOffSet, maskTagL1, maskTagL2;
    int i, remp, CI;
    
    ///Caracteristicas de los caches
    int tamL1 = 8000, tamL2 = 64000, tamB = 16, asoc = 1;
    ///Variables de cada cache
    int numBL1, numBL2, setsL1, setsL2, bOffSet, bIndexL1, bIndexL12, bIndexL2, bTagL1, bTagL12, bTagL2;
    char tipo;
    bool nohit;
    
    ///Parametros de los caches
    numBL1 = tamL1/tamB;
    numBL2 = tamL2/tamB;
    bOffSet = log2(tamB);
    setsL1 = numBL1/asoc;
    setsL2 = numBL2/asoc;
    bIndexL1 = log2(setsL1);
    bIndexL2 = log2(setsL2);
    bTagL1 = 32 - bIndexL1 - bOffSet;
    bTagL2 = 32 - bIndexL2 - bOffSet;
    
    ///Se crean los caches locales y el compartido
    Cache CacheL11(tamB, tamL1, asoc);
    Cache CacheL12(tamB, tamL1, asoc);
    Cache CacheL2(tamB, tamL2, asoc);
    
    //cout << "btag " << btag << " bindex " << bindex << " bBoffset " << bBoffset << endl;
    int OffSet, tagL2, tagL1, indexL2, indexL1;
    
    
    ///Verifica si el archivo .trace se encuentra en el directorio.
    ifstream inst("aligned.trace");
    if (inst.is_open()) {
         cout << "Procesando archivo aligned.trace ..." << endl;
    } else {
        cout << "No se encuentra el archivo aligned.trace" << endl;
        return 1;
    }
    
    ///Mascaras para obtener el tag, index y byte-offset de la direccion
    maskOffSet = pow(2,bOffSet)-1;
    maskIndexL1 = (pow(2,bIndexL1) - 1) * pow(2,bOffSet);
    maskIndexL2 = (pow(2,bIndexL2) - 1) * pow(2,bOffSet);
    maskTagL1 = (pow(2,bTagL1) - 1) * pow(2,bOffSet+bIndexL1);
    maskTagL2 = (pow(2,bTagL2) - 1) * pow(2,bOffSet+bIndexL2);
    
    ///Se inicializan contadores
    CI = 0;
    srand(time(0));
    while (!inst.eof()) { ///Verifica si faltan lineas por leer
        CI++;
        inst >> hex >> dir;
        //cout << i << ". Direccion: " << dir;
        
        //cout << " Byte-offset: ";
        OffSet = dir & maskOffSet;
        //cout << Boffset;
        
        //cout << " Index: ";
        indexL1 = (dir & maskIndexL1) / pow(2,bOffSet);
        indexL2 = (dir & maskIndexL2) / pow(2,bOffSet);
        //cout << index;
        
        //cout << " Tag: ";
        tagL1 = (dir & maskTagL1) / pow(2,bOffSet+bIndexL1);
        tagL2 = (dir & maskTagL2) / pow(2,bOffSet+bIndexL2);
        //cout << tag;
        
        inst >> tipo;
        //cout << " Tipo: " << tipo << endl;
        
        nohit = true;
        
	///Si la instruccion es par se llama al CPU1 (CacheL12)
	if(CI%2) { 
            switch (tipo) {
		case 'L':
                    for(i=0; i<asoc; i++) {
			remp = rand()%asoc;
                	///Revisa si el tag coincide con todos los posible campos
                	if (tagL1==CacheL12.read(indexL1, i).tag) {
                            nohit = false;
			    ///Comprueba si el dato esta en el otro cache
			    if (tagL1==CacheL11.read(indexL1, i).tag) {
				switch (CacheL11.read(indexL1, i).state) {
				    ///Modified
				    case 0:
					CacheL12.missRp();
					CacheL12.write(indexL1, remp, tagL1);
					CacheL12.setState(indexL1, remp, 2);
					CacheL11.setState(indexL1, remp, 2);
					break;
				    ///Exclusive
				    case 1:
					CacheL12.missRp();
					CacheL12.write(indexL1, remp, tagL1);
					CacheL12.setState(indexL1, remp, 2);
					CacheL11.setState(indexL1, remp, 2);
					break;
				    ///Invalid
				    case 3:
                            		CacheL12.hitRp();
					CacheL12.setState(indexL1, remp, 1);
					break;
				    default:
                            		CacheL12.hitRp();
					CacheL12.setState(indexL1, remp, 2);
					break;
				}
			    } else {
				CacheL12.hitRp();
				CacheL12.setState(indexL1, remp, 1);
			    }
                            break;
                        }
                    }
                    ///Si el tag no coincide trae todo un nuevo bloque al cache
                    if(nohit) {
			///Reemplaza uno de los bloques al azar
                        CacheL12.missRp();
			///Comprueba si el dato esta en el otro cache
			if (tagL1==CacheL11.read(indexL1, i).tag) {
			    switch (CacheL11.read(indexL1, i).state) {
				///Modified
				case 0:
				    CacheL12.write(indexL1, remp, tagL1);
				    CacheL12.setState(indexL1, remp, 2);
				    CacheL11.setState(indexL1, remp, 2);
				    break;
				///Exclusive
				case 1:
				    CacheL12.write(indexL1, remp, tagL1);
				    CacheL12.setState(indexL1, remp, 2);
				    CacheL11.setState(indexL1, remp, 2);
				    break;
				///Invalid
				case 3:
				    CacheL12.write(indexL1, remp, tagL1);
				    CacheL12.setState(indexL1, remp, 1);
				    break;
				default:
				    CacheL12.write(indexL1, remp, tagL1);
				    CacheL12.setState(indexL1, remp, 2);
				    break;
			    }
			} else {
			    CacheL12.write(indexL1, remp, tagL1);
			    CacheL12.setState(indexL1, remp, 1);
			}   
                    }
                    break;
                case 'S':
		    remp = rand()%asoc;
                    for(i=0; i<asoc; i++) {
			///Revisa si el tag coincide con todos los posible campos
                        if (tagL1==CacheL12.read(indexL1, i).tag) {
                           CacheL12.hitWp();
                           nohit = false;
                           break;
                        }
                    }
                    ///Si el tag no coincide entonces se escribe en memoria.
                    if(nohit) {
			///Reemplaza uno de los bloques al azar
                        CacheL12.missWp();
                        CacheL12.write(indexL1, remp, tagL1);     
                    }
		    CacheL12.setState(indexL1, remp, 0);
		    CacheL11.setState(indexL1, remp, 3);
                    break;     
                default:
                    break;
	    }
	} else { ////Si es impar llama al CPU0
            switch (tipo) {
		case 'L':
                    for(i=0; i<asoc; i++) {
                	///Revisa si el tag coincide con todos los posible campos
                	if (tagL1==CacheL11.read(indexL1, i).tag) {
                            CacheL11.hitRp();
                            nohit = false;
                            break;
                        }
                    }
                    ///Si el tag no coincide trae todo un nuevo bloque al cache
                    if(nohit) {
			///Reemplaza uno de los bloques al azar
                        CacheL11.missRp();
                        remp = rand()%asoc;
                        CacheL11.write(indexL1, remp, tagL1);    
                    }
                    break;
                case 'S':
                    for(i=0; i<asoc; i++) {
			///Revisa si el tag coincide con todos los posible campos
                        if (tagL1==CacheL11.read(indexL1, i).tag) {
                           CacheL11.hitWp();
                           nohit = false;
                           break;
                        }
                    }
                    ///Si el tag no coincide entonces se escribe en memoria.
                    if(nohit) {
			///Reemplaza uno de los bloques al azar
                        CacheL11.missWp();
                        remp = rand()%asoc;
                        CacheL11.write(indexL1, remp, tagL1);     
                    }
                    break;     
                default:
                    break;
	    }
	}        
        
    }//
   
    ///Calcula los miss rate. 
    float missrateRL11=100*((float)CacheL11.getmissR()/((float)CacheL11.getmissR()+(float)CacheL11.gethitR()));
    float missrateWL11=100*(float)CacheL11.getmissW()/((float)CacheL11.getmissW()+(float)CacheL11.gethitW());
    float missrateTL11=100*((float)CacheL11.getmissR()+(float)CacheL11.getmissW())/((float)CacheL11.getmissR()+(float)CacheL11.gethitR()+(float)CacheL11.getmissW()+(float)CacheL11.gethitW());
    float missrateRL12=100*((float)CacheL12.getmissR()/((float)CacheL12.getmissR()+(float)CacheL12.gethitR()));
    float missrateWL12=100*(float)CacheL12.getmissW()/((float)CacheL12.getmissW()+(float)CacheL12.gethitW());
    float missrateTL12=100*((float)CacheL12.getmissR()+(float)CacheL12.getmissW())/((float)CacheL12.getmissR()+(float)CacheL12.gethitR()+(float)CacheL12.getmissW()+(float)CacheL12.gethitW());
    
    ///Escribe el resultado en consola.
    cout << "Catidad de hits por reads para L11: " << CacheL11.gethitR() << endl;
    cout << "Cantidad de misses por reads para L11: " << CacheL11.getmissR() << endl;
    cout << "Catidad de hits por writes para L11: " << CacheL11.gethitW() << endl;
    cout << "Cantidad de misses por writes para L11: " << CacheL11.getmissW() << endl;
    cout << "Miss rate por reads para L11: " << missrateRL11 << "%. "<< endl;
    cout << "Miss rate por writes para L11: " << missrateWL11 << "%. "<< endl;
    cout << "Miss rate total para L11: " << missrateTL11 << "%. "<< endl;
    cout << "Catidad de hits por reads para L12: " << CacheL12.gethitR() << endl;
    cout << "Cantidad de misses por reads para L12: " << CacheL12.getmissR() << endl;
    cout << "Catidad de hits por writes para L12: " << CacheL12.gethitW() << endl;
    cout << "Cantidad de misses por writes para L12: " << CacheL12.getmissW() << endl;
    cout << "Miss rate por reads para L12: " << missrateRL12 << "%. "<< endl;
    cout << "Miss rate por writes para L12: " << missrateWL12 << "%. "<< endl;
    cout << "Miss rate total para L12: " << missrateTL12 << "%. "<< endl;
    
    /*ofstream outputFile("CacheResults_"+to_string(asoc)+"-"+ to_string(tamL1)+"-" +to_string(tamB)+".txt");
    outputFile << "Resultados de un cache con: " << "\n\t*tamaño de cache: " << tamL1 << " \n\t*tamaño de bloque: " << tamB << " \n\t*asociatividad: "<< asoc << " way.\n" << endl;
    outputFile << "Catidad de hits por reads: " << CacheL11.gethitR() << endl;
    outputFile << "Cantidad de misses por reads: " << CacheL11.getmissR() << endl;
    outputFile << "Catidad de hits por writes: " << CacheL11.gethitW() << endl;
    outputFile << "Cantidad de misses por writes: " << CacheL11.getmissW() << endl;
    outputFile << "Miss rate por reads: " << missrateR << "%. "<< endl;
    outputFile << "Miss rate por writes: " << missrateW << "%. "<< endl;
    outputFile << "Miss rate total: " << missrateT << "%. "<< endl;*/
    //Escribe el resultado en un archivo de texto.
    
    inst.close();
    //outputFile.close();
    return 0;
}
