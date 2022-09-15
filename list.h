#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define LNULL NULL
#define MAX_LENGHT 200

typedef int posicion;
typedef struct{
    posicion puesto;
    char comando[MAX_LENGHT];
}tItemL;
typedef struct tNode *tPosL;
struct  tNode {
    tItemL data;
    tPosL next;
};

typedef tPosL tList;

bool createList(tList *L);
bool isEmptyList (tList L);
bool insertElement (tItemL d , tPosL p, tList *L);
tItemL getItem (tPosL p, tList L);
tPosL  findItem (posicion num, tList L);
void deleteList (tList *L);