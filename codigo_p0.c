/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

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
#include "historial.h"
//#define MAX_INPUT 100 -> se define en la lista, pero mejor no usar ese -> está pendiente de cambiar
#define MAX_LENGHT_PATH 100 //para cuando se quieran arrays de nombres de directorios

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
bool procesarEntrada(tList *historial);
bool salir(char cadena[]);
int TrocearCadena(char *cadena, char *trozos[]);
void leerEntrada( char *entrada[], tList *historial);
void new_historial(char *comando[], tList *hist, int ntokens);
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

//comandos
void ayuda(tItemL comando);
void infosis();
void repetir_comando(tItemL entrada, tList *hist);
void autores( tItemL comando );
void pillar_pid( tItemL comando);
void carpeta( tItemL comando);
void fecha( tItemL comando);
void hist (tItemL comando, tList *hist);
void status(tItemL comando);
void listar(tItemL comando);
void create (tItemL comando);
int delete(tItemL comando) ;//borra documentos o directorios vacios
int deleteTree(tItemL comando);//borra recursivamente documentos y directorios no vacios


int main(int argc, char *arvg[]){

    tList historial;
    createList(&historial);
    bool salida = false;

    while( salida == false ){
        leerEntrada(arvg,&historial);
        salida = procesarEntrada(&historial);
    }

    deleteList(&historial);
    return 0;
}


//funciones generales
bool procesarEntrada(tList *historial){

    if( !isEmptyList(*historial)) {
        tItemL peticion = getItem(last(*historial), *historial);

        if (salir(peticion.comando)) return true;
        else{
            if (peticion.tokens == 0) deleteLast(last(*historial), historial); //peticion.tokens == 0
            else if (strcmp(peticion.comando, "autores") == 0) autores(peticion);
            else if (strcmp(peticion.comando, "comando") == 0) repetir_comando(peticion, historial);
            else if (strcmp(peticion.comando, "pid") == 0) pillar_pid(peticion);
            else if (strcmp(peticion.comando, "carpeta") == 0) carpeta(peticion);
            else if (strcmp(peticion.comando, "fecha") == 0) fecha(peticion);
            else if (strcmp(peticion.comando, "infosis") == 0) infosis();
            else if (strcmp(peticion.comando, "ayuda") == 0) ayuda(peticion);
            else if (strcmp(peticion.comando, "hist") == 0) hist(peticion, historial);
            else if (strcmp(peticion.comando, "crear") == 0) create(peticion);
            else if (strcmp(peticion.comando, "stat") == 0) status(peticion);
            else if (strcmp(peticion.comando, "list") == 0) listar(peticion);
            else if (strcmp(peticion.comando, "delete") == 0)delete(peticion);
            else if (strcmp(peticion.comando, "deltree") == 0)deleteTree(peticion);
            else printf("%s: no es un comando del shell\n", peticion.comando);
            return false;
        }

    }
    return false;
}

void leerEntrada(char *entrada[], tList *historial){
    char *orden_procesada[MAX_LENGHT];

    printf(">> ");
    fgets(*entrada, MAX_LENGHT, stdin);
    if( strcmp("\n",*entrada) != 0 ) {
        int ntokens = TrocearCadena(*entrada, orden_procesada);
        new_historial(&orden_procesada[0],historial,ntokens);
    }else{
        char *n = "vacia";
        new_historial(&n,historial,0);
    }

}

void new_historial(char *comando[], tList *hist, int ntokens){
    tItemL nuevo;
    nuevo.tokens = ntokens;
    strcpy(nuevo.comando,*comando);
    createEmptyTokensList(&nuevo.comandos);

    for(int i = 1; i < ntokens; i++) //empieza en 1 porque ya se copió el valor en 0
        if( !insertToken(comando[i],&nuevo.comandos) ) printf("no se ha insertado el token %d\n", i);

    if ( !insertElement(nuevo, hist) ) printf("no se ha insertado el elemento\n");

}

bool salir(char cadena[]){

    if (strcmp(cadena, "fin") == 0) return true;
    else if (strcmp(cadena, "salir") == 0) return true;
    else if( strcmp(cadena, "bye") == 0 ) return true;
    else return false;

}

//generales, auxiliares
int TrocearCadena(char *cadena, char *trozos[]){
    int i=1;

    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    else{
        while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
            i++;
        return i;
    }

}

int int_convert(tItemT cadena){
    int convertido = atoi(cadena);
    return convertido*(-1);
}

void printComand(tItemL impresion){
    printf("%d: %s", impresion.puesto, impresion.comando);
    if(impresion.tokens > 1) { //si hay algún token más que el comando ppal
        for (int i = 0; i < impresion.tokens - 1; i++) { //ntokens -1 == nº de especificadores
            printf(" %s", impresion.comandos.data[i]);
        }
    }
    printf("\n");
}

char LetraTF (mode_t m){//devuelve el tipo de un fichero
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no debería aparecer*/
    }
}

void getDir(){
    char directorio[MAX_LENGHT_PATH];
    getcwd(directorio,sizeof(directorio) );
    if( errno == -1 ){ //se ha dado un error
        perror("no se ha podido conseguir la ruta actual");
        strerror(errno);
    }else{
        printf("%s\n", directorio );
    }
}

char * ConvierteModo2 (mode_t m)
{
    static char permisos[12];
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

int opciones(tItemL entrada,modo *opciones) {
    int controlador = 0;

    for (int i = 0; i <= entrada.tokens - 2; i++) { //tokens es el total de tokens, incluido el ppal
        tItemT aux;
        getToken(i, entrada.comandos, aux);

        if (strcmp("-long", aux) == 0) {
            (*opciones).largo = true; //se ha detectado long
            controlador++;
        } else if (strcmp("-link", aux) == 0) {
            (*opciones).link = true;
            controlador++;
        } else if (strcmp("-acc", aux) == 0) {
            (*opciones).acc = true;
            controlador++;
        } else if (strcmp("-hid", aux) == 0) {
            (*opciones).hid = true;
            controlador++;
        } else if ((*opciones).listar) {
            if (strcmp("-reca", aux) == 0) {
                (*opciones).reca = true;
                (*opciones).recb = false;
                controlador++;
            } else if (strcmp("-recb", aux) == 0) {
                (*opciones).recb = true;
                (*opciones).reca = false;
                controlador++;
            }
        } else i = entrada.tokens - 2; //no se ha detectado ninguno -> actualizar i para terminar el bucle

    }
    return controlador;
}

int printInfo(char rutaReal[MAX_LENGHT_PATH], char enlazada[MAX_LENGHT_PATH], const modo *opciones) { //la ruta enlazada se pasa siemrpe, la rutaReal puede ser NULL en un directorio
    struct stat contenido;

    if (lstat(enlazada, &contenido) == -1) {
        perror("error al acceder al los datos del archivo/directorio ");
        perror(enlazada);
        return -1;
    }

    struct tm *time;
    if (opciones->acc || opciones->largo) {
        if (opciones->acc) {
            time = localtime(&contenido.st_atim.tv_sec);
        } else time = localtime(&contenido.st_mtim.tv_sec);

        if (time == NULL) {
            perror("error en localtime_r");
            return -1;
        }

        char tiempo[15];
        char format[] = "%Y-%m-%d %H:%M";
        if (strftime(tiempo, 100,format, time) == 0) {
            printf("el string no cabe en el tamaño proporcionado\n");
            return 0;
        }

        printf("%s ",tiempo);
    }

    if (opciones->largo) {
        struct group *grupinho = getgrgid(contenido.st_gid);
        if ( grupinho == NULL) {
            perror("error al acceder al nombre del grupo");
            return -1;
        }

        struct passwd *user = getpwuid(contenido.st_uid);
        if(user == NULL){
            perror("error al acceder al nombre del propietario");
            return -1;
        }

        printf("%5ld", (long) contenido.st_nlink);
        printf(" (%10ld)", (long) contenido.st_ino);
        printf("%7s", grupinho->gr_name);
        printf("%7s",user->pw_name);
        printf("%3c", LetraTF(contenido.st_mode));
        printf("%s", ConvierteModo2(contenido.st_mode));

    }

    printf("%10ld ", (long) contenido.st_size);

    if(opciones->listar == true){
        printf("%s", rutaReal);

        if (opciones->largo && (opciones->link && LetraTF(contenido.st_mode) == 'l')) { //imprimir la ruta real a la que apunta el enlace
            char origen[MAX_LENGHT_PATH];
            realpath(enlazada,origen);
            printf(" -> %s", origen);
        }

    }else {
        printf("%s", enlazada); //se imprime siempre

        if (opciones->largo && (opciones->link && LetraTF(contenido.st_mode) == 'l')) { //imprimir la ruta real a la que apunta
            printf(" -> %s", rutaReal); //con -link siempre se imprime LA RUTA real
        }

    }
    printf("\n");

    return 0;
}

int ListContent(char path[MAX_LENGHT_PATH], const modo *opciones){ //pasar el path requerido con las opciones pedidas y mostrarlo
    printf("************%s\n",path);

    DIR *directory_stream = opendir(path);
    if(directory_stream == NULL){
        perror("error en opendir");
        return -1;
    }

    struct dirent *directorio;
    errno = 0;

    while((directorio = readdir(directory_stream)) != NULL) {
        errno = 0;
        int tam = 2*MAX_LENGHT_PATH;
        char rutaDir[tam];
        sprintf(rutaDir,"%s/%s",path,(*directorio).d_name);

        if (opciones->hid) {
            printInfo((*directorio).d_name,rutaDir,opciones); //enseñar todos los ficheros
        }else{
            if ((*directorio).d_name[0] != '.') {
                if (printInfo((*directorio).d_name,rutaDir, opciones) == -1) return -1;
            }

        }

        long sig = telldir(directory_stream);
        if(sig == -1){
            perror("error en telldir");
            return -1;
        }
        seekdir(directory_stream,sig);
    }

    if(directorio == NULL && errno != 0){
        printf("errno: %d\n",errno);
        perror("error en readdir list");
        return -1;
    }

    if( closedir(directory_stream) == -1){
        perror("error al cerrar el directorio");
        return -1;
    }

    return 0;
}

int ListReca(char path[MAX_LENGHT_PATH], const modo *opciones){ //pasar el path requerido con las opciones pedidas y mostrarlo
    errno = 0;
    printf("************%s\n",path);

    tList DirRecord;
    if(!createList(&DirRecord)){
        perror("no hay espacio para la recursividad");
        return -1;
    }


    DIR *directory_stream = opendir(path);
    if (directory_stream == NULL) return -1;

    struct dirent *directorio;

    while( (directorio = readdir(directory_stream)) != NULL) { //incluir detectar errores y no mostrar hidd
        int tam = 2 * MAX_LENGHT_PATH;
        char rutaDir[tam];
        sprintf(rutaDir, "%s/%s", path, (*directorio).d_name);

        if (opciones->hid) {
            if (printInfo((*directorio).d_name, rutaDir, opciones) == -1) strerror(errno); //enseñar todos los ficheros
            if (isDirectory(rutaDir) == 1) { //falta obviar a  . y ..
                if (strcmp((*directorio).d_name, ".") != 0 && strcmp((*directorio).d_name, "..") != 0) {
                    tItemL nuevo;
                    strcpy(nuevo.comando, rutaDir);
                    if (!insertElement(nuevo, &DirRecord)) {
                        perror("no hay sitio para la recursividad");
                        return -1;
                    }
                }
            }
        } else {
            if ((*directorio).d_name[0] != '.') {
                if (printInfo((*directorio).d_name, rutaDir, opciones) == -1)
                    strerror(errno); //enseñar los que no empiecen por '.'
                if (isDirectory(rutaDir) == 1) {
                    tItemL nuevo;
                    strcpy(nuevo.comando, rutaDir);
                    nuevo.puesto = 1;
                    if(!insertElement(nuevo, &DirRecord)) {
                        perror("no hay sitio para la recursividad");
                        return -1;
                    }
                }
            }
        }

        long sig = telldir(directory_stream);
        if (sig == -1) return -1;

        seekdir(directory_stream, sig);
    }

    if (directorio == NULL && errno != 0) return -1;

    if (closedir(directory_stream) == -1) return -1;

    //llamada recursiva para los siguientes directorios
    if (!isEmptyList(DirRecord)) {
        int total_records = getItem(last(DirRecord), DirRecord).puesto;
        for (int j = 0; j <= total_records - 1; j++) {
            tItemL nextdir = getItem(primero(DirRecord), DirRecord);
            if (ListReca(nextdir.comando, opciones) == -1) {
                perror("error en la recursividad ");
                strerror(errno);
                //return -1;
            }
            deletePrimero(&DirRecord);
        }
    }
    deleteList(&DirRecord); //al ser con head node tienes que hacerlo aunque no esté "vacía"
    return 0;
}

int ListRecb(char path[MAX_LENGHT_PATH], const modo *opciones){ //pasar el path requerido con las opciones pedidas y mostrarlo
    errno = 0;

    DIR *directory_stream = opendir(path);
    if(directory_stream == NULL) return -1;

    struct dirent *directorio;

    while((directorio = readdir(directory_stream)) != NULL ) {
        int tam = 2 * MAX_LENGHT_PATH;
        char rutaDir[tam];
        sprintf(rutaDir, "%s/%s", path, (*directorio).d_name);

        if (opciones->hid) {
            if (isDirectory(rutaDir) == 1) {
                if (strcmp((*directorio).d_name, ".") != 0 && strcmp((*directorio).d_name, "..") != 0)
                    ListRecb(rutaDir, opciones); //solo llama a la recursividad si no es ni . ni .., que si no nos hacemos un lío
            }
        } else {
            if ((*directorio).d_name[0] != '.') {
                if (isDirectory(rutaDir) == 1) {
                    ListRecb(rutaDir, opciones);
                }
            }
        }

        long sig = telldir(directory_stream);
        if (sig == -1) return -1;

        seekdir(directory_stream, sig);
    }

    if (directorio == NULL && errno != 0) {
        perror("error en readdir ");
        return -1;
    }

    if (ListContent(path, opciones) == -1) {
        return -1;
    }


    if(closedir(directory_stream) == -1) return -1;

    return 0;
}

int isDirectory(const char *path) {

    struct stat x;
    stat(path, &x);
    int aux = S_ISDIR(x.st_mode);
    return aux;

}

int borrar_dir(char *dir) {//funcion recursiva para borrar directorios

    DIR *dirp;
    struct dirent *tlist;
    char aux[MAX];

    if((dirp= opendir(dir))==NULL){
        return -1;}

    while ((tlist = readdir(dirp))!=NULL){
        strcpy(aux,dir);
        strcat(strcat(aux,"/"),tlist->d_name);

        //comprobamos que sea un directorio valido
        if(strcmp(tlist->d_name,"..")==0 || strcmp(tlist->d_name,".")==0)
            //no vuelve a comenzar la recursividad si es el direcotrio actual o el anterior
            continue;


        if(isDirectory(aux)){//aqui se comprueba que sea un directorio

            borrar_dir(aux);//aqui repetimos recursivamente la funcion
        }

        if(remove(aux)){//aqui se borra
            return -1;
        }}
    closedir(dirp);

    return 0;
}

int isDirEmpty(char *dirname) {   //ver si un directorio esta o no vacio
    int n = 0;
    struct dirent *p;
    DIR *dir = opendir(dirname);

    if (dir == NULL)
        return 1;

    while ( (p = readdir(dir)) != NULL ) {
        if(++n > 2)break;
    }

    if(p == NULL && errno !=0){
        perror("error en readdir DirEmpty");
        return -1;
    }

    closedir(dir);

    if (n <= 2) //directorio vacio
        return 1;
    else
        return 0;
}


//comandos
void ayuda( tItemL comando){ //como manda el mismo mensaje dando igual los especificadores del comando solo hace falta el comando

    if(comando.tokens == 1){
        printf("'ayuda cmd' donde cmd es uno de los siguientes comandos:\n");
        printf("fin salir bye fecha pid autores hist comando carpeta infosis ayuda crear delete deltree stat list\n");
    }else if( strcmp(comando.comando, "fin") == 0) printf("fin\tTermina la ejecucion del shell\n");
    else if( strcmp(comando.comando, "salir") == 0) printf("salir\tTermina la ejecucion del shell\n");
    else if( strcmp(comando.comando, "bye") == 0) printf("bye\tTermina la ejecucion del shell\n");
    else if( strcmp(comando.comando, "autores") == 0) printf("autores [-n|-l]\tMuestra los nombres y logins de los autores\n");
    else if( strcmp(comando.comando, "pid") == 0) printf("pid [-p]\tMuestra el pid del shell o de su proceso padre\n");
    else if( strcmp(comando.comando, "carpeta") == 0) printf("carpeta [dir]\tCambia (o muestra) el directorio actual del shell\n");
    else if( strcmp(comando.comando, "fecha") == 0) printf("fecha [-d|-h]\tMuestra la fecha y o la hora actual\n");
    else if( strcmp(comando.comando, "hist") == 0) printf("hist [-c|-N]\tMuestra el historico de comandos, con -c lo borra\n");
    else if( strcmp(comando.comando, "comando") == 0) printf("comando [-N]\tRepite el comando N (del historico)\n");
    else if( strcmp(comando.comando, "infosis") == 0 ) printf("infosis \tMuestra informacion de la maquina donde corre el shell\n");
    else if( strcmp(comando.comando, "crear") == 0 ) printf("crear [-f] [name]\tCrea un directorio o un fichero (-f)\n");
    else if( strcmp(comando.comando, "delete") == 0 ) printf("delete [name1 name2 ..]\tBorra ficheros o directorios vacios\n");
    else if( strcmp(comando.comando, "deltree") == 0 ) printf("deltree [name1 name2 ..]\tBorra ficheros o directorios no vacios recursivamente\n");
    else if( strcmp(comando.comando, "stat") == 0 ){
        printf("stat [-long][-link][-acc] name1 name2 ..\tlista ficheros;\n");
        printf("\t\t-long: listado largo\n");
        printf("\t\t-acc: acesstime\n");
        printf("\t\t-link: si es enlace simbolico, el path contenido\n");
    }else if( strcmp(comando.comando, "list") == 0 ){
        printf("list [-reca] [-recb] [-hid][-long][-link][-acc] n1 n2 ..\tlista contenidos de directorios\n");
        printf("\t\t-hid: incluye los ficheros ocultos\n");
        printf("\t\t-reca: recursivo (antes)\n");
        printf("\t\t-recb: recursivo (despues)\n");
        printf("\t\tresto parametros como stat\n");

    }else printf("%s no encontrado\n", comando.comando);
}

void infosis(){
    struct utsname informacion;
    uname(&informacion);
    if( errno == -1 ) {
        perror("no se ha conseguido la información del sistema");
        strerror(errno);
    }

    printf("%s (%s), OS: %s %s %s\n", informacion.nodename, informacion.machine, informacion.sysname,informacion.release, informacion.version);
}

void repetir_comando(tItemL entrada, tList *hist){ //comando N

    tItemT cpos; getToken(0,entrada.comandos, cpos);
    int posicion = atoi(cpos);
    tPosL posrepeticion = findItem(posicion, *hist);
    tItemL repeticion = getItem(posrepeticion,*hist);

    tItemL nuevo;
    strcpy(nuevo.comando,repeticion.comando);
    nuevo.comandos = repeticion.comandos;
    nuevo.tokens = repeticion.tokens;

    if ( !insertElement(nuevo, hist) ) printf("no se ha insertado el elemento\n");
    procesarEntrada(hist);

}

void autores( tItemL comando ){
    char nombre_noa[] = "Noa Rodriguez Pache", login_noa[] = "noa.rpache";
    char nombre_fatima[] = "Fátima Ansemil Barros", login_fatima[] = "fatima.ansemil";

    //printf("%s\n", modo);
    if ( comando.tokens == 1 ){ //no se especifica nada
        printf("%s: %s\n", nombre_noa, login_noa);
        printf("%s: %s\n", nombre_fatima, login_fatima);

    }else{
        tItemT modo; getToken(0,comando.comandos, modo);
        if (strcmp("-l", modo) == 0) {
            printf("%s\n", login_noa);
            printf("%s\n", login_fatima);
        } else {
            if (strcmp(modo, "-n") == 0) {
                printf("%s\n", nombre_noa);
                printf("%s\n", nombre_fatima);
            } else {
                printf("%s no es un especificador válido\n",modo);
            }
        }
    }

}

void pillar_pid( tItemL comando ){ //char *modo, int ntokens
    //printf("entra en pid\n");
    if( comando.tokens == 1 ){ //no se ha especificado -p
        //int PID = ;
        printf("Pid de shell: %d\n", getpid());
    }else{
        tItemT modo;
        getToken(0,comando.comandos,modo);
        if( strcmp(modo, "-p") == 0 ){ //hay que sacar el proceso padre de la shell
            //int PID = ;
            printf("Pid del padre del shell: %d\n", getppid());
        }else{
            printf("%s no es un especificador válido\n",modo);
        }

    }

}

void carpeta( tItemL comando ){

    if ( comando.tokens == 1 ){ //no se han recibido argumentos adicionales
        getDir();
    }else { //se cambia de directorio
        tItemT modo;
        getToken(0,comando.comandos,modo);
        int error = chdir(modo);
        if( error == -1 ){
            perror("no se ha podido cambiar de directorio");
            strerror(errno);
        }
    }

}

void fecha( tItemL comando){

    int horas,minutos,segundos,dia,mes,anho;

    time_t now;//tipo de tiempo aritmetico
    time(&now);
    if( errno == -1) {
        perror("no se ha podido conseguir la fecha actual"); //'time()' devuelve la hora actual del sistema como un valor 'time_t'
        strerror(errno);
    }

    //localtime convierte un valor de 'time_t' a la hora del calendario

    struct tm *local = localtime(&now);

    horas= local->tm_hour;
    //aqui obtiene horas desde la medianoche
    minutos=local->tm_min;
    //obtiene los minutos
    segundos=local->tm_sec;
    //obtiene los segundos

    dia=local->tm_mday;
    //obtiene el dia del mes (1 al 31)
    mes=local->tm_mon+1;
    //ontiene el mes del año del 0 al 11
    anho=local->tm_year + 1900;
    //obtiene el año desde 1900

    //ya tenemos hechas todas las variables

    if(comando.tokens == 1){//tiene que imprimir la fecha y la hora

        printf("%02d:%02d:%02d \n",horas,minutos,segundos);
        printf("%02d/%02d/%d\n",dia,mes,anho);

    }else {
        tItemT modo;
        getToken(0,comando.comandos,modo);

        if (strcmp(modo, "-d") == 0) {//imprime solo la fecha actual
            printf("%02d/%02d/%d\n", dia, mes, anho);
        } else {
            if (strcmp(modo, "-h") == 0) {//imprime solo la hora actual
                printf("%02d:%02d:%02d \n", horas, minutos, segundos);
            }else{
                printf("%s no es un especificador válido\n",modo);
            }
        }
    }

}

void hist ( tItemL comando, tList *hist){

    if (comando.tokens == 1 && !isEmptyList(*hist)){ //imprimir la lista
        tPosL LastNode = primero(*hist);
        while(LastNode != LNULL){
            tItemL objeto = getItem(LastNode,*hist);
            printComand(objeto);
            LastNode = LastNode->next;
        }
    }else{
        tItemT modo;
        getToken(0,comando.comandos, modo);

        if( strcmp(modo, "-c") == 0 ) {
            deleteList(hist);
            createList(hist);
        }else{
            int N = int_convert(modo);
            tPosL LastNode = primero(*hist);
            int i = 0;
            do{
                tItemL objeto = getItem(LastNode,*hist);
                printComand(objeto);
                LastNode = next(LastNode, *hist);
                i++;
            }while( i<=N-1 && LastNode != NULL );
        }
    }
}

void status(tItemL comando){ //y si pasamos directorios y archivos a la vez??
    if(comando.tokens == 1){
        getDir();
    }else{
        modo modo;
        modo.largo = false;
        modo.acc = false;
        modo.link = false;
        modo.reca = false;
        modo.recb = false;
        modo.hid = false;
        modo.listar = false;
        int controlador = opciones(comando,&modo); //controlador cuenta el número de -long, -link, -acc que hay; además de que es la posición del 1er path en el array de tokens

        if(comando.tokens-1 == controlador){ //se ha terminado el bucle sin ningún path/file posible -> imprimir ruta actual
            getDir();
        }else{
            for(int j = controlador-1; j <= comando.comandos.lastPos-1 ; j++){ //recorrer todos los archivos que han entrado por terminal
                char path[MAX_LENGHT_PATH]; getToken(j+1, comando.comandos, path);
                char ruta[MAX_LENGHT_PATH];
                if( realpath(path,ruta) == NULL) {
                    perror("error al obtener el path");
                    strerror(errno);
                }else {
                    if ( printInfo(ruta,path,&modo) == -1) strerror(errno);
                }
            }
        }
    }
}

void listar(tItemL comando){
    if(comando.tokens == 1){
        getDir();
    }else{
        modo modo;
        modo.largo = false;
        modo.acc = false;
        modo.link = false;
        modo.reca = false;
        modo.recb = false;
        modo.hid = false;
        modo.listar = true;
        int controlador = opciones(comando,&modo);

        //impresión de la petición
        if(comando.tokens-1 == controlador){ //se ha terminado el bucle sin ningún path/file posible -> imprimir ruta actual
            getDir();
        }else{
            char path[MAX_LENGHT_PATH];
            getToken(controlador,comando.comandos,path);

            if(!modo.reca && !modo.recb){
                if( ListContent(path,&modo) == -1 ) strerror(errno);
            }else{
                if(modo.reca){
                    if ( ListReca(path,&modo) == -1 ) strerror(errno);
                }
                if(modo.recb){
                    if ( ListRecb(path,&modo) == -1 ) strerror(errno);
                }
            }

        }

    }
}

void create (tItemL comando){

     if(comando.tokens==1){
         getDir();
     }

     if(comando.tokens !=0) {
         char path[MAX_LENGHT_PATH];
         char error[MAX_LENGHT_PATH] = "Error, imposible de crear";

         getcwd(path,sizeof (path));
         strcat(path,"/");

         tItemT modo;
         getToken(0, comando.comandos, modo);

         if (strcmp("-f", modo) == 0) {//crear un archivo

             getToken(1, comando.comandos, modo);

             char *name_folder = modo;


             if (creat(strcat(path, name_folder), 0666) == -1) {//creamos el documento y le pasamos el permiso
                 perror(error);//salta error si no se puede crear
             }
         }else {


             char *name_folder = modo;

             if(mkdir(strcat(path,name_folder),0755)==-1){//creamos el directorio y le pasamos el permiso
                 perror(error);//en el caso de que no se pueda crear salta el error
             }

         }


     }
}

int delete(tItemL comando) {//borra documentos o directorios vacios

    char error[MAX_LENGHT_PATH] = "Error, imposible de borrar";

    if(comando.tokens == 1) getDir();
    else {
        if (comando.tokens != 0) {
            int controlador1, controlador2, auxiliar, auxiliar1;
            tItemT aux;
            for (int i = 0; i < comando.tokens - 1; i++) {//aqui recorremos todos los tokens ns si hay que poner -2

                getToken(i, comando.comandos, aux);

                controlador1 = isDirectory(aux);
                controlador2 = isDirEmpty(aux);

                if (controlador1 != 0 && controlador2 == 1) {//si es directorio y esta vacio lo borra

                    auxiliar = remove(aux);

                    if (auxiliar != 0) {//si no ha sido posible borrarlo salta error

                        perror(error);
                        return -1;

                    }
                } else {//sino es documento

                    auxiliar1 = remove(aux);//segun ivan esto es para eliminar files

                    if (auxiliar1 != 0) {
                        perror(error);
                        return -1;
                    }
                }
            }
        }
    }
    return 0;
}

int deleteTree(tItemL comando){//borra recursivamente documentos y directorios no vacios

    int controlador1,controlador2,controlador3,auxiliar;

    char error[MAX_LENGHT_PATH] = "no se ha podido borrar";
    if(comando.tokens == 1)getDir();
    else{
        if (comando.tokens != 0) {

            tItemT aux;

            for (int i = 0; i < comando.tokens - 1; i++) {//ns si hay que poner el -2

                getToken(i, comando.comandos, aux);

                controlador1 = isDirectory(aux);
                controlador2 = isDirEmpty(aux);

                if (controlador1 != 0 && controlador2 == 0) {

                    borrar_dir(aux);//aqui lo borra recursivamente

                    controlador3 = isDirEmpty(aux);
                    if (controlador3 == 1) {
                        auxiliar = remove(aux);

                        if (auxiliar != 0) {//si no ha sido posible borrarlo salta error
                            perror(error);

                        }
                    }

                } else {

                    unlink(aux);

                    //sino es directorio lo borra con unlink

                }
            }
        }
    }
    return 0;
}
