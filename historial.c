#include "historial.h"
#include <stdio.h>


bool createNode(tPosL* p){
    *p=malloc(sizeof(**p)); //**p es a donde apunta p //a *p le estás reservando en memoria el espacio de un tipo int
    return *p != LNULL; //si se cumple hay memoria disponible
}

bool createList(tList *L){
    tPosL q;

    if (!createNode(&q)) return false; //no hay memoria
    else{
        char *titulo = "\0";
        strcpy(q->data.comando, titulo);
        //createEmptyTokensList(&comandos); //no hace falta porque el head node no tiene tokens
        q->data.puesto = 0;
        q->data.tokens = 0;
        q->next = LNULL;
        *L = q;
        return true;

    }

}

bool isEmptyList (tList L){
    if(L->next == LNULL) return true; //L->next es el campo que apunta al siguiente del head node
    else return false;
}

tPosL first(tList L){
    return L;
}

tPosL primero(tList L){
    return L->next;
}

tPosL last(tList L){
    tPosL p;
    for(p=L; p->next!=LNULL; p=p->next);
    return p;
}

tPosL next(tPosL p, tList L){ //tPosL next(tPosL p, tList L)
    return p->next;
}

tPosL previous (tPosL p, tList L){
    tPosL q;
    if(p==L) return LNULL;
    else{
        for(q=L; q->next!=p; q=q->next);
        return q;
    }
}

bool insertElement(tItemL d, tList *L) { //en este caso siempre se va a insertar por el final, es decir, después del último nodo

    tPosL q, r;

    if (!createNode(&q)) return false; //no hay espacio
    else{
        //asignamos valores al nodo
        //q->data = d;
        int contador = 1;
        q->next = LNULL;

        if( d.puesto == 0){ //estamos insertando en el head node
            (*L)->next = q;
        }else{ //la lista tiene más elementos
            for (r = *L; r->next != LNULL; r = r->next) contador++; //meter aquí el contador y asignar q->data = d después
            d.puesto = contador;
            q->data = d;
            r->next = q;
        }

        return true;
    }

}

tItemL getItem(tPosL p, tList L){
    return p->data;
}

tPosL findItem (int num, tList L){
    tPosL p;
    p = LNULL;
    for (p=L; (p != LNULL) && (p->data.puesto != num); p=p->next);
    return p;
} //devuelve el puntero al nodo que tiene la instrucción de número num

void deleteList (tList *L){
    tPosL p;
    while(*L!=LNULL){
        deleteTokensList(&(*L)->data.comandos);
        p=*L;
        *L = (*L)->next;
        free(p);
    }
}