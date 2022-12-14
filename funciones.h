/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#ifndef P0_SO_FUNCIONES_H
#define P0_SO_FUNCIONES_H

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h> //esto es para infosis
#include <time.h> //para fecha
#include <errno.h> //para errores
#include <sys/types.h>
#include <sys/stat.h> //para struct stat
#include <sys/resource.h>//para las funciones de prioridad
#include <grp.h> //utilidades sobre grupos
#include <pwd.h> //utilidades fichero password -> para gid y uid
#include <sys/dir.h> //utilidades sobre directorios
#include <fcntl.h>
#include <dirent.h> //para opendir()
#include <sys/shm.h> //para memoria compartida
#include <sys/mman.h> //para (des)mapear
#include <sys/wait.h> //para waitpid
#include <sys/time.h>
#include <sys/resource.h>
#include "memoria.h" //lista bloques de memoria
#include "procesos.h" //para los procesos, este tiene incluido el historial.h

#define TAMANO 2048

extern char **environ;

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

struct SEN {
    char *nombre;
    int valor;
};

//las siguientes funciones nos permiten obtener el nombre de una senal a partir del número y viceversa
static struct SEN sigstrnum[] = {
        {"HUP", SIGHUP},
        {"INT", SIGINT},
        {"QUIT", SIGQUIT},
        {"ILL", SIGILL},
        {"TRAP", SIGTRAP},
        {"ABRT", SIGABRT},
        {"IOT", SIGIOT},
        {"BUS", SIGBUS},
        {"FPE", SIGFPE},
        {"KILL", SIGKILL},
        {"USR1", SIGUSR1},
        {"SEGV", SIGSEGV},
        {"USR2", SIGUSR2},
        {"PIPE", SIGPIPE},
        {"ALRM", SIGALRM},
        {"TERM", SIGTERM},
        {"CHLD", SIGCHLD},
        {"CONT", SIGCONT},
        {"STOP", SIGSTOP},
        {"TSTP", SIGTSTP},
        {"TTIN", SIGTTIN},
        {"TTOU", SIGTTOU},
        {"URG", SIGURG},
        {"XCPU", SIGXCPU},
        {"XFSZ", SIGXFSZ},
        {"VTALRM", SIGVTALRM},
        {"PROF", SIGPROF},
        {"WINCH", SIGWINCH},
        {"IO", SIGIO},
        {"SYS", SIGSYS},
/*señales que no hay en todas partes*/
#ifdef SIGPOLL
        {"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
        {"PWR", SIGPWR},
#endif
#ifdef SIGEMT
        {"EMT", SIGEMT},
#endif
#ifdef SIGINFO
        {"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
        {"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
        {"CLD", SIGCLD},
#endif
#ifdef SIGLOST
        {"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
        {"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
        {"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
        {"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
        {"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
        {"WAITING", SIGWAITING},
#endif
        {NULL, -1},
};    /*fin array sigstrnum */

//utilidades
int TrocearCadena(char *cadena, char *trozos[]);

int int_convert(tItemT cadena);

void printComand(tItemL impresion, bool puesto, bool salto);

char LetraTF(mode_t m);

char *ConvierteModo2(mode_t m);

void getDir();

int convertPriority(tItemT prioridad);

char *currentUser();

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

int asignarCompartida(tItemL entrada, tItemM *datos, bool crear);

void *MapearFichero(char *fichero, int protection, tItemM *datos);

int asignarMap(tItemL entrada, tItemM *datos);

//sobre deallocate
void desasignarMalloc(size_t tamano, tHistMem *bloques);

void desasignarClave(key_t clave, tHistMem *bloques);

void desasignarMapped(tItemT nombre, tHistMem *bloques);

void desasignarDireccion(tItemL entrada, tHistMem *bloques);

//aux de mem
void dopmap();

int isNumber(char *string);

int minimo(int a, int b);

//sobre I-O
int modos_IO(tItemL entrada, modo_IO *opciones);

ssize_t LeerFichero(char *f, void *p, size_t cont);

ssize_t EscribirFichero(char *f, const void *p, size_t cont, int overwrite);

//sobre variables de entorno
int BuscarVariable(char *var, char *e[]); //se busca *var en el entorno *e y se devuelve su posición

int CambiarVariable(char *var, char *valor, char *e[]);

//para ejecutar procesos
int OurExecvpe(char *file, char *const argv[], char *const envp[]); //para ejecutar un proceso en 1er plano

char *Ejecutable(char *s);//entiendo que busca el ejecutable en el sistema

int execute(char *prog, char *argv[MAX_TOKENS], char *envp[MAX_TOKENS], int prioridad, bool plano2, bool env);

//sobre señales
int ValorSenal(char *sen);

char *NombreSenal(int sen);

void procInfo(tItemP p);

#endif //P0_SO_FUNCIONES_H
