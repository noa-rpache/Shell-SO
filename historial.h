/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "static_tokens.h"
#define LNULL NULL

//typedef void *tItem; //tItem es un puntero genérico, pero para no liarse

typedef struct tNode *tPos;

struct  tNode {
    int puesto;
    void *data;
    tPos next;
    tPos last;
};

typedef tPos tList;

//historial comandos

typedef struct {
    char comando[MAX_LENGHT_PATH];
    TokensList comandos;
    int tokens;
}tItemL;

typedef tItemL *NodoLista;

//historial memoria
typedef enum { maloc, shared, mapped }tmem;

typedef struct{
    int direccion;
    int tamano;
    //cuando se colocó -> mirar tipo
    tmem tipo;
    key_t clave;
    char nombre_archivo[MAX_LENGHT_PATH];
    int file_descriptor;
}tItemM;

//lista para recursividad
typedef struct{
    char path[MAX_LENGHT_PATH];
}tItemR;


bool createList(tList *L);
bool isEmptyList (tList L);

tPos first (tList L);
tPos primero(tList L); //sirve para saltarte el head node al acceder a la lista
tPos last (tList L);
tPos next (tPos p);
tPos previous (tPos p, tList L);

bool insertElement (void *d , tList *L);
void *getItem(tPos p);
int getPuesto(tPos p);
tPos  findItem (int num, tList L);

//el ítem está al final de la lista
bool deleteLast(tPos p, tList *L);
void deletePrimero(tList *L);
void deleteHist (tList *L);
void deleteList (tList *L);
