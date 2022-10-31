/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#ifndef P0_SO_FUNCIONES_H
#define P0_SO_FUNCIONES_H

//#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //esta venía en el man C de google
#include <sys/utsname.h> //esto es para infosis
#include <time.h> //para fecha
#include <errno.h> //para errores
#include <sys/types.h>
#include <sys/stat.h> //para struct stat
#include <grp.h> //utilidades sobre grupos
#include <pwd.h> //utilidades fichero password -> para gid y uid
//#include <sys/dir.h> //utilidades sobre directorios
#include <fcntl.h>
#include <dirent.h> //para opendir()
#include <sys/shm.h> //para memoria compartida
#include "historial.h"
#include "memoria.h"
#define TAMANO 2048

//tipos de datos a parte
typedef struct {
    bool largo;
    bool link;
    bool acc;
    bool hid;
    bool reca;
    bool recb;
    bool listar; //el struct lo pasa list y no stat
}modo;


//generales
int TrocearCadena(char *cadena, char *trozos[]);
int int_convert(tItemT cadena);
void printComand(tItemL impresion);
char LetraTF (mode_t m);
void getDir();
int isDirectory(const char *path);
int isDirEmpty(char *dirname);   //ver si un directorio esta o no vacio
char * ConvierteModo2 (mode_t m);
int printInfo(char ruta[MAX_LENGHT_PATH], char enlazada[MAX_LENGHT_PATH], const modo *opciones);
int ListContent(char path[MAX_LENGHT_PATH], const modo *opciones);
int ListReca(char path[MAX_LENGHT_PATH], const modo *opciones);
int ListRecb(char path[MAX_LENGHT_PATH], const modo *opciones);
int opciones(tItemL entrada,modo *opciones);
int borrar_dir(char *dir);//funcion recursiva para borrar directorios
void ListarBloques(tHistMem bloques);
void Recursiva (int n);
int asignarMalloc(tItemL entrada,tItemM *datos);
int asignarCompartida(tItemL entrada,tItemM *datos);
int desasignarMalloc(tItemL entrada,tItemM *datos, tHistMem *bloques);




#endif //P0_SO_FUNCIONES_H
