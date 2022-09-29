#ifndef STATIC_TOKENS_H
#define STATIC_TOKENS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define TNULL -1
#define MAX_TOKENS 15
#define MAX 30

typedef char[MAX] tItemL;
typedef int tPosL;
typedef struct {
    tItemL data [MAX_TOKENS];
    tPosL lastPos; //última posición con datos
} TokensList;

/*
 * es una lista de tokens, solo se añade por el final, el campo data es un array de strings
 * */

void createEmptyTokensList(tList *L);
bool isEmptyTokensList (tList L);
tPosL firstToken (tList L);
tPosL lastToken (tList L);
tPosL nextToken (tPosL p, tList L);
tPosL previousToken (tPosL p, tList L);
bool insertToken (tItemL d ,tList *L);
void deleteAtTokenPosition(tPosL p, tList *L);
tItemL getToken (tPosL p, tList L);
void updateToken (tItemL d, tPosL p, tList *L);
tPosL findToken (tProductId d, tList L)

#endif
