#include "static_tokens.h"

void createEmptyTokensList(TokensList *L){
    L.lastPos = TNULL;
}

bool isEmptyTokensList (TokensList L){
    return (L.lastPos==TNULL);
}

tPosT firstToken(TokensList L){
    return 0;
}

tPosT lastToken(TokensList L){
    return L.lastPos;
}

tPosT previousToken(tPosT p, TokensList L){
    return --p;
}

tPosT nextToken(tPosT p, TokensList L){
     if (p==L.lastPos)
         return TNULL;
     else
         return ++p;
}

bool insertToken (tItemT d, TokensList *L){

    if(L->lastPos == MAX-1) //ya no hay más sitio en el array
        return false;
    else {
        L->lastPos++; //aumentar en 1 la última posición
        L->data[L->lastPos]=d; //el contenido de la lista[pos = Last->pos] = contenido a insertar
        return true;
    }

}

void deleteAtTokenPosition (tPosT p, TokensList *L){ //no se va a usar, así que no se ha mirado
    int i;
    L->lastPos--;
    for(i=p; i<= L->lastPos; i++){
        L->data[i]= L->data[i+1];
    }

}

void getToken(tPosT p, TokensList L, char *token[MAX]){
    strcpy(*token, L.data[p]);
   //return L.data[p];
}

void updateToken (tItemT d, tPosT p, TokensList *L){ //tampoco se va a usar
    L->data[p] = d;
}
/*
tPosT  findToken (int d, TokensList L) {
    tPosT p;
    if(L.lastPos == TNULL)
        return TNULL;
    else{
        for(p=0; (p<L.lastPos) && ( ); p++);
        if(strcmp(L.data[p].productId,d)==0)
            return p;
        else
            return TNULL;
    }
}
 */

void deleteTokenList (TokensList* L){
     L->lastPos = TNULL;
}




