#Se definen los objetos, estos son los archivos que necesitan
#para crear el ejecutable.
OBJS = Cache.cpp
CMP = Cache.o

#Se define el compilador
CC = g++

#Bandera de depuracion
DEBUG = -g

#Banderas de compilacion
CFLAGS = -Wall $(DEBUG) -pedantic -std=c++0x

#Banderas para el Linker
LFLAGS = -Wall $(DEBUG) -pedantic

#Archivo Ejecutable que se va a crear
TARGET = Run

#Reglas a ejecutar cuando se ejecute make
all: $(TARGET)
$(TARGET):
$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -c
	$(CC) $(CFLAGS) -o $(TARGET) main.cpp $(CMP)


#Al digitar make clean se borraran todos los archivos compilados,
#note e l −f en rm, s i no sabe para que sirve use e l manual de rm.
clean:
	rm -rf ∗.o *.gch *.dSYM $(CMP)
	rm -rf ∗.o *.gch *.dSYM $(TARGET)
