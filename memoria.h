/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#ifndef P0_SO_MEMORIA_H
#define P0_SO_MEMORIA_H
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum {
    malloc, shared, mapped
}tmem;

typedef struct{
    int direccion;
    int tamano;
    //cuando se colocó -> mirar tipo
    tmem tipo;
    key_t clave;
    char nombre_archivo[MAX_LENGHT_PATH];
    int file_descriptor;
    //campos restantes
}tItemM;

typedef struct tNode *tPosM;
struct  tNode {
    tItemM data;
    tPosM next;
    tPosM last;
};

typedef tPosM tHistMem;

bool createMem(tHistMem *L);
bool isEmptyMem (tHistMem L);
tPosM firstBlock (tHistMem L);
//sirve para saltarte el head node al acceder a la lista
tPosM primeroBlock(tHistMem L);
tPosM lastBlock (tHistMem L);
tPosM nextBlock (tPosM p, tHistMem L);
tPosM previousBlock (tPosM p, tHistMem L);
bool insertMemoryBlock (tItemM d , tHistMem *L);
tItemM getMemBlock (tPosM p, tHistMem L);
tPosM  findMemblock (int num, tHistMem L);
//el ítem esta al final de la lista
bool deleteLastBlock(tPosM p, tHistMem *L);
void deletePrimBlock(tHistMem *L);
void deleteHistMem (tHistMem *L);





#endif //P0_SO_MEMORIA_H
