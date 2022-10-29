/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#include "historial.h"


bool createNode(tPos* p){
    *p=malloc(sizeof(**p)); //**p es a donde apunta p //a *p le estás reservando en memoria el espacio de un tipo int
    return *p != LNULL; //si se cumple hay memoria disponible
}

bool createList(tList *L){
    tPos q;

    if (!createNode(&q)) return false; //no hay memoria
    else{
        q->puesto = 0;
        q->next = LNULL;
        q->last = LNULL;
        q->data = LNULL;
        *L = q;
        return true;
    }

}

bool isEmptyList (tList L){
    if(L->next == LNULL) return true; //L->next es el campo que apunta al siguiente del head node
    else return false;
}

tPos first(tList L){
    return L;
}

tPos primero(tList L){
    return L->next;
}

tPos last(tList L){
    return L->last;
}

tPos next(tPos p){ //tPos next(tPos p, tList L)
    return p->next;
}

tPos previous (tPos p, tList L){
    tPos q;
    if(p==L) return LNULL;
    else{
        for(q=L; q->next!=p; q=q->next);
        return q;
    }
}

bool insertElement(void *d, tList *L) { //en este caso siempre se va a insertar por el final, es decir, después del último nodo

    tPos q, r;

    if (!createNode(&q)) return false; //no hay espacio
    else{

        int contador = 1;
        q->data = d;
        q->next = LNULL;

        if( (*L)->next == NULL ){ //estamos insertando en el head node porque aún no se ha actualizado el último
            //printf("head\n");
            q->puesto = contador;
            (*L)->next = q;
        }else{ //la lista tiene más elementos
            //printf("skinheads banned from here\n");
            for (r = *L; r->next != LNULL; r = r->next) contador++;
            q->puesto = contador;
            //q->data = d;
            r->next = q;
        }
        (*L)->last = q; //puntero al último nodo
        //printf("insertamos con éxito!! yupi\n");
        return true;
    }

}

/*tItemL getItem(tPos p){
    return p->data;
}*/

void *getItem(tPos p){
    if (p == NULL) return NULL;
    return p->data;
}

int getPuesto(tPos p){
    return p->puesto;
}

tPos findItem (int num, tList L){
    tPos p;
    p = LNULL;
    for (p=L; (p != LNULL) && (p->puesto != num); p=p->next);
    return p;
} //devuelve el puntero al nodo que tiene la instrucción de número num

void deleteHist (tList *L){
    tPos p;
    while(*L!=LNULL){
        p=*L;
        //printf("posición eliminada: %d\n",p->puesto);
        if(p->data != LNULL){

            //printf("se borra data\n");

            NodoLista n = getItem(p);
            deleteTokensList(&n->comandos);
            //free(n); //para que no quede nada

            //free(p->data); //borrar el resto del contenido
        }
        //printf("se borra la posición\n");
        free(p);
        *L = (*L)->next;
    }
}

void deleteList (tList *L){
    tPos p;
    while(*L!=LNULL){
        p=*L;
        *L = (*L)->next;
        if(p->data != LNULL) free(p->data);
        free(p);
    }
}

bool deleteLast(tPos p, tList *L){ //siempre será al final
    tPos q;

    if(p->next==LNULL){ //está al final de la lista
        for(q=*L; q->next!=p; q=q->next);
        q->next=LNULL;
        free(p->data);
        free(p);
        return true;
    }else{
        printf("no se puede borrar porque no es el último");
        return false;
    }
}

void deletePrimero(tList *L){
    tPos q,p;
    p = (*L)->next;
    q = p->next;
    free(p->data);
    free(p);
    (*L)->next = q;
}
