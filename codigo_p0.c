/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> //esta venía en el man C de google
#include <sys/utsname.h> //esto es para infosis
#include "list.h"
//#define MAX_INPUT 100 -> se define en la lista, pero mejor no usar ese -> está pendiente de cambiar


void procesarEntrada( char orden[MAX_LENGHT + 1], int ntokens ){

    perror(orden);

    switch(/*comando principal*/){

        //aquí se introducen las funciones del resto con sus modificadores y eso

        default: printf("%s: no es un comando del shell", orden);
            break;
    }

} //falta bastante

bool salir(char *cadena[]){

    if ( strcmp(*cadena, "fin")  == 0 ){ //si es fin O salir
        return true;
    }else{

        if (strcmp(*cadena, "salir") == 0){
            return true;
        }else{

            if( strcmp(*cadena, "bye") == 0 ) { //si es bye
                return true;
            }else{ //si NO ES NINGUNO de los tres
                return false;
            }

        }


    }
}//check (?) //devuelve true si hay que salir, false si no

void ayuda(char *comando, int ntokens){ //como manda el mismo mensaje dando igual los especificadores del comando solo hace falta el comando

    if(ntokens == 0){
        printf("'ayuda cmd' donde cmd es uno de los siguientes comandos:\n");
        printf("fin salir bye fecha pid autores hist comando carpeta infosis ayuda\n");

    }else{
        if(salir(&comando)) printf("%s\tTermina la ejecucion del shell", comando); //creo que no está bonito el printf
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
                                            printf("%s no encontrado", comando);
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
} //check (?)

void infosis(){

    struct utsname informacion;

    uname(&informacion);

    printf("%s %s %s %s %s\n", informacion.machine, informacion.nodename, informacion.release, informacion.sysname, informacion.version);

}//en cuanto lo probemos a ejecutar cambiamos el orden para que coincida con el de la shell

void repetir_comando(int pos, tList hist){

    tItemL repeticion = getItem(findItem(pos, hist), hist);

    procesarEntrada(repeticion.comando, repeticion.tokens);

} //check (?)

void autores( char modo, int ntokens ){
    char nombre_noa[] = "Noa Rodriguez Pache", login_noa[] = "noa.rpache";
    char nombre_fatima[] = {}, login_fatima[] = {};

    if ( ntokens == 0 ){ //no se especifica nada
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

} // check (?)

void pillar_pid( char modo, int ntokens){

    char p;
    int PID;

    if( ntokens == 0 ){ //no se ha especificado -p
        PID = getpid();
        printf("Pid de shell: %d\n", PID);
    }else{

        if( strcmp(&modo, &p) == 0 ){ //hay que sacar el proceso padre de la shell
            PID = getppid();
            printf("Pid del padre del shell: %d\n", PID);
        }else{

            printf("no ha introducido un especificador válido\n");

        }

    }

} //check (?)

void carpeta(char modo, int ntokens){

    if ( ntokens == 0 ){ //no se han recibido argumentos

        char *get_current_dir_name(void); //así queda guardada aquí la cadena que contiene la ruta al directorio actual
        printf("%s", get_current_dir_name());

    }else { //se cambia de directorio

        chdir(&modo); //no tengo claro cómo funciona

    }

} //falta que salte un aviso cuando metes un directorio raro


void printPrompt(){
    printf(">>");
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

void leerEntrada( char *entrada[], char *comandos_separados[]){

	fgets(*entrada, MAX_LENGHT, stdin);

	TrocearCadena(*entrada, comandos_separados);

} //check (?)

void new_historial(char *comando, int numero, tList *hist){ //el comando tiene que llevar los tokens incluidos, así nos ahorramos pensar cómo guardarlos

    tItemL nuevo;
    strcpy(nuevo.comando, comando);
    nuevo.puesto = numero;
    if ( !insertElement(nuevo, LNULL, hist) ) return ; //mensaje error


} //check(?)


int main(int argc, char *arvg[]){ //nº de argumentos recibidos, array con las direcciones a dichos argumentos

    tList historial;
    bool salida = false; //comprobador de condición de salida
    int contador = 0; //número de ítems en el historial

    while( salida == false ){ //mientras no se cumpla ninguna condición de salida

        contador++;
        char *orden_procesada[MAX_LENGHT + 1]; //comandos separados

        printPrompt();
        leerEntrada(arvg,orden_procesada); printf("\n");
        //si a partir de aquí solo se usa orden_procesada no habrá problemas con el resto de funciones porque todas usarán el mismo formato de char[]

        salida = salir(orden_procesada);
        new_historial( *orden_procesada,contador, &historial); //se tiene que guardar el comando si no está bien escrito -> SÍ
        procesarEntrada(*orden_procesada, argc);

    }

    return 0;
}







