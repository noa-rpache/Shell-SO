/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#include "procesos.h"

bool createNodeP(tPosP *p) {
    *p = malloc(sizeof(**p)); //**p es a donde apunta p //a *p le estás reservando en memoria el espacio de un tipo int
    return *p != PNULL; //si se cumple hay memoria disponible
}


bool createProcList(tHistProc *L) {
    tPosP q;

    if (!createNodeP(&q)) return false; //no hay memoria
    else {
        q->next = PNULL;
        q->last = PNULL;
        *L = q;
        return true;
    }
}

bool isEmptyProc(tHistProc L) {
    return L == PNULL;
}

tPosP firstProc(tHistProc L) {
    return L;
}

tPosP primerProc(tHistProc L) { //sirve para saltarte el head node al acceder a la lista
    return L->next;
}

tPosP lastProc(tHistProc L) {
    return L->last;
}

tPosP nextProc(tPosP p) {
    return p->next;
}

tPosP previousProc(tPosP p, tHistProc L) {
    tPosP q;
    if (p == L) return PNULL;
    else {
        for (q = L; q->next != p; q = q->next);
        return q;
    }
}

bool insertProc(tItemP d, tHistProc *L) {
    tPosP q, r;

    if (!createNodeP(&q)) return false; //no hay espacio
    else {
        q->next = PNULL;
        q->data = d;

        if ((*L)->next == PNULL) { //estamos insertando en el head node
            (*L)->next = q;
            (*L)->last = q;
        } else { //la lista tiene más elementos
            for (r = *L; r->next != PNULL; r = r->next); //meter aquí el contador y asignar q->data = d después
            r->next = q;
            (*L)->last = r->next; //puntero al último nodo
        }
        return true;
    }
}

tItemP getProc(tPosP p) {
    return p->data;
}

//tPosP findProc(int num, tHistProc L);

void deleteProc(tPosP p, tHistProc *L) {
    if (p == (*L)->next) deletePrimeroProc(L);
    else {
        if (p->next == PNULL) deleteLastProc(L);
        else {
            tPosP q;

            q = p->next;
            p->data = q->data;
            p->next = q->next;
            //p = q;

            free(p);
        }
    }

}

//el ítem esta al final de la lista
void deleteLastProc(tHistProc *L) {
    tPosP q, p = (*L)->last;

    for (q = *L; q->next != p; q = q->next); //buscamos el penúltimo
    q->next = PNULL; //hacemos que el penúltimo no apunte al siguiente
    (*L)->last = q;
    free(p); //liberamos el último
}

void deletePrimeroProc(tHistProc *L) {
    tPosP q, p;
    p = (*L)->next;
    q = p->next;
    free(p);
    (*L)->next = q;
}

void deleteProcList(tHistProc *L) {
    tPosP p;
    while (*L != PNULL) {
        p = *L;
        *L = (*L)->next;
        free(p);
    }
}