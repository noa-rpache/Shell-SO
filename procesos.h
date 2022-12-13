/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#ifndef P0_SO_PROCESOS_H
#define P0_SO_PROCESOS_H
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "historial.h"

#define PNULL NULL

typedef enum {
    finished, stopped, signaled, active
}estadio;

typedef struct {
    int pid; //esto es el PID del propio proceso
    int info; //int con información sobre la salida
    struct tm tiempo;
    estadio estado;
    tItemL comando;//línea de comandos, podemos guardar el tposl de cuando se introduce
        // -> se añadirá cuando se haya probado bien esta lista, para comprobar que no hay coincidencias con la otra implementación
} tItemP;

typedef struct tNodeP *tPosP;
struct tNodeP {
    tItemP data;
    tPosP next;
    tPosP last;
};

typedef tPosP tHistProc;

bool createProcList(tHistProc *L);

bool isEmptyProc(tHistProc L);

tPosP firstProc(tHistProc L);

tPosP primerProc(tHistProc L); //sirve para saltarte el head node al acceder a la lista

tPosP lastProc(tHistProc L);

tPosP nextProc(tPosP p);

tPosP previousProc(tPosP p, tHistProc L);

bool insertProc(tItemP d, tHistProc *L);

tItemP getProc(tPosP p);

//tPosP findProc(int PID, tHistProc L);

void deleteProc(tPosP p, tHistProc *L);

//el ítem esta al final de la lista
void deleteLastProc(tHistProc *L);

void deletePrimeroProc(tHistProc *L);

void deleteProcList(tHistProc *L);


#endif //P0_SO_PROCESOS_H
