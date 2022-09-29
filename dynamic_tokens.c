#include "dynamic_tokens.h.h"

void createEmptyTokensList(tList *L){
    *L = LNULL;
}

bool isEmptyTokensList(tList L){
    return(L==LNULL);
}

tPosL firstToken(tList L){
    return L;
}

tPosL lastToken(tList L){
    tPosL p;
    for(p=L; p->next!=LNULL; p=p->next);
    return p;
}

tPosL nextToken(tPosL p, tList L){
    return p->next;
}

tPosL previousToken (tPosL p, tList L){
    tPosL q;
    if(p==L) return LNULL;
    else{
        for(q=L; q->next!=p; q=q->next);
        return q;
    }
}

bool createNode(tPosL *p){
    *p=malloc(sizeof(**p));
    return *p != LNULL; //si p==LNULL false, que es cuando no hay espacio
}

tPosL findTokenPosition (tList L, tItemL d) {
    tPosL p = L; //si está vacía L == LNULL
    while ( ( p->next !=LNULL ) && ( strcmp(p->next->data.productId, d.productId) < 0 ) ){ //( p->next !=LNULL ) && ( strcmp(p->next->data.productId, d.productId) < 0 )
        p = p->next;
    }
    return p;
}

bool insertTokenItem(tItemL d, tList *L) {
    tPosL p, q;// apunta al nodo insertado, para insertar, para recorrer la lista

    if ( !createNode(&q) ) return false; //si no hay espacio en la lista
    else {
        q->data = d; //q ahora tiene el contenido de d
        q->next = LNULL; //inicializas next

        if (*L == LNULL){ //si la lista está vacía se enlaza al principio
            *L = q;
        }else{

            if (strcmp(d.productId, (*L)->data.productId) < 0) { //si es menor que el primero se enlaza antes de él
                q->next = *L;
                *L = q;

            }else{ //la posición deseada está por el medio

                p = findPosition(*L,d); //esto no puede dar LNULL porque no puedes llegar hasta aquí con la lista vacía

                q-> next = p ->next;
                p->next = q;
            }
        }
        return true;
    }
}



void deleteAtTokenPosition(tPosL p, tList *L){

    tPosL q;

    if(p==*L){ //si es el primero
        *L = (*L)->next;
    }else{

        if(p->next==LNULL){
            for(q=*L; q->next!=p; q=q->next);
            q->next=LNULL;
        }else{
            q = p->next;
            p->data = q->data;
            p->next = q->next;
            p = q;
        }
        free(p);

    }

}


tItemL getTokenItem(tPosL p, tList L){
    return p->data;
}


void updateTokenItem(tItemL d, tPosL p, tList *L) {
    p->data = d;
}


tPosL findTokenItem (tProductId d, tList L){ //esta es la que no funciona
    tPosL p = LNULL;

    for ( p=L; (p != LNULL) && (strcmp(p->data.productId, d) < 0); p=p->next ); //p no llega al final && el dato es "más pequeño" que el buscado

    if( (p!=LNULL) && (strcmp(d, p->data.productId)==0) ) { //no se terminó la lista y coincide el contenido
        return p; //devolver la posición en cuestión
    }else{
        return LNULL; //pues LNULL porque no está
    }
}

void deleteTokensList (tList *L){
    tPosL p;
    while(*L!=LNULL){
        p=*L;
        *L = (*L)->next;
        free(p);
    }
}