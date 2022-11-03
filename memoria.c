/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#include "memoria.h"


bool createNodeM(tPosM* p){
    *p=malloc(sizeof(**p)); //**p es a donde apunta p //a *p le estás reservando en memoria el espacio de un tipo int
    return *p != MNULL; //si se cumple hay memoria disponible
}

bool createMem(tHistMem *L){
    tPosM q;

    if (!createNodeM(&q)) return false; //no hay memoria
    else{
        q->next = MNULL;
        q->last = MNULL;
        *L = q;
        return true;
    }

}

bool isEmptyMem (tHistMem L){
    if(L->next == MNULL) return true; //L->next es el campo que apunta al siguiente del head node
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
    if(p==L) return MNULL;
    else{
        for(q=L; q->next!=p; q=q->next);
        return q;
    }
}


bool insertMemoryBlock(tItemM d, tHistMem *L) { //en este caso siempre se va a insertar por el final, es decir, después del último nodo

    tPosM q, r;

    if (!createNodeM(&q)) return false; //no hay espacio
    else{
        q->next = MNULL;
        q->data = d;

        if( (*L)->next == MNULL ){ //estamos insertando en el head node
            (*L)->next = q;
            (*L)->last = q;
        }else{ //la lista tiene más elementos
            for (r = *L; r->next != MNULL; r = r->next); //meter aquí el contador y asignar q->data = d después
            r->next = q;
            (*L)->last = r->next; //puntero al último nodo
        }
        return true;
    }

}

tItemM getMemBlock(tPosM p){
    return p->data;
}

void deleteMemBlock(tPosM p, tHistMem *L){

    if(p == (*L)->next) deletePrimBlock(L);
    else{
        if( p->next == MNULL) deleteLastBlock(p,L);
        else{
            tPosM q;

            q = p->next;
            p->data = q->data;
            p->next = q->next;
            //p = q;

            free(p);
        }
    }
}

void deleteHistMem (tHistMem *L){
    tPosM p;
    while(*L!=MNULL){
        p=*L;
        *L = (*L)->next;
        free(p);
    }
}

bool deleteLastBlock(tPosM p, tHistMem *L){ //siempre será al final
    p = (*L)->last;
    tPosM q;

    if(p->next==MNULL){ //está al final de la lista
        for(q=*L; q->next!=p; q=q->next);
        q->next=MNULL;
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

void printMalloc(tItemM bloque){ //operación privada de la implementación
    char tiempo[15];
    char format[] = "%b %d %H";

    if (strftime(tiempo, 100,format, &bloque.tiempo) == 0) {
        printf("error en strftime: el string no cabe en el tamaño proporcionado\n");
        return ;
    }

    printf("%p %10lu %s malloc\n",bloque.direccion, bloque.tamano, tiempo);
}

void printShared(tItemM bloque){
    char tiempo[15];
    char format[] = "%b %d %H";

    if (strftime(tiempo, 100,format, &bloque.tiempo) == 0) {
        printf("error en strftime: el string no cabe en el tamaño proporcionado\n");
        return ;
    }

    printf("%p %10lu %s shared (key %d)\n",bloque.direccion, bloque.tamano, tiempo, bloque.clave);
}

void printMmap(tItemM bloque){
    char tiempo[15];
    char format[] = "%b %d %H";

    if (strftime(tiempo, 100,format, &bloque.tiempo) == 0) {
        printf("error en strftime: el string no cabe en el tamaño proporcionado\n");
        return ;
    }

    printf("%p %10lu %s %s (descriptor %d)\n",bloque.direccion, bloque.tamano, tiempo, bloque.nombre_archivo, bloque.file_descriptor);
}

void printBLocks(tHistMem L, tmem tipo){
    char type[10];

    switch(tipo){
        case maloc:
            strcpy(type, "malloc"); printf("***Lista de bloques asignados %s para el proceso %d\n",type,getpid());

            for(tPosM p = primeroBlock(L); p!= MNULL ; p = p->next){
                if(p->data.tipo == tipo){
                    printMalloc(p->data);
                }
            }

            break;
        case shared:
            strcpy(type, "shared"); printf("***Lista de bloques asignados %s para el proceso %d\n",type,getpid());

            for(tPosM p = primeroBlock(L); p!= MNULL ; p = p->next){
                if(p->data.tipo == tipo){
                    printShared(p->data);
                }
            }

            break;
        case mapped:
            strcpy(type, "mapped"); printf("***Lista de bloques asignados %s para el proceso %d\n",type,getpid());

            for(tPosM p = primeroBlock(L); p!= MNULL ; p = p->next){
                if(p->data.tipo == tipo){
                    printMmap(p->data);
                }
            }

            break;
        default: printf("si llegas aquí has hecho algo muy mal\n");
    }

}

void ListBLocks(tHistMem L){
    printf("***Lista de bloques asignados para el proceso %d\n",getpid());

    for(tPosM p = primeroBlock(L); p!= MNULL ; p = p->next){
        if(p->data.tipo == maloc){
            printMalloc(p->data);
        }else if(p->data.tipo == shared){
            printShared(p->data);
        }else {
            printMmap(p->data);
        }
    }

}

tPosM findBlockMalloc(tHistMem L, size_t tamano){ //si hay varios con el mismo tamaño??
    for(tPosM p = primeroBlock(L); p != MNULL; p = p->next){
        if(p->data.tipo == maloc){
            if(p->data.tamano == tamano){
                return p;
            }
        }
    }
    return MNULL;
}

tPosM findBlockShared(tHistMem L, key_t c){
    for(tPosM p = primeroBlock(L); p != MNULL; p = p->next){
        if(p->data.tipo == shared){
            if(p->data.clave == c){
                return p;
            }
        }
    }
    return MNULL;
}

tPosM findBlockMapped(tHistMem L, char nombre[MAX_LENGHT_PATH]){
    for(tPosM p = primeroBlock(L); p != MNULL; p = p->next){
        if(p->data.tipo == mapped){
            if(strcmp(p->data.nombre_archivo,nombre) == 0){
                return p;
            }
        }
    }
    return MNULL;
}

tPosM findAddress(void *address,tHistMem L){
    for(tPosM p = primeroBlock(L); p != MNULL; p = p->next){
        if(address == p->data.direccion) return p;
    }
    return MNULL;
}