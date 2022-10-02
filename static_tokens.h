#ifndef STATIC_TOKENS_H
#define STATIC_TOKENS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define TNULL -1
#define MAX_TOKENS 15
#define MAX 30


typedef char tItemT[MAX]; //definición de los elementos que queremos insertar en data
typedef int tPosT;
typedef struct {
    char data[MAX_TOKENS][MAX];
    tPosT lastPos; //última posición con datos
} TokensList;

/*
 * es una lista de tokens, solo se añade por el final, el campo data es un array de strings
 * */

void createEmptyTokensList(TokensList *L);
bool isEmptyTokensList (TokensList L);
tPosT firstToken (TokensList L);
tPosT lastToken (TokensList L);
tPosT nextToken (tPosT p, TokensList L);
tPosT previousToken (tPosT p, TokensList L);
bool insertToken (tItemT d ,TokensList *L);
void getToken(tPosT p, TokensList L, char *token[MAX]);
void deleteTokensList (TokensList* L);

#endif
