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

#define PNULL NULL

typedef enum {
    finished, stopped, signaled, active
}status;

typedef struct {
    int pid;
    struct tm tiempo;
    status estado;
    //línea de comandos, podemos guardar el tposl de cuando se introduce
    int prioridad;
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

tPosP primeroProc(tHistProc L); //sirve para saltarte el head node al acceder a la lista

tPosP lastProc(tHistProc L);

tPosP nextProc(tPosP p, tHistProc L);

tPosP previousProc(tPosP p, tHistProc L);

bool insertProc(tItemP d, tHistProc *L);

tItemP getProc(tPosP p, tHistProc L);

//tPosP findProc(int num, tHistProc L);

//el ítem esta al final de la lista
bool deleteLastProc(tPosP p, tHistProc *L);

void deletePrimeroProc(tHistProc *L);

void deleteProcList(tHistProc *L);


#endif //P0_SO_PROCESOS_H
