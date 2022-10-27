/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#ifndef STATIC_TOKENS_H
#define STATIC_TOKENS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define TNULL -1
#define MAX_TOKENS 15 //máximo nº de tokens
#define MAX_LENGHT_PATH 100 //para cuando se quieran arrays de nombres de directorios

typedef char tItemT[MAX_LENGHT_PATH]; //definición de los elementos que queremos insertar en data
typedef int tPosT;
typedef struct {
    char data[MAX_TOKENS][MAX_LENGHT_PATH];
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
void getToken(tPosT p, TokensList L, tItemT token);
void deleteTokensList (TokensList* L);

#endif
