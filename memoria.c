/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#include "memoria.h"


bool createNode(tPosM* p){
    *p=malloc(sizeof(**p)); //**p es a donde apunta p //a *p le estás reservando en memoria el espacio de un tipo int
    return *p != LNULL; //si se cumple hay memoria disponible
}

bool createMem(tHistMem *L){
    tPosM q;

    if (!createNode(&q)) return false; //no hay memoria
    else{
        q->next = LNULL;
        q->last = LNULL;
        *L = q;
        return true;
    }

}

bool isEmptyMem (tHistMem L){
    if(L->next == LNULL) return true; //L->next es el campo que apunta al siguiente del head node
    else return false;
}

tPosM firstBlock(tHistMem L){
    return L;
}

tPosM primeroBlock(tHistMem L){
    return L->next;
}

tPosM lastBlock(tHistMem L){
    return L->last;
}

tPosM nextBlock(tPosM p){ //tPosL next(tPosL p, tList L)
    return p->next;
}

tPosM previousBlock (tPosM p, tHistMem L){
    tPosM q;
    if(p==L) return LNULL;
    else{
        for(q=L; q->next!=p; q=q->next);
        return q;
    }
}


bool insertMemoryBlock(tItemM d, tHistMem *L) { //en este caso siempre se va a insertar por el final, es decir, después del último nodo

    tPosM q, r;

    if (!createNode(&q)) return false; //no hay espacio
    else{
        q->next = LNULL;
        q->data = d;

        if( (*L)->next == LNULL ){ //estamos insertando en el head node
            (*L)->next = q;
            (*L)->last = q;
        }else{ //la lista tiene más elementos
            for (r = *L; r->next != LNULL; r = r->next); //meter aquí el contador y asignar q->data = d después
            r->next = q;
            (*L)->last = r->next; //puntero al último nodo
        }
        return true;
    }

}

tItemM getMemBlock(tPosM p){
    return p->data;
}

tPosM findMemblock (int num, tHistMem L){
    tPosM p;
    p = LNULL;
    for (p=L; (p != LNULL) && (p->data.puesto != num); p=p->next);
    return p;
} //devuelve el puntero al nodo que tiene la instrucción de número num

void deleteHistMem (tHistMem *L){
    tPosM p;
    while(*L!=LNULL){
        p=*L;
        *L = (*L)->next;
        free(p);
    }
}

bool deleteLastBlock(tPosM p, tHistMem *L){ //siempre será al final
    p = (*L)->last;
    tPosM q;

    if(p->next==LNULL){ //está al final de la lista
        for(q=*L; q->next!=p; q=q->next);
        q->next=LNULL;
        free(p);
        return true;
    }else{
        printf("no se puede borrar porque no es el último");
        return false;
    }
}

void deletePrimBlock(tHistMem *L){
    tPosM q,p;
    p = (*L)->next;
    q = p->next;
    free(p);
    (*L)->next = q;
}