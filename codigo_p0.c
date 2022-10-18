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
//#include <pwd.h> //utilidades fichero password -> para gid y uid
//#include <sys/dir.h> //utilidades sobre directorios
#include <dirent.h> //para opendir()
#include "historial.h"
//#define MAX_INPUT 100 -> se define en la lista, pero mejor no usar ese -> está pendiente de cambiar
#define MAX_LENGHT_PATH 100 //para cuando se quieran arrays de nombres de directorios


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
char * ConvierteModo (mode_t m, char *permisos);
int printInfo(char ruta[MAX_LENGHT_PATH], char enlazada[MAX_LENGHT_PATH], bool largo, bool link, bool acc);

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

int main(int argc, char *arvg[]){

    tList historial;
    createList(&historial);
    bool salida = false;

    while( salida == false ){
        leerEntrada(arvg,&historial);
        salida = procesarEntrada(&historial);
        //printf("antes de salir del while\n");
    }
    //printf("antes de borrar el historial\n");
    deleteList(&historial);
    return 0;
}


//funciones generales
bool procesarEntrada(tList *historial){

    if( !isEmptyList(*historial)) {
        tItemL peticion = getItem(last(*historial), *historial);

        if (salir(peticion.comando)) return true;
        else{
            /*
            tItemL prueba = getItem(primero(*historial), *historial);
            if(!isEmptyTokensList(prueba.comandos)){
                tItemT aux;
                getToken(firstToken(prueba.comandos), prueba.comandos, aux);
                printf("comando: %s %s\n", prueba.comando, aux);
            }

            printf("comando: %s\n", prueba.comando);
            */
            if(strcmp(peticion.comando,"vacia") == 0) deleteLast(last(*historial), historial); //peticion.tokens == 0
            else if (strcmp(peticion.comando, "autores") == 0) autores(peticion);
            else if (strcmp(peticion.comando, "comando") == 0) repetir_comando(peticion, historial);
            else if (strcmp(peticion.comando, "pid") == 0) pillar_pid(peticion);
            else if (strcmp(peticion.comando, "carpeta") == 0) carpeta(peticion);
            else if (strcmp(peticion.comando, "fecha") == 0) fecha(peticion);
            else if (strcmp(peticion.comando, "infosis") == 0) infosis();
            else if (strcmp(peticion.comando, "ayuda") == 0) ayuda(peticion);
            else if (strcmp(peticion.comando, "hist") == 0) hist(peticion, historial);
            else if (strcmp(peticion.comando, "create") == 0) printf("*create en construcción*\n");
            else if (strcmp(peticion.comando, "stat") == 0) {
                printf("*stat en construcción*\n");
                status(peticion);
            } else if (strcmp(peticion.comando, "list") == 0) {
                printf("*list en construcción*\n");
                listar(peticion);
            } else if (strcmp(peticion.comando, "delete") == 0) printf("*delete en construcción*\n");
            else if (strcmp(peticion.comando, "deltree") == 0) printf("*deltree en construcción*\n");
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
        perror(strerror(errno));
    }else{
        printf("%s\n", directorio );
    }
}

char * ConvierteModo (mode_t m, char *permisos){
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

int printInfo(char ruta[MAX_LENGHT_PATH], char enlazada[MAX_LENGHT_PATH], bool largo, bool link, bool acc) {
    struct stat contenido;
    int salir = stat(ruta, &contenido);

    if (salir == -1) { //esto ya da error cuando metes algo que no es un path/file
        printf("path: %s\n", ruta);
        perror(strerror(errno));
        return -1;
    } else {
        printf("imprimir información\n\t");

        if (largo || acc) { //imprimir la hora de último acceso
            /*
            struct tm *time;
            char ultacceso[15];
            if ((time = localtime(contenido.st_atim)) == NULL) {
                perror(strerror(errno));
                return -1;
            }
            if (strftime(ultacceso, sizeof(ultacceso), "%Y-%m-%d %H:%M", time) == 0) {
                perror(strerror(errno));
                return -1;
            }
            */

            printf("AAAA/MM/DD - HH:mm "); //printf("%s", ultacceso);

        }


        if (largo) {
            printf("%ld ", (long) contenido.st_nlink);
            printf("(%ld) ", (long) contenido.st_ino);

            struct group *grupinho = malloc(sizeof(struct group));
            char buffer[MAX_LENGHT_PATH]; //Compila, pero no es tamaño suficiente o adecuado
            struct group *error;
            getgrgid_r(contenido.st_gid, grupinho, buffer, sizeof(MAX_LENGHT), &error);

            if (error == NULL) {
                if (errno != 0) perror(strerror(errno));
                else printf("El id %d no tiene grupo asociado.\n", contenido.st_gid);
                return -1;
            } else {
                printf("%s", grupinho->gr_name);
            }

            //contenido.st_uid -> comprobar en /passwd

            printf("%c", LetraTF(contenido.st_mode));
            char *permisos = malloc(sizeof(10));
            ConvierteModo(contenido.st_mode, permisos);
            printf("%s", permisos);
        }

        printf("%ld ", (long) contenido.st_size); //st_size
        if (largo && link) {
            printf("%s -> ", enlazada);
        }
        printf("%s\n", ruta); //ya tiene los enlaces simbólicos incluidos
        return 0;
    }

}


//comandos
void ayuda( tItemL comando){ //como manda el mismo mensaje dando igual los especificadores del comando solo hace falta el comando

    if(comando.tokens == 1){
        printf("'ayuda cmd' donde cmd es uno de los siguientes comandos:\n");
        printf("fin salir bye fecha pid autores hist comando carpeta infosis ayuda create delete deltree stat list\n");
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
    else if( strcmp(comando.comando, "create") == 0 ) printf("create [-f] [name]\tCrea un directorio o un fichero (-f)\n");
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
    if( errno == -1 ) perror(strerror(errno));

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
    //el puesto se actualiza en insertElement

    if ( !insertElement(nuevo, hist) ) printf("no se ha insertado el elemento\n");
    procesarEntrada(hist);

}

void autores( tItemL comando ){ //char *modo, int ntokens
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
        int PID = getpid();
        printf("Pid de shell: %d\n", PID);
    }else{
        tItemT modo;
        getToken(0,comando.comandos,modo);
        if( strcmp(modo, "-p") == 0 ){ //hay que sacar el proceso padre de la shell
            int PID = getppid();
            printf("Pid del padre del shell: %d\n", PID);
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
            perror(strerror(errno));
        }
    }

}

void fecha( tItemL comando){

    int horas,minutos,segundos,dia,mes,anho;

    time_t now;//tipo de tiempo aritmetico
    time(&now);
    if( errno == -1) perror(strerror(errno)); //'time()' devuelve la hora actual del sistema como un valor 'time_t'

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
        int controlador=0; //controlador cuenta el número de -long, -link, -acc que hay; además de que es la posición del 1er path en el array de tokens
        bool largo=false, link=false,acc=false;

        //saber qué es lo que se ha introducido
        for(int i = 0; i<=comando.tokens-2; i++){ //tokens es el total de tokens, incluido el ppal
            tItemT aux; getToken(i,comando.comandos,aux);
            if(strcmp("-long",aux)==0){
                largo = true; //se ha detectado long
                controlador++;
            }else if(strcmp("-link",aux)==0){
                link = true;
                controlador++;
            }else if(strcmp("-acc",aux)==0){
                acc = true;
                controlador++;
            }else{
                i = comando.tokens-2; //no se ha detectado ninguno -> actualizar i para terminar el bucle
            }
        }

        //imprimir petición
        if(comando.tokens-1 == controlador){ //se ha terminado el bucle sin ningún path/file posible -> imprimir ruta actual
            getDir();
        }else{
            for(int j = controlador-1; j <= comando.comandos.lastPos-1 ; j++){ //recorrer todos los archivos
                tItemT path; getToken(j+1, comando.comandos, path);
                char ruta[MAX_LENGHT_PATH];
                if( realpath(path,ruta) == NULL) perror(strerror(errno));
                else printInfo(ruta,path,largo,link,acc); //hay incoherencias con algunos tamaños
            }
        }

    }
}

void listar(tItemL comando){
    if(comando.tokens == 1){
        getDir();
    }else{
        int controlador=0;
        bool largo=false, link=false, acc = false, hid = false, reca = false, recb = false;
        //saber lo que ha entrado
        for(int i = 0; i<=comando.tokens-2; i++){ //tokens es el total de tokens, incluido el ppal
            tItemT aux; getToken(i,comando.comandos,aux);

            if(strcmp("-long",aux)==0){
                largo = true; //se ha detectado long
                controlador++;
            }else if(strcmp("-link",aux)==0){
                link = true;
                controlador++;
            }else if(strcmp("-acc",aux)==0){
                acc = true;
                controlador++;
            }else if(strcmp("-hid",aux)==0){
                hid = true;
            }else if(strcmp("-reca",aux)==0) reca = true;
            else if(strcmp("-recb",aux)!=0) recb = true;
            else i = comando.tokens-2; //no se ha detectado ninguno -> actualizar i para terminar el bucle

        }

        //impresión de la petición
        if(comando.tokens-1 == controlador){ //se ha terminado el bucle sin ningún path/file posible -> imprimir ruta actual
            getDir();
        }else{ //falta incluir ficheros ocultos

            //recorrido normal sin reca ni recb, sin pensar en los hid
            char path[MAX_LENGHT_PATH];
            getToken(controlador,comando.comandos,path);
            printf("************%s\n",path);
            //¿qué se le pasa a scandir()??
            //printf("nombre directorio pre scandir: %s\n",directorio->d_name);
            struct dirent **namelist;
            int total_entradas = scandir(path,&namelist,NULL,alphasort); //guarda las entradas del directorio en namelist
            printf("total entradas: %d\n",total_entradas);

            if(total_entradas == -1){
                perror(strerror(errno));
                //return -1;
            }

            /*for(int i = 0; i <= total_entradas-1; i++){ //recorrer el directorio para mostrar el contenido
                //printf("nombre: %s\n tipo: %c\n",(**namelist).d_name, (**namelist).d_type );
                // /home/noa/Paradigmas_de_la_programación
                DIR *directory_stream = opendir(path);
                DIR *next_stream;
                struct dirent *directorio = readdir(directory_stream ); //readdir te apunta al primer directorio dentro del path que le pasas
                printf("nombre readdir:\n\tnombre: %s\n\ttipo: %c\n",(*directorio).d_name,(*directorio).d_type); //este es el bueno

                seekdir(next_stream,telldir(directory_stream) ); //seekdir(opendir(path),(*directorio).d_off);

                printf("nombre seekdir:\n\tnombre: %s\n\ttipo: %c\n",(*).d_name,(*directorio).d_type);


                realpath();
                printInfo(ruta real,ruta con enlaces,largo,link,acc);
                seekdir();

            }*/

            // /home/noa/Paradigmas_de_la_programación
            DIR *directory_stream = opendir(path);

            for(int i = 0; i <= total_entradas-1; i++) {
                struct dirent *directorio1 = readdir(directory_stream); //readdir te apunta al primer directorio dentro del path que le pasas
                printf("nombre readdir:\n\tnombre: %s\n\ttipo: %c\n", (*directorio1).d_name, (*directorio1).d_type);
                seekdir(directory_stream, telldir(directory_stream));
            }



            closedir(directory_stream);
        }

    }
}

