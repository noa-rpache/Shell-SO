#include "list.h"


bool createNode(tPosL* p){
    *p=malloc(sizeof(**p)); //**p es a donde apunta p //a *p le estás reservando en memoria el espacio de un tipo int
    return *p != LNULL; //si se cumple hay memoria disponible
}

bool createList(tList *L){
    tPosL q;

    if (!createNode(&q)) return false; //no hay memoria
    else{
        *L = q;
        return true;

    }

}

bool insertElement(tItemL d, tPosL p, tList *L) { //en este caso siempre se va a insertar por el final, es decir, después del último nodo

    tPosL q, r;

    if (!createNode(&q)) return false; //no hay espacio
    else{

        //asignamos valores al nodo
        q->data = d;
        q->next = LNULL;

        //vamos al último elemento de la lista
        for (r = *L; r->next != LNULL; r = r->next);
        r->next = q;

        return true;
    }

} //cambia la condicion de lista vacia

tItemL getItem(tPosL p, tList L){
    return p->data;
}

tPosL findItem (posicion num, tList L){
    tPosL p;
    p = LNULL;
    for (p=L; (p != LNULL) && (p->data.puesto != num); p=p->next);
    return p;
} //devuelve el puntero al nodo que tiene la instrucción de número num

void deleteList (tList *L){
    tPosL p;
    while(*L!=LNULL){
        p=*L;
        *L = (*L)->next;
        free(p);
    }
}