/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#include <stdio.h>
#include <unistd.h> //esta venía en el man C de google
#include <sys/utsname.h> //esto es para infosis
#include "list.h"
#include <time.h>
//#define MAX_INPUT 100 -> se define en la lista, pero mejor no usar ese -> está pendiente de cambiar


//generales
void procesarEntrada( char *orden[], int ntokens, tList historial);
bool salir(char *cadena[]); //check
void printPrompt(); //check
int TrocearCadena(char *cadena, char *trozos[]); //check
void leerEntrada( char *entrada[], char *comandos_separados[], int *ntokens);
void new_historial(char *comando, int numero, tList *hist, int ntokens);
void hist (char *comando, tList *hist, int ntokens);
int int_convert(char cadena[]);

//comandos
void ayuda(char *comando, int ntokens);
void infosis();
void repetir_comando(char *pos, tList hist);
void autores( char *modo, int ntokens );
void pillar_pid( char *modo, int ntokens);
void carpeta(char *modo, int ntokens);
void fecha(char *modo, int ntokens);


int main(int argc, char *arvg[]){ //nº de argumentos recibidos, array con las direcciones a dichos argumentos

    tList historial; //printf("declaras el historial\n");
    createList(&historial);
    bool salida = false; //comprobador de condición de salida
    int contador = 0; //número de ítems en el historial

    while( salida == false ){ //mientras no se cumpla ninguna condición de salida

        contador++;
        char *orden_procesada[MAX_LENGHT]; //comandos separados
        int ntokens=0;

        printPrompt();
        leerEntrada(arvg,orden_procesada, &ntokens); //printf("leyó la entrada\n");
        salida = salir(orden_procesada);
        if(!salida){
            new_historial(*orden_procesada,contador, &historial, ntokens); //printf("guardó en el historial\n");
            procesarEntrada(orden_procesada, ntokens, historial);
        }

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
} //check

void infosis(){

    struct utsname informacion;
    uname(&informacion);

    printf("%s (%s), OS: %s %s %s\n", informacion.nodename, informacion.machine, informacion.sysname,informacion.release, informacion.version);

}//check

void repetir_comando(char *pos, tList hist){
    int posicion = atoi(pos);

    tItemL repeticion = getItem(findItem(posicion, hist), hist);
    printf("Ejecutando hist (%d): %s\n",posicion, repeticion.comando);

    char *aux[MAX_LENGHT];
    //strcpy(aux,repeticion.comando);
    *aux = repeticion.comando;
    //printf("ha hecho el strcpy/asignación\n");
    procesarEntrada(aux, repeticion.tokens, hist);

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

} //check

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

} //check

void carpeta(char *modo, int ntokens){

    if ( ntokens == 1 ){ //no se han recibido argumentos adicionales
        //printf("se ha llegado al if\n");
        char *get_current_dir_name(void); //así queda guardada aquí la cadena que contiene la ruta al directorio actual
        //printf("no llega a imprimirlo\n");
        printf("%s\n", get_current_dir_name());

    }else { //se cambia de directorio

        chdir(modo); //no tengo claro cómo funciona

    }

} //falta que salte un aviso cuando metes un directorio raro

void fecha(char *modo, int ntokens){

    int horas,minutos,segundos,dia,mes,anho;

    time_t now;//tipo de tiempo aritmetico
    time(&now);//'time()' devuelve la hora actual del sistema como un valor 'time_t'

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

} //check

void hist (char *comando, tList *hist, int ntokens){
    int i=0;
    //printf("entra al historial\n");

    if (ntokens == 1){//imprimir la lista

        //printf("entra en el if\n");
        tPosL LastNode = primero(*hist);
        //printf("(1) %d - %s\n", getItem(LastNode,*hist).puesto, *(getItem(LastNode,*hist).comando) );

        while(LastNode->next != LNULL){
            tItemL objeto = getItem(LastNode,*hist);
            //printf("entrar el while\n\n");
            //printf("%d - %s ntokens %d\n", objeto.puesto, *objeto.comando, objeto.tokens );
            printf("%d - %s\n", objeto.puesto, objeto.comando);
            LastNode = LastNode->next;
        }

    }else{

        if( strcmp(comando, "-c") == 0 ) {

            deleteList(hist);
            createList(hist);
            //printf("ha borrado la lista\n");

        }else{//printf("entra a los N comandos\n");

            int valor = int_convert(comando);
            int N = 0;
            tPosL LastNode = first(*hist);

            do{
                printf("%d_%s\n", i-1, getItem(LastNode,*hist).comando);
                LastNode = next(LastNode, *hist);
                N++;
            }while(LastNode != NULL && N <valor);

        }

    }
}


void procesarEntrada( char *orden[], int ntokens, tList historial ){
    //printf("entró a procesar\n");

    if (strcmp(orden[0], "autores") == 0) autores(orden[1], ntokens);
    else if (strcmp(orden[0], "pid") == 0){
        //printf("entra el if\n\n");
        pillar_pid(orden[1],ntokens);
    }
    else if(strcmp(orden[0], "carpeta") == 0) carpeta(orden[1],ntokens);
    else if(strcmp(orden[0], "fecha") == 0) fecha(orden[1],ntokens);
    else if(strcmp(orden[0], "hist") == 0) hist(orden[1],&historial,ntokens);
    else if(strcmp(orden[0], "comando") == 0) repetir_comando(orden[1],historial);
    else if(strcmp(orden[0], "infosis") == 0) infosis();
    else if(strcmp(orden[0], "ayuda") == 0) ayuda(orden[1], ntokens);
    else printf("%s: no es un comando del shell\n", orden[0]);

}

bool salir(char *cadena[]){

    if (strcmp(*cadena, "fin") == 0) return true;
    else if (strcmp(*cadena, "salir") == 0) return true;
    else if( strcmp(*cadena, "bye") == 0 ) return true;
    else return false;

}//check

void printPrompt(){
    printf(">> ");
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

void leerEntrada( char *entrada[], char *comandos_separados[], int *ntokens){

    fgets(*entrada, MAX_LENGHT, stdin);
    *ntokens = TrocearCadena(*entrada, comandos_separados);

}

void new_historial(char *comando, int numero, tList *hist, int ntokens){

    tItemL nuevo;
    nuevo.puesto = numero; //orden de entrada en el historial
    nuevo.tokens = ntokens;

    //char *aux[MAX_LENGHT];
    strcpy(nuevo.comando,comando);
    //*nuevo.comando = comando;
    //strcpy(*nuevo.comando, comando); //ya está separado
    //printf("%s\n",nuevo.comando);

    if ( !insertElement(nuevo, hist) ) printf("no se ha insertado el elemento\n"); //mensaje error

}

int int_convert(char cadena[]){

    //quitarle el guión
    return atoi();
}


