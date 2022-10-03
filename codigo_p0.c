/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#include <stdio.h>
#include <unistd.h> //esta venía en el man C de google
#include <sys/utsname.h> //esto es para infosis
#include "historial.h"
#include <time.h>
#include <errno.h>
//#define MAX_INPUT 100 -> se define en la lista, pero mejor no usar ese -> está pendiente de cambiar
//#define MAX_LENGHT_PATH


//generales
bool procesarEntrada(tList *historial);
bool salir(char cadena[]);
int TrocearCadena(char *cadena, char *trozos[]);
void leerEntrada( char *entrada[], tList *historial);
void new_historial(char *comando[], tList *hist, int ntokens);
int int_convert(tItemT cadena);
void printComand(tItemL impresion);

//comandos
void ayuda(tItemL comando);
void infosis();
void repetir_comando(tItemL entrada, tList *hist);
void autores( tItemL comando );
void pillar_pid( tItemL comando);
void carpeta( tItemL comando);
void fecha( tItemL comando);
void hist (tItemL comando, tList *hist);


int main(int argc, char *arvg[]){

    tList historial;
    createList(&historial);
    bool salida = false;

    while( salida == false ){
        leerEntrada(arvg,&historial); //printf("leyó la entrada\n");
        salida = procesarEntrada(&historial);
    }

    deleteList(&historial);
    return 0;
}


void ayuda( tItemL comando){ //como manda el mismo mensaje dando igual los especificadores del comando solo hace falta el comando

    if(comando.tokens == 1){
        printf("'ayuda cmd' donde cmd es uno de los siguientes comandos:\n");
        printf("fin salir bye fecha pid autores hist comando carpeta infosis ayuda\n");
    }else if(strcmp(comando.comando, "fin") == 0) printf("fin\tTermina la ejecucion del shell\n");
    else if(strcmp(comando.comando, "salir") == 0) printf("salir\tTermina la ejecucion del shell\n");
    else if(strcmp(comando.comando, "bye") == 0) printf("bye\tTermina la ejecucion del shell\n");
    else if (strcmp(comando.comando, "autores") == 0) printf("autores [-n|-l]\tMuestra los nombres y logins de los autores\n");
    else if (strcmp(comando.comando, "pid") == 0) printf("pid [-p]\tMuestra el pid del shell o de su proceso padre\n");
    else if (strcmp(comando.comando, "carpeta") == 0) printf("carpeta [dir]\tCambia (o muestra) el directorio actual del shell\n");
    else if(strcmp(comando.comando, "fecha") == 0) printf("fecha [-d|-h]\tMuestra la fecha y o la hora actual\n");
    else if(strcmp(comando.comando, "hist") == 0) printf("hist [-c|-N]\tMuestra el historico de comandos, con -c lo borra\n");
    else if(strcmp(comando.comando, "comando") == 0) printf("comando [-N]\tRepite el comando N (del historico)\n");
    else if( strcmp(comando.comando, "infosis") == 0 ) printf("infosis \tMuestra informacion de la maquina donde corre el shell\n");
    else if( strcmp(comando.comando, "ayuda") == 0 ) printf("ayuda [cmd]\tMuestra ayuda sobre los comandos\n");
    else printf("%s no encontrado\n", comando.comando);
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

    }else {
        tItemT modo; getToken(0,comando.comandos, modo);
        if (strcmp("-l", modo) == 0) {
            printf("%s\n", login_noa);
            printf("%s\n", login_fatima);
        } else {
            if (strcmp(modo, "-n") == 0) {
                printf("%s\n", nombre_noa);
                printf("%s\n", nombre_fatima);
            } else {
                printf("no ha introducido un especificador válido\n");
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
            printf("no ha introducido un especificador válido\n");
        }

    }

}

void carpeta( tItemL comando ){ //char *modo, int ntokens

    if ( comando.tokens == 1 ){ //no se han recibido argumentos adicionales
        //printf("se ha llegado al if\n");
        char *directorio[MAX_LENGHT];
        //printf("no llega a imprimirlo\n");
        printf("%s\n", getcwd(*directorio,sizeof(directorio)) );

    }else { //se cambia de directorio
        tItemT modo;
        getToken(0,comando.comandos,modo);
        chdir(modo);
        if( errno == -1 ) perror(strerror(errno));
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

        } else if (strcmp(modo, "-h") == 0) {//imprime solo la hora actual
            printf("%02d:%02d:%02d \n", horas, minutos, segundos);
        }
    }

}

void hist ( tItemL comando, tList *hist){ //char *comando, tList *hist, int ntokens

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


bool procesarEntrada(tList *historial){ //char *orden[], int ntokens, tList *historial
    tItemL peticion = getItem((*historial)->last, *historial);

    if(strcmp(peticion.comando,"\n") != 0 || strcmp(peticion.comando,"\0") != 0) {
        if(salir(peticion.comando)) return true;
        else {
            /*
            tItemL prueba = getItem(primero(*historial), *historial);
            if(!isEmptyTokensList(prueba.comandos)){
                tItemT aux;
                getToken(firstToken(prueba.comandos), prueba.comandos, aux);
                printf("comando: %s %s\n", prueba.comando, aux);
            }

            printf("comando: %s\n", prueba.comando);
            */
            if (strcmp(peticion.comando, "autores") == 0) autores(peticion);
            else if (strcmp(peticion.comando, "comando") == 0) repetir_comando(peticion,historial);
            else if (strcmp(peticion.comando, "pid") == 0) pillar_pid(peticion);
            else if (strcmp(peticion.comando, "carpeta") == 0) carpeta(peticion);
            else if (strcmp(peticion.comando, "fecha") == 0) fecha(peticion);
            else if (strcmp(peticion.comando, "infosis") == 0) infosis();
            else if (strcmp(peticion.comando, "ayuda") == 0) ayuda(peticion);
            else if (strcmp(peticion.comando, "hist") == 0) hist(peticion, historial);
            else if (strcmp(peticion.comando, "\0") == 0); //no sé qué va aquí
            else printf("%s: no es un comando del shell\n", peticion.comando);

            return false;
        }
    }
    return false;
}

bool salir(char cadena[]){

    if (strcmp(cadena, "fin") == 0) return true;
    else if (strcmp(cadena, "salir") == 0) return true;
    else if( strcmp(cadena, "bye") == 0 ) return true;
    else return false;

}

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

void leerEntrada(char *entrada[], tList *historial){
    char *orden_procesada[MAX_LENGHT];
    int ntokens = 0;

    printf(">> ");
    fgets(*entrada, MAX_LENGHT, stdin); //printf("%s",*entrada);
    if(strcmp("\n",*entrada) != 0 ) ntokens = TrocearCadena(*entrada,orden_procesada);
    new_historial(&orden_procesada[0], historial,ntokens);
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

int int_convert(tItemT cadena){
    int convertido = atoi(cadena);
    return convertido*(-1);
}

void printComand(tItemL impresion){
    printf("%d: %s", impresion.puesto, impresion.comando);
    if(impresion.tokens > 1) { //si hay algún token más que el comando ppal
        for (int i = 0; i < impresion.tokens - 1; i++) { //ntokens -1 == nº de especificadores
            printf(" %s ", impresion.comandos.data[i]);
        }
    }
    printf("\n");
}
