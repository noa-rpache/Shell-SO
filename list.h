#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define LNULL NULL
#define MAX_LENGHT 100 //este es el largo de una cadena con punteros a otras, debería ser tanto??

typedef struct{
    int puesto;
    const char *comando[MAX_LENGHT];
    int tokens;
}tItemL;
typedef struct tNode *tPosL;
struct  tNode {
    tItemL data;
    tPosL next;
};

typedef tPosL tList;

bool createList(tList *L);
//bool isEmptyList (tList L);
tPosL first (tList L);
tPosL primero(tList L); //sirve para saltarte el head node al acceder a la lista
tPosL last (tList L);
tPosL next (tPosL p, tList L);
tPosL previous (tPosL p, tList L);
bool insertElement (tItemL d , tList *L);
tItemL getItem (tPosL p, tList L);
tPosL  findItem (int num, tList L);
void deleteList (tList *L);