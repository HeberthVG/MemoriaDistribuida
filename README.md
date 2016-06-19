# MemoriaDistribuida

A cache simulator with MESI protocol for data coherence and library OpenMPI for parallelism.

AUTHORS:

    Carlos Eduardo Solano, B36685
    Heberth Valverde, B37174

CHANGELOG:

    -v1: initial release.

REQUIREMENTS:

    -C++ compiler g++.
    -iostream, fstream, cmath, ctime, cstdlib, openmpi libraries.
    -make
    
INSTALL:

    $ git clone https://github.com/HeberthVG/MemoriaDistribuida
    $ cd MemoriaDistribuida
    $ make

RUN:

    For test MESI protocol:
    $ ./Run  

NOTE: please put and rename "*.trace" to "memory.trace" into ~/"path-to"/MemoriaDistribuida/
