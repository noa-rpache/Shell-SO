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

//historial comandos
typedef struct{
    char comando[MAX_LENGHT_PATH];
    TokensList comandos;
    int tokens;
    int puesto; //el puesto del último indica el número total de elementos en la lista
}tItemL;


typedef void *tItem; //tItem es un puntero a un Item

typedef struct tNode *tPos;
struct  tNode {
    void *data;
    tPos next;
    tPos last; //solo tiene sentido en el head node
};

typedef tPos tList;

//historial memoria
typedef enum {
    malloc;
    shared;
    mapped;
}tmem;

typedef struct{
    int direccion;
    int tamano;
    //cuando se colocó -> mirar tipo
    tmem tipo;
    key_t clave;
    char[MAX_LENGHT_PATH] nombre_archivo;
    int file_descriptor;
}tItemM;



bool createList(tList *L);
bool isEmptyList (tList L);
tPosL first (tList L);
tPosL primero(tList L); //sirve para saltarte el head node al acceder a la lista
tPosL last (tList L);
tPosL next (tPosL p, tList L);
tPosL previous (tPosL p, tList L);
bool insertElement (tItemL d , tList *L);
tItemL getItem (tPosL p, tList L);
tPosL  findItem (int num, tList L);
//el ítem esta al final de la lista
bool deleteLast(tPosL p, tList *L);
void deletePrimero(tList *L);
void deleteList (tList *L);
