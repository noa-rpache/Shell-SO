/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

//quitar las que ya vengan en el list.h para que no sean redundantes
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> //esta venía en el man C de google
#include <sys/utsname.h> //esto es para infosis
#include "list.h"
#include <time.h>
//#define MAX_INPUT 100 -> se define en la lista, pero mejor no usar ese -> está pendiente de cambiar


void procesarEntrada( char orden[MAX_LENGHT + 1], int ntokens, tList historial);

bool salir(char *cadena[], tList *historial){

    if ( strcmp(*cadena, "fin")  == 0 ){
        deleteList(historial);
        return true;
    }else{

        if (strcmp(*cadena, "salir") == 0){
            deleteList(historial);
            return true;
        }else{

            if( strcmp(*cadena, "bye") == 0 ) {
                deleteList(historial);
                return true;
            }else{ //si NO ES NINGUNO de los tres
                return false;
            }

        }


    }
}//check //devuelve true si hay que salir, false si no

void ayuda(char *comando, int ntokens, tList *historial){ //como manda el mismo mensaje dando igual los especificadores del comando solo hace falta el comando

    if(ntokens == 1){
        printf("'ayuda cmd' donde cmd es uno de los siguientes comandos:\n");
        printf("fin salir bye fecha pid autores hist comando carpeta infosis ayuda\n");

    }else{
        if(strcmp(comando, "fin") == 0) printf("fin\tTermina la ejecucion del shell");
        else{
            if(strcmp(comando, "salir") == 0) printf("salir\tTermina la ejecucion del shell");
            else{
                if(strcmp(comando, "bye") == 0) printf("bye\tTermina la ejecucion del shell");
                else{
                    if (strcmp(comando, "autores") == 0) printf("autores [-n|-l]\tMuestra los nombres y logins de los autores\n");
                    else{
                        if (strcmp(comando, "pid") == 0) printf("pid [-p]\tMuestra el pid del shell o de su proceso padre\n");
                        else{
                            if (strcmp(comando, "carpeta") == 0) printf("carpeta [dir]\tCambia (o muestra) el directorio actual del shell\n");
                            else{
                                if(strcmp(comando, "fecha") == 0) printf("fecha [-d|-h]\tMuestra la fecha y o la hora actual\n");
                                else{
                                    if(strcmp(comando, "hist") == 0) printf("hist [-c|-N]\tMuestra el historico de comandos, con -c lo borra\n");
                                    else{
                                        if(strcmp(comando, "comando") == 0) printf("comando [-N]\tRepite el comando N (del historico)\n");
                                        else{
                                            if( strcmp(comando, "infosis") == 0 ) printf("infosis \tMuestra informacion de la maquina donde corre el shell\n");
                                            else{
                                                if( strcmp(comando, "ayuda") == 0 ) printf("ayuda [cmd]\tMuestra ayuda sobre los comandos");
                                                else{
                                                    printf("%s no encontrado\n", comando);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
} //check

void infosis(){

    struct utsname informacion;

    uname(&informacion);

    printf("%s (%s), OS: %s %s %s\n", informacion.nodename, informacion.machine, informacion.sysname,informacion.release, informacion.version);

}//en cuanto lo probemos a ejecutar cambiamos el orden para que coincida con el de la shell

void repetir_comando(char pos, tList hist){

    int posicion = (int)pos;

    tItemL repeticion = getItem(findItem(posicion, hist), hist);
    printf("Ejecutando hist (%d): %s\n",posicion, repeticion.comando);
    procesarEntrada(repeticion.comando, repeticion.tokens, hist);

} //check (?)

void autores( char modo, int ntokens ){
    char nombre_noa[] = "Noa Rodriguez Pache", login_noa[] = "noa.rpache";
    char nombre_fatima[] = "Fátima Ansemil Barros", login_fatima[] = "fatima.ansemil";

    if ( ntokens == 1 ){ //no se especifica nada
        printf("%s: %s\n", nombre_noa, login_noa);
        printf("%s: %s\n", nombre_fatima, login_fatima);

    }else {

        if (strcmp(&modo, "-l") == 0) {
            printf("%s\n", login_noa);
            printf("%s\n", login_fatima);
        } else {
            if (strcmp(&modo, "-p") == 0) {
                printf("%s\n", nombre_noa);
                printf("%s\n", nombre_fatima);
            } else {
                printf("no ha introducido un especificador válido\n");
            }
        }
    }

} // check

void pillar_pid( char modo, int ntokens){

    if( ntokens == 1 ){ //no se ha especificado -p
        int PID = getpid();
        printf("Pid de shell: %d\n", PID);
    }else{

        if( strcmp(&modo, "p") == 0 ){ //hay que sacar el proceso padre de la shell
            int PID = getppid();
            printf("Pid del padre del shell: %d\n", PID);
        }else{

            printf("no ha introducido un especificador válido\n");

        }

    }

} //check

void carpeta(char modo, int ntokens){

    if ( ntokens == 1 ){ //no se han recibido argumentos adicionales

        char *get_current_dir_name(void); //así queda guardada aquí la cadena que contiene la ruta al directorio actual
        printf("%s\n", get_current_dir_name());

    }else { //se cambia de directorio

        chdir(&modo); //no tengo claro cómo funciona

    }

} //falta que salte un aviso cuando metes un directorio raro

void fecha(char modo, int ntokens){

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

    }else if(strcmp(&modo,"-d")==0){//imprime solo la fecha actual
        printf("%02d/%02d/%d\n",dia,mes,anho);

    }else if(strcmp(&modo,"h")==0){//imprime solo la hora actual
        printf("%02d:%02d:%02d \n",horas,minutos,segundos);
    }

} //check

void printPrompt(){
    printf(">> ");
} //check

int TrocearCadena(char *cadena, char *trozos[]){

    int i=1;

    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    else{
        while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
            i++;
        return i;
    }

} //check

void leerEntrada( char *entrada[], char *comandos_separados[], int *ntokens){

	fgets(*entrada, MAX_LENGHT, stdin);
	*ntokens = TrocearCadena(*entrada, comandos_separados);

} //check (?)

void new_historial(char *comando, int numero, tList *hist){

    tItemL nuevo;
    strcpy(nuevo.comando, comando); //ya está separado
    nuevo.puesto = numero; //posición en la lista de
    if ( !insertElement(nuevo, hist) ) printf("no se ha insertado el elemento\n"); //mensaje error

} //check(?)


int main(int argc, char *arvg[]){ //nº de argumentos recibidos, array con las direcciones a dichos argumentos

    tList historial;
    createList(&historial);
    bool salida = false; //comprobador de condición de salida
    int contador = 0; //número de ítems en el historial

    while( salida == false ){ //mientras no se cumpla ninguna condición de salida

        contador++;
        char *orden_procesada[MAX_LENGHT + 1]; //comandos separados
        int ntokens = 0;

        printPrompt();
        leerEntrada(arvg,orden_procesada, &ntokens);
        //si a partir de aquí solo se usa orden_procesada no habrá problemas con el resto de funciones porque todas usarán el mismo formato de char[]
        //ntokens es el número de argumentos que se han recibido

        salida = salir(orden_procesada, &historial);
        if(!salida){
            new_historial( *orden_procesada,contador, &historial); //se tiene que guardar el comando si no está bien escrito -> SÍ
            procesarEntrada(*orden_procesada, ntokens, historial);
        }

    }

    return 0;
}


void procesarEntrada( char orden[MAX_LENGHT + 1], int ntokens, tList historial ){

    if (strcmp(&orden[0], "autores") == 0) autores(orden[1], ntokens);
    else if (strcmp(&orden[0], "pid") == 0) pillar_pid(orden[1],ntokens);
    else if(strcmp(&orden[0], "carpeta") == 0) carpeta(orden[1],ntokens);
    else if(strcmp(&orden[0], "fecha") == 0) fecha(orden[1],ntokens);
    //else if(strcmp(&orden[0], "hist") == 0) ;
    else if(strcmp(&orden[0], "comando") == 0) repetir_comando(orden[1],historial);
    else if(strcmp(&orden[0], "infosis") == 0) infosis();
    else if(strcmp(&orden[0], "ayuda") == 0) ayuda(&orden[1], ntokens, &historial);
    else printf("%s: no es un comando del shell\n", &orden[0]);

}




