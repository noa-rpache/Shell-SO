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
#include <time.h>
#include "static_tokens.h"
#define MNULL NULL

typedef enum {
    maloc, shared, mapped
}tmem;

typedef struct{
    void *direccion; //para todos
    size_t tamano; //para todos
    struct tm tiempo; //para todos
    tmem tipo; //para todos
    key_t clave; //shared
    char nombre_archivo[MAX_LENGHT_PATH]; //mapped
    int file_descriptor; //mapped
}tItemM;

typedef struct tNodeM *tPosM;
struct  tNodeM {
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
tPosM nextBlock (tPosM p);
tPosM previousBlock (tPosM p, tHistMem L);
bool insertMemoryBlock (tItemM d , tHistMem *L);
tItemM getMemBlock (tPosM p);
tPosM  findMemblock (int num, tHistMem L);
//el ítem esta al final de la lista
bool deleteLastBlock(tPosM p, tHistMem *L);
void deletePrimBlock(tHistMem *L);
void deleteHistMem (tHistMem *L);
void deleteMemBlock(tPosM p, tHistMem *L);
void printBLocks(tHistMem L, tmem tipo);
tPosM findBlockMalloc(tHistMem L, size_t tamano);
tPosM findBlockShared(tHistMem L, key_t c);
tPosM findBlockMapped(tHistMem L, char nombre[MAX_LENGHT_PATH]);





#endif //P0_SO_MEMORIA_H
