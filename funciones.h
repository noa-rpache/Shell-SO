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
#include <sys/mman.h> //para (des)mapear
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
} modo;

typedef struct {
    bool write;
    bool read;
    bool overwrite;
} modo_IO;


//utilidades
int TrocearCadena(char *cadena, char *trozos[]);

int int_convert(tItemT cadena);

void printComand(tItemL impresion);

char LetraTF(mode_t m);

char *ConvierteModo2(mode_t m);

void getDir();

//sobre directorios
int isDirectory(const char *path);

int isDirEmpty(char *dirname);   //ver si un directorio esta o no vacio

int borrar_dir(char *dir);//funcion recursiva para borrar directorios

//sobre stat y list
int printInfo(char ruta[MAX_LENGHT_PATH], char enlazada[MAX_LENGHT_PATH], const modo *opciones);

int ListContent(char path[MAX_LENGHT_PATH], const modo *opciones);

int ListReca(char path[MAX_LENGHT_PATH], const modo *opciones);

int ListRecb(char path[MAX_LENGHT_PATH], const modo *opciones);

int opciones(tItemL entrada, modo *opciones);

void ListarBloques(tHistMem bloques, int modo);

//sobre recursiva
void Recursiva(int n);

//sobre allocate
int asignarMalloc(tItemL entrada, tItemM *datos);

void *ObtenerMemoriaShmget(key_t clave, size_t tam);

int asignarCompartida(tItemL entrada, tItemM *datos);

void *MapearFichero(char *fichero, int protection, tItemM *datos);

int asignarMap(tItemL entrada, tItemM *datos);

//sobre deallocate
void desasignarMalloc(size_t tamano, tHistMem *bloques);

void desasignarCompartida(key_t clave, tHistMem *bloques);

void desasignarClave(key_t clave);

void desasignarMapped(tItemT nombre, tHistMem *bloques);

void desasignarDireccion(tItemL entrada, const tHistMem *bloques);

//sobre I-O
int modos_IO(tItemL entrada, modo_IO *opciones);

//provisionales
struct tm *ActualTime(); //no estoy nada segura de esto



#endif //P0_SO_FUNCIONES_H
