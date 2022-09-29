#include "static_tokens.h"

void createEmptyTokensList(tList *L){
    L.lastPos = LNULL;
}

bool isEmptyTokensList (tList L){
    return (L.lastPos==LNULL);
}

tPosL firstToken(tList L){
    return 0;
}

tPosL lastToken(tList L){
    return L.lastPos;
}

tPosL previousToken(tPosL p, tList L){
    return --p;
}

tPosL nextToken(tPosL p, tList L){
     if (p==L.lastPos)
         return LNULL;
     else
         return ++p;
}

bool insertToken (tItemL d, tList *L){

    if(L->lastPos == MAX-1) //ya no hay más sitio en el array
        return false;
    else {
        L->lastPos++; //aumentar en 1 la última posición
        L->data[L->lastPos]=d; //el contenido de la lista[pos = Last->pos] = contenido a insertar
        return true;
    }

}

void deleteAtTokenPosition (tPosL p, tList *L){
    int i;
    L->lastPos--;
    for(i=p; i<= L->lastPos;i++){
        L->data[i]= L->data[i+1];
    }

}

tItemL getToken(tPosL p, tList L){
   return L.data[p];
}

void updateToken (tItemL d, tPosL p, tList *L){
    L->data[p] = d;
}

tPosL  findToken (tProductId d, tList L) {
    tPosL p;
    if(L.lastPos == LNULL)
        return LNULL;
    else{
        for(p=0; (p<L.lastPos) && (strcmp(L.data [p].productId,d)!= 0); p++);
        if(strcmp(L.data[p].productId,d)==0)
            return p;
        else
            return LNULL;
    }
}

void deleteTokenList (tList* L){
     L->lastPos = LNULL;
}




