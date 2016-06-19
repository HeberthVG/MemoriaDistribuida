#Se definen los objetos, estos son los archivos que necesitan
#para crear el ejecutable.
OBJS = Cache.cpp
CMP = Cache.o
OBJS1 = Prime.cpp
CMP1 = Prime.o
OBJS2 = PrimeMPI.cpp
CMP2 = PrimeMPI.o

#Se define el compilador
CC = g++
MPI=mpicxx

#Bandera de depuracion
DEBUG = -g

#Banderas de compilacion
CFLAGS = -Wall $(DEBUG) -pedantic -std=c++0x
MPIFLAGS= 


#Banderas para el Linker
LFLAGS = -Wall $(DEBUG) -pedantic

#Archivo Ejecutable que se va a crear
TARGET = Run
TARGET1 = RunPrime
TARGET2 = RunPrimeMPI

#Reglas a ejecutar cuando se ejecute make
all: $(TARGET) $(TARGET1) $(TARGET2)

$(TARGET):
$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -c
	$(CC) $(CFLAGS) -o $(TARGET) main.cpp $(CMP)

$(TARGET1):
$(TARGET1) : $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) -c
	$(CC) $(CFLAGS) -o $(TARGET1) mainPrime.cpp $(CMP1)
	
$(TARGET2):
$(TARGET2) : $(OBJS2)
	$(MPI) $(MPIFLAGS) $(OBJS2) -c
	$(MPI) $(MPIFLAGS) -o $(TARGET2) mainPrimeMPI.cpp $(CMP2)


#Al digitar make clean se borraran todos los archivos compilados,
#note e l −f en rm, s i no sabe para que sirve use e l manual de rm.
clean:
	rm -rf ∗.o *.gch *.dSYM $(CMP)
	rm -rf ∗.o *.gch *.dSYM $(TARGET)
	rm -rf ∗.o *.gch *.dSYM $(CMP1)
	rm -rf ∗.o *.gch *.dSYM $(TARGET1)
	rm -rf ∗.o *.gch *.dSYM $(CMP2)
	rm -rf ∗.o *.gch *.dSYM $(TARGET2)
