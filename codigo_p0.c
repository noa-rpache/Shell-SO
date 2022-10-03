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
void procesarEntrada( char *orden[], int ntokens, tList *historial);
bool salir(char *cadena[]);
int TrocearCadena(char *cadena, char *trozos[]);
void leerEntrada( char *entrada[], char *comandos_separados[], int argc);
void new_historial(char *comando, tList *hist, int ntokens, int argc);
void hist (char *comando, tList *hist, int ntokens);
int int_convert(char *cadena[]);

//comandos
void ayuda(char *comando, int ntokens);
void infosis();
void repetir_comando(char *pos, tList hist);
void autores( char *modo, int ntokens );
void pillar_pid( char *modo, int ntokens);
void carpeta(char *modo, int ntokens);
void fecha(char *modo, int ntokens);


int main(int argc, char *arvg[]){

    tList historial;
    createList(&historial);
    bool salida = false;

    while( salida == false ){
        char *orden_procesada[MAX_LENGHT];

        leerEntrada(arvg,orden_procesada, argc); //printf("leyó la entrada\n");
        salida = salir(orden_procesada); //printf("no se sale\n");
        if(!salida) procesarEntrada(orden_procesada,argc,&historial);

    }

    deleteList(&historial);
    return 0;
}


void ayuda(char *comando, int ntokens){ //como manda el mismo mensaje dando igual los especificadores del comando solo hace falta el comando

    if(ntokens == 1){
        printf("'ayuda cmd' donde cmd es uno de los siguientes comandos:\n");
        printf("fin salir bye fecha pid autores hist comando carpeta infosis ayuda\n");
    }else if(strcmp(comando, "fin") == 0) printf("fin\tTermina la ejecucion del shell\n");
    else if(strcmp(comando, "salir") == 0) printf("salir\tTermina la ejecucion del shell\n");
    else if(strcmp(comando, "bye") == 0) printf("bye\tTermina la ejecucion del shell\n");
    else if (strcmp(comando, "autores") == 0) printf("autores [-n|-l]\tMuestra los nombres y logins de los autores\n");
    else if (strcmp(comando, "pid") == 0) printf("pid [-p]\tMuestra el pid del shell o de su proceso padre\n");
    else if (strcmp(comando, "carpeta") == 0) printf("carpeta [dir]\tCambia (o muestra) el directorio actual del shell\n");
    else if(strcmp(comando, "fecha") == 0) printf("fecha [-d|-h]\tMuestra la fecha y o la hora actual\n");
    else if(strcmp(comando, "hist") == 0) printf("hist [-c|-N]\tMuestra el historico de comandos, con -c lo borra\n");
    else if(strcmp(comando, "comando") == 0) printf("comando [-N]\tRepite el comando N (del historico)\n");
    else if( strcmp(comando, "infosis") == 0 ) printf("infosis \tMuestra informacion de la maquina donde corre el shell\n");
    else if( strcmp(comando, "ayuda") == 0 ) printf("ayuda [cmd]\tMuestra ayuda sobre los comandos\n");
    else printf("%s no encontrado\n", comando);
}

void infosis(){

    struct utsname informacion;
    uname(&informacion);
    if( errno == -1 ) perror(strerror(errno));

    printf("%s (%s), OS: %s %s %s\n", informacion.nodename, informacion.machine, informacion.sysname,informacion.release, informacion.version);

}

void repetir_comando(char *pos, tList hist){
    int posicion = atoi(pos);
    tItemL repeticion = getItem(findItem(posicion, hist), hist);
    printf("Ejecutando hist (%d): %s\n",posicion, repeticion.comando);

    char *aux[MAX_LENGHT];
    //strcpy(aux,repeticion.comando);
    *aux = repeticion.comando;
    //printf("ha hecho el strcpy/asignación\n");
    printf("%s\n",&repeticion.comando[1]);
    //procesarEntrada(aux, repeticion.tokens, hist);

}

void autores( char *modo, int ntokens ){
    char nombre_noa[] = "Noa Rodriguez Pache", login_noa[] = "noa.rpache";
    char nombre_fatima[] = "Fátima Ansemil Barros", login_fatima[] = "fatima.ansemil";

    //printf("%s\n", modo);
    if ( ntokens == 1 ){ //no se especifica nada
        printf("%s: %s\n", nombre_noa, login_noa);
        printf("%s: %s\n", nombre_fatima, login_fatima);

    }else {

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

void pillar_pid( char *modo, int ntokens){
    //printf("entra en pid\n");
    if( ntokens == 1 ){ //no se ha especificado -p
        int PID = getpid();
        printf("Pid de shell: %d\n", PID);
    }else{

        if( strcmp(modo, "-p") == 0 ){ //hay que sacar el proceso padre de la shell
            int PID = getppid();
            printf("Pid del padre del shell: %d\n", PID);
        }else{

            printf("no ha introducido un especificador válido\n");

        }

    }

}

void carpeta(char *modo, int ntokens){

    if ( ntokens == 1 ){ //no se han recibido argumentos adicionales
        //printf("se ha llegado al if\n")
        char directorio[MAX_LENGHT];
        //printf("no llega a imprimirlo\n");
        printf("%s\n", getcwd(directorio,MAX_LENGHT));

    }else { //se cambia de directorio
        chdir(modo);
        if( errno == -1 ) perror(strerror(errno));
    }

}

void fecha(char *modo, int ntokens){

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

    if(ntokens==1){//tiene que imprimir la fecha y la hora

        printf("%02d:%02d:%02d \n",horas,minutos,segundos);
        printf("%02d/%02d/%d\n",dia,mes,anho);

    }else if(strcmp(modo,"-d")==0){//imprime solo la fecha actual
        printf("%02d/%02d/%d\n",dia,mes,anho);

    }else if(strcmp(modo,"-h")==0){//imprime solo la hora actual
        printf("%02d:%02d:%02d \n",horas,minutos,segundos);
    }

}

void hist (char *comando, tList *hist, int ntokens){ //printf("entra al historial\n");

    if (ntokens == 1 && !isEmptyList(*hist)){ //imprimir la lista
        //printf("entra en el if\n");
        tPosL LastNode = primero(*hist);
        while(LastNode != LNULL){
            tItemL objeto = getItem(LastNode,*hist);
            //printf("entrar el while\n\n");
            printf("%d - %s\n", objeto.puesto, objeto.comando);
            LastNode = LastNode->next;
        }

    }else{

        if( strcmp(comando, "-c") == 0 ) {
            deleteList(hist);
            createList(hist);
            //printf("ha borrado la lista\n");

        }else{
            //printf("entra a los N comandos\n");
            int N = int_convert(&comando);
            //printf("tiene que imprimir N = %d entradas del historial\n",N);
            tPosL LastNode = primero(*hist);
            int i = 0;

            do{
                printf("%d - %s\n", i+1, getItem(LastNode,*hist).comando);
                LastNode = next(LastNode, *hist);
                i++;
            }while( i<=N-1 && LastNode != NULL );

        }

    }
}


void procesarEntrada(char *orden[], int ntokens, tList *historial){

    if(strcmp(orden[0],"\n") != 0 && strcmp(orden[0],"\0") != 0) {
        new_historial(*orden,historial,ntokens,ntokens);

        tItemL prueba = getItem(primero(*historial),*historial);
        tItemT aux; getToken(firstToken(prueba.comandos),prueba.comandos, aux);

        printf("%s %s\n", prueba.comando, aux);

        /*
        if (strcmp(orden[0], "autores") == 0) autores(orden[1], ntokens);
        else if (strcmp(orden[0], "pid") == 0) pillar_pid(orden[1], ntokens);
        else if (strcmp(orden[0], "carpeta") == 0) carpeta(orden[1], ntokens);
        else if (strcmp(orden[0], "fecha") == 0) fecha(orden[1], ntokens);
        else if (strcmp(orden[0], "hist") == 0) hist(orden[1], historial, ntokens);
        else if (strcmp(orden[0], "comando") == 0) repetir_comando(orden[1], *historial);
        else if (strcmp(orden[0], "infosis") == 0) infosis();
        else if (strcmp(orden[0], "ayuda") == 0) ayuda(orden[1], ntokens);
        else if (strcmp(orden[0], "\0") == 0);
        else printf("%s: no es un comando del shell\n", orden[0]);
         */
    }
}

bool salir(char *cadena[]){

    if (strcmp(cadena[0], "fin") == 0) return true;
    else if (strcmp(cadena[0], "salir") == 0) return true;
    else if( strcmp(cadena[0], "bye") == 0 ) return true;
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

void leerEntrada(char *entrada[], char *comandos_separados[], int argc){

    printf(">> ");
    fgets(*entrada, MAX_LENGHT, stdin); //printf("%s",*entrada);
    if(strcmp("\n",*entrada) != 0 && argc != TrocearCadena(*entrada, comandos_separados))
        //printf("ha ocurrido un error en la entrada de datos\n");

}

void new_historial(char *comando, tList *hist, int ntokens, int argc){

    tItemL nuevo;
    nuevo.tokens = ntokens;
    strcpy(nuevo.comando,comando);
    createEmptyTokensList(&nuevo.comandos);

    for(int i = 1; i < argc; i++) //empieza en 1 porque ya se copió el valor en 0
        if( !insertToken(&comando[i],&nuevo.comandos) ) printf("no se ha insertado el token %d\n", i);

    if ( !insertElement(nuevo, hist) ) printf("no se ha insertado el elemento\n");
    printf("ha guardado bien\n");

}

int int_convert(char *cadena[]){
    int convertido = atoi(*cadena);
    return convertido*(-1);
}


