#include"Cache.h"

using namespace std;

int main(int argc, char* argv[]) {
    
    unsigned int dir, maskIndexL1, maskIndexL2, maskOffSet, maskTagL1, maskTagL2, tmpDir;
    int i, remp, CI;
    
    ///Caracteristicas de los caches
    int tamL1 = 8000, tamL2 = 64000, tamB = 16, asoc = 1;
    ///Variables de cada cache
    int numBL1, numBL2, setsL1, setsL2, bOffSet, bIndexL1, bIndexL2, bTagL1, bTagL2;
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
    
    int OffSet, tagL2, tagL1, indexL2, indexL1, tmpIndex, tmpTag;
    
    
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

    ///Se crea el archivo de salida
    ofstream outputFile("MESI_Protocol.txt");
    while (!inst.eof()) { ///Verifica si faltan lineas por leer
        CI++;

	///Se lee la direccion y se calcula el ByteOffset, el index y el tag
        inst >> hex >> dir;
        
        OffSet = dir & maskOffSet;

        indexL1 = (dir & maskIndexL1) / pow(2,bOffSet);
        indexL2 = (dir & maskIndexL2) / pow(2,bOffSet);

        tagL1 = (dir & maskTagL1) / pow(2,bOffSet+bIndexL1);
        tagL2 = (dir & maskTagL2) / pow(2,bOffSet+bIndexL2);
        
        inst >> tipo;
        
        nohit = true;
        
	///Si la instruccion es par se llama al CPU1 (CacheL12)
	if(!(CI%2)) {
            switch (tipo) {
		case 'L':
                    for(i=0; i<asoc; i++) {
			remp = rand()%asoc;
                	///Revisa si el tag coincide con todos los posible campos y es valido
                	if (tagL1==CacheL12.read(indexL1, i).tag && CacheL12.read(indexL1, i).state!=3) {
                            nohit = false;
			    ///Comprueba si el dato esta en el otro cache
			    if (tagL1==CacheL11.read(indexL1, i).tag) {
				switch (CacheL11.read(indexL1, i).state) {
				    ///Modified
				    case 0:
					CacheL12.missRp();
					///Se debe actualizar el dato en la memoria anterior
					CacheL2.write(indexL2, remp, tagL2);
					CacheL2.setState(indexL2, remp, 2);
					///Si el dato de L12 esta modificado se debe escribir antes de reemplazarse
					if (!CacheL12.read(indexL1, i).state) {
					    ///Se debe recuperar la direccion almacenada en L12
					    tmpDir = CacheL12.read(indexL1, i).tag * pow(2,bOffSet+bIndexL1);
					    tmpDir += indexL1 * pow(2,bOffSet) + OffSet;
					    tmpIndex = (tmpDir & maskIndexL2) / pow(2,bOffSet);
					    tmpTag = (tmpDir & maskTagL2) / pow(2,bOffSet+bIndexL2);
					    CacheL2.write(tmpIndex, remp, tmpTag);
					    CacheL2.setState(tmpIndex, remp, 1);
					}
					///Se copia el dato actualizado en el cache L12
					CacheL12.write(indexL1, remp, tagL1);
					CacheL12.setState(indexL1, remp, 2);
					CacheL11.setState(indexL1, remp, 2);
					break;
				    ///Exclusive
				    case 1:
					CacheL12.missRp();
					///Si el dato de L12 esta modificado se debe escribir antes de reemplazarse
					if (!CacheL12.read(indexL1, i).state) {
					    ///Se debe recuperar la direccion almacenada en L12
					    tmpDir = CacheL12.read(indexL1, i).tag * pow(2,bOffSet+bIndexL1);
					    tmpDir += indexL1 * pow(2,bOffSet) + OffSet;
					    tmpIndex = (tmpDir & maskIndexL2) / pow(2,bOffSet);
					    tmpTag = (tmpDir & maskTagL2) / pow(2,bOffSet+bIndexL2);
					    CacheL2.write(tmpIndex, remp, tmpTag);
					    CacheL2.setState(tmpIndex, remp, 1);
					}
					CacheL12.write(indexL1, remp, tagL1);
					CacheL12.setState(indexL1, remp, 2);
					CacheL11.setState(indexL1, remp, 2);
					CacheL2.setState(indexL2, remp, 2);
					break;
				    ///Shared
				    case 2:
                            		CacheL12.hitRp();
					CacheL12.setState(indexL1, remp, 2);
					CacheL2.setState(indexL2, remp, 2);
					break;
				    ///Invalid
				    case 3:
                            		CacheL12.hitRp();
					CacheL12.setState(indexL1, remp, 1);
					CacheL2.setState(indexL2, remp, 1);
					break;
				    default:
					break;
				}
			    } else {
				CacheL12.hitRp();
				CacheL12.setState(indexL1, remp, 1);
				CacheL2.setState(indexL1, remp, 1);
			    }
                            break;
                        }
                    }
                    ///Si el tag no coincide o el dato es invalido trae todo un nuevo bloque al cache
                    if(nohit) {
                        CacheL12.missRp();
			///Si el dato de L12 esta modificado se debe escribir antes de reemplazarse
			if (!CacheL12.read(indexL1, i).state) {
			    ///Se debe recuperar la direccion almacenada en L12
			    tmpDir = CacheL12.read(indexL1, i).tag * pow(2,bOffSet+bIndexL1);
			    tmpDir += indexL1 * pow(2,bOffSet) + OffSet;
			    tmpIndex = (tmpDir & maskIndexL2) / pow(2,bOffSet);
			    tmpTag = (tmpDir & maskTagL2) / pow(2,bOffSet+bIndexL2);
			    CacheL2.write(tmpIndex, remp, tmpTag);
			    CacheL2.setState(tmpIndex, remp, 1);
			}
			///Comprueba si el dato esta en el otro cache
			if (tagL1==CacheL11.read(indexL1, 0).tag) {
			    switch (CacheL11.read(indexL1, 0).state) {
				///Modified
				case 0:
				    ///Se debe actualizar el dato en la memoria anterior
				    CacheL2.write(indexL2, remp, tagL2);
				    CacheL2.setState(indexL2, remp, 2);
				    ///Se copia el dato actualizado en el cache L12
				    CacheL12.write(indexL1, remp, tagL1);
				    CacheL12.setState(indexL1, remp, 2);
				    CacheL11.setState(indexL1, remp, 2);
				    break;
				///Exclusive
				case 1:
				    CacheL12.write(indexL1, remp, tagL1);
				    CacheL12.setState(indexL1, remp, 2);
				    CacheL11.setState(indexL1, remp, 2);
				    CacheL2.setState(indexL2, remp, 2);
				    break;
				///Shared
				case 2:
				    CacheL12.write(indexL1, remp, tagL1);
				    CacheL12.setState(indexL1, remp, 2);
				    CacheL2.setState(indexL2, remp, 2);
				    break;
				///Invalid
				case 3:
				    CacheL12.write(indexL1, remp, tagL1);
				    CacheL12.setState(indexL1, remp, 1);
				    CacheL2.setState(indexL2, remp, 1);
				    break;
				default:
				    break;
			    }
			} else if (tagL2==CacheL2.read(indexL2, 0).tag) { ///Verifica si esta en el cache L2
			    CacheL12.write(indexL1, remp, tagL1);
			    CacheL12.setState(indexL1, remp, 1);
			    CacheL2.setState(indexL2, remp, 1);
			} else { ///Se trae el bloque primero al cache L2
			    CacheL2.write(indexL2, remp, tagL2);
			    CacheL12.write(indexL1, remp, tagL1);
			    CacheL12.setState(indexL1, remp, 1);
			    CacheL2.setState(indexL2, remp, 1);
			} 
                    }
                    break;
                case 'S':
		    remp = rand()%asoc;
                    for(i=0; i<asoc; i++) {
			///Revisa si el tag coincide con todos los posible campos y el dato es valido
                        if (tagL1==CacheL12.read(indexL1, i).tag && CacheL12.read(indexL1, i).state!=3) {
                           CacheL12.hitWp();
                           nohit = false;								
                           break;
                        }
                    }
                    ///Si el tag no coincide debe traerse la instruccion de la memoria anterior.
                    if(nohit) {
                        CacheL12.missWp();
			if (tagL2!=CacheL2.read(indexL2, 0).tag) { ///Verifica si esta en el cache L2
			    CacheL2.write(indexL2, remp, tagL2);
			}
			///Si el dato de L12 esta modificado se debe escribir antes de reemplazarse
			if (!CacheL12.read(indexL1, i).state) {
			    ///Se debe recuperar la direccion almacenada en L12
			    tmpDir = CacheL12.read(indexL1, i).tag * pow(2,bOffSet+bIndexL1);
			    tmpDir += indexL1 * pow(2,bOffSet) + OffSet;
			    tmpIndex = (tmpDir & maskIndexL2) / pow(2,bOffSet);
			    tmpTag = (tmpDir & maskTagL2) / pow(2,bOffSet+bIndexL2);
			    CacheL2.write(tmpIndex, remp, tmpTag);
			    CacheL2.setState(tmpIndex, remp, 1);
			}
			///Reemplaza uno de los bloques al azar
			CacheL12.write(indexL1, remp, tagL1);     
                    }
		    ///Si esta en otro cache se debe invalidar
		    if (tagL1==CacheL11.read(indexL1, 0).tag) {
			CacheL11.setState(indexL1, remp, 3);
		    }
		    CacheL12.setState(indexL1, remp, 0);
		    CacheL2.setState(indexL2, remp, 1);
                    break;     
                default:
                    break;
	    }
	} else { ////Si es impar llama al CPU0
            switch (tipo) {
		case 'L':
                    for(i=0; i<asoc; i++) {
			remp = rand()%asoc;
                	///Revisa si el tag coincide con todos los posible campos y es valido
                	if (tagL1==CacheL11.read(indexL1, i).tag && CacheL11.read(indexL1, i).state!=3) {
                            nohit = false;
			    ///Comprueba si el dato esta en el otro cache
			    if (tagL1==CacheL12.read(indexL1, i).tag) {
				switch (CacheL12.read(indexL1, i).state) {
				    ///Modified
				    case 0:
					CacheL11.missRp();
					///Se debe actualizar el dato en la memoria anterior
					CacheL2.write(indexL2, remp, tagL2);
					CacheL2.setState(indexL2, remp, 2);
					///Si el dato de L11 esta modificado se debe escribir antes de reemplazarse
					if (!CacheL11.read(indexL1, i).state) {
					    ///Se debe recuperar la direccion almacenada en L11
					    tmpDir = CacheL11.read(indexL1, i).tag * pow(2,bOffSet+bIndexL1);
					    tmpDir += indexL1 * pow(2,bOffSet) + OffSet;
					    tmpIndex = (tmpDir & maskIndexL2) / pow(2,bOffSet);
					    tmpTag = (tmpDir & maskTagL2) / pow(2,bOffSet+bIndexL2);
					    CacheL2.write(tmpIndex, remp, tmpTag);
					    CacheL2.setState(tmpIndex, remp, 1);
					}
					///Se copia el dato actualizado en el cache L11
					CacheL11.write(indexL1, remp, tagL1);
					CacheL11.setState(indexL1, remp, 2);
					CacheL12.setState(indexL1, remp, 2);
					break;
				    ///Exclusive
				    case 1:
					CacheL11.missRp();
					///Si el dato de L11 esta modificado se debe escribir antes de reemplazarse
					if (!CacheL11.read(indexL1, i).state) {
					    ///Se debe recuperar la direccion almacenada en L11
					    tmpDir = CacheL11.read(indexL1, i).tag * pow(2,bOffSet+bIndexL1);
					    tmpDir += indexL1 * pow(2,bOffSet) + OffSet;
					    tmpIndex = (tmpDir & maskIndexL2) / pow(2,bOffSet);
					    tmpTag = (tmpDir & maskTagL2) / pow(2,bOffSet+bIndexL2);
					    CacheL2.write(tmpIndex, remp, tmpTag);
					    CacheL2.setState(tmpIndex, remp, 1);
					}
					CacheL11.write(indexL1, remp, tagL1);
					CacheL11.setState(indexL1, remp, 2);
					CacheL12.setState(indexL1, remp, 2);
					CacheL2.setState(indexL2, remp, 2);
					break;
				    ///Shared
				    case 2:
                            		CacheL11.hitRp();
					CacheL11.setState(indexL1, remp, 2);
					CacheL2.setState(indexL2, remp, 2);
					break;
				    ///Invalid
				    case 3:
                            		CacheL11.hitRp();
					CacheL11.setState(indexL1, remp, 1);
					CacheL2.setState(indexL2, remp, 1);
					break;
				    default:
					break;
				}
			    } else {
				CacheL11.hitRp();
				CacheL11.setState(indexL1, remp, 1);
				CacheL2.setState(indexL1, remp, 1);
			    }
                            break;
                        }
                    }
                    ///Si el tag no coincide o el dato es invalido trae todo un nuevo bloque al cache
                    if(nohit) {
                        CacheL11.missRp();
			///Si el dato de L11 esta modificado se debe escribir antes de reemplazarse
			if (!CacheL11.read(indexL1, i).state) {
			    ///Se debe recuperar la direccion almacenada en L11
			    tmpDir = CacheL11.read(indexL1, i).tag * pow(2,bOffSet+bIndexL1);
			    tmpDir += indexL1 * pow(2,bOffSet) + OffSet;
			    tmpIndex = (tmpDir & maskIndexL2) / pow(2,bOffSet);
			    tmpTag = (tmpDir & maskTagL2) / pow(2,bOffSet+bIndexL2);
			    CacheL2.write(tmpIndex, remp, tmpTag);
			    CacheL2.setState(tmpIndex, remp, 1);
			}
			///Comprueba si el dato esta en el otro cache
			if (tagL1==CacheL12.read(indexL1, 0).tag) {
			    switch (CacheL12.read(indexL1, 0).state) {
				///Modified
				case 0:
				    ///Se debe actualizar el dato en la memoria anterior
				    CacheL2.write(indexL2, remp, tagL2);
				    CacheL2.setState(indexL2, remp, 2);
				    ///Se copia el dato actualizado en el cache L12
				    CacheL11.write(indexL1, remp, tagL1);
				    CacheL11.setState(indexL1, remp, 2);
				    CacheL12.setState(indexL1, remp, 2);
				    break;
				///Exclusive
				case 1:
				    CacheL11.write(indexL1, remp, tagL1);
				    CacheL11.setState(indexL1, remp, 2);
				    CacheL12.setState(indexL1, remp, 2);
				    CacheL2.setState(indexL2, remp, 2);
				    break;
				///Shared
				case 2:
				    CacheL11.write(indexL1, remp, tagL1);
				    CacheL11.setState(indexL1, remp, 2);
				    CacheL2.setState(indexL2, remp, 2);
				    break;
				///Invalid
				case 3:
				    CacheL11.write(indexL1, remp, tagL1);
				    CacheL11.setState(indexL1, remp, 1);
				    CacheL2.setState(indexL2, remp, 1);
				    break;
				default:
				    break;
			    }
			} else if (tagL2==CacheL2.read(indexL2, 0).tag) { ///Verifica si esta en el cache L2
			    CacheL11.write(indexL1, remp, tagL1);
			    CacheL11.setState(indexL1, remp, 1);
			    CacheL2.setState(indexL2, remp, 1);
			} else { ///Se trae el bloque primero al cache L2
			    CacheL2.write(indexL2, remp, tagL2);
			    CacheL11.write(indexL1, remp, tagL1);
			    CacheL11.setState(indexL1, remp, 1);
			    CacheL2.setState(indexL2, remp, 1);
			} 
                    }
                    break;
                case 'S':
		    remp = rand()%asoc;
                    for(i=0; i<asoc; i++) {
			///Revisa si el tag coincide con todos los posible campos y el dato es valido
                        if (tagL1==CacheL11.read(indexL1, i).tag && CacheL11.read(indexL1, i).state!=3) {
                           CacheL11.hitWp();
                           nohit = false;
                           break;
                        }
                    }
                    ///Si el tag no coincide debe traerse la instruccion de la memoria anterior.
                    if(nohit) {
			///Si el dato de L11 esta modificado se debe escribir antes de reemplazarse
			if (!CacheL11.read(indexL1, i).state) {
			    ///Se debe recuperar la direccion almacenada en L11
			    tmpDir = CacheL11.read(indexL1, i).tag * pow(2,bOffSet+bIndexL1);
			    tmpDir += indexL1 * pow(2,bOffSet) + OffSet;
			    tmpIndex = (tmpDir & maskIndexL2) / pow(2,bOffSet);
			    tmpTag = (tmpDir & maskTagL2) / pow(2,bOffSet+bIndexL2);
			    CacheL2.write(tmpIndex, remp, tmpTag);
			    CacheL2.setState(tmpIndex, remp, 1);
			}
                        CacheL11.missWp();
			if (tagL2!=CacheL2.read(indexL2, 0).tag) { ///Verifica si esta en el cache L2
			    CacheL2.write(indexL2, remp, tagL2);
			}
			///Reemplaza uno de los bloques al azar
			CacheL11.write(indexL1, remp, tagL1);    
                    }
		    ///Si esta en otro cache se debe invalidar
		    if (tagL1==CacheL12.read(indexL1, 0).tag) {
			CacheL12.setState(indexL1, remp, 3);
		    }
		    CacheL11.setState(indexL1, remp, 0);
		    CacheL2.setState(indexL2, remp, 1); 
                    break;     
                default:
                    break;
	    }
	} 
	if (CI>49642108 && CI<49642129) {///Guarda el estado de los datos para las ultimas 20 instrucciones ejecutadas
	    outputFile << "Instrucción: " << CI << ". Dirección: " << dir << ". Estado Cache L2: " << CacheL2.read(indexL2, 0).state << endl;
	    outputFile << "Tag Cache L11: " << CacheL11.read(indexL1, 0).tag <<  ". Estado Cache L11: " << CacheL11.read(indexL1, 0).state << ". Tag Cache L12: " << CacheL12.read(indexL1, 0).tag << ". Estado Cache L12: " << CacheL12.read(indexL1, 0).state << endl;
	}  
    }
    
    ///Escribe el resultado en consola.
    cout << "El estado final de los datos para las últimas 10 instrucciones de cada procesador se muestra en el archivo MESI_Protocol.txt" << endl;
    
    ///Se cierran los archivos
    inst.close();
    outputFile.close();
    return 0;
}
