/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#include "static_tokens.h"

void createEmptyTokensList(TokensList *L) {
    L->lastPos = TNULL;
}

bool isEmptyTokensList(TokensList L) {
    return (L.lastPos == TNULL);
}

tPosT firstToken(TokensList L) {
    return 0;
}

tPosT lastToken(TokensList L) {
    return L.lastPos;
}

tPosT prevToken(tPosT p) {
    return --p;
}

tPosT nextToken(tPosT p, TokensList L) {
    if (p == L.lastPos)
        return TNULL;
    else
        return ++p;
}

bool insertToken(tItemT d, TokensList *L) {

    if (L->lastPos == MAX_TOKENS - 1) //ya no hay más sitio en el array
        return false;
    else {
        L->lastPos++; //aumentar en 1 la última posición
        strcpy(L->data[L->lastPos], d);
        //L->data[L->lastPos] = d; //el contenido de la lista[pos = Last->pos] = contenido a insertar
        return true;
    }

}

void getToken(tPosT p, TokensList L, tItemT token) {
    strcpy(token, L.data[p]);
}


void deleteTokensList(TokensList *L) {
    L->lastPos = TNULL;
}




