#ifndef TOKENS_LIST_H
#define TOKENS_LIST_H

#include "list.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define TNULL NULL
#define MAX 30
//estoy pensando, mejor un array porque así ser puede acceder a cada elemento
typedef struct {
    char token[MAX]; //largo máximo de caracteres en un token
}tItemL;

typedef struct tNode *tPosL;
struct  tNode {
    tItemL data;
    tPosL next;
};

typedef tPosL TokensList;

void createEmptyTokensList(tList *L);
bool isEmptyTokensList (tList L);
tPosL firstToken (tList L);
tPosL lastToken (tList L);
tPosL nextToken (tPosL p, tList L);
tPosL previousToken (tPosL p, tList L);
bool insertTokenItem (tItemL d ,tList *L);
void deleteAtTokenPosition(tPosL p, tList *L);
tItemL getTokenItem (tPosL p, tList L);
void updateTokenItem (tItemL d, tPosL p, tList *L);
tPosL findTokenItem (tProductId d, tList L);
void deleteTokensList (tList *L)



#endif
