#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> //esta venía en el man C de google
#include <errno.h> //aquí se supone que está perror()
#include <sys/utsname.h> //esto es para infosis
#include "list.h"
#define MAX_INPUT 100

//incluir el tipo del historial

//COMANDOS

void procesarEntrada( char orden[MAX_INPUT] ){

    //mismo método de strcmp que en bool_salir

    switch(/*comando principal*/){

        //aquí se introducen las funciones del resto con sus modificadores y eso

        default: printf("ha ocurrido un error jeje");
            break;
    }

}

void infosis(){

    struct utsname informacion;

    uname(&informacion);

    printf("%s %s %s %s %s\n", informacion.machine, informacion.nodename, informacion.release, informacion.sysname, informacion.version);

}//en cuanto se ejecute cambiamos el orden para que coincida con el de la shell

void repetir_comando(int orden, tList hist){

    tItemL repeticion = getItem(findItem(orden, hist), hist);

    procesarEntrada(repeticion.comando);

}


void autores( char modo ){

    int opcion = (int)modo;

    char nombre_noa[] = "Noa Rodriguez Pache", login_noa[] = "noa.rpache";
    char nombre_fatima[] = {}, login_fatima[] = {};


    if ( strcmp(&modo, "\0") == 0 ){ //no se especifica nada
        printf("%s: %s\n", nombre_noa, login_noa);
        printf("%s: %s\n", nombre_fatima, login_fatima);
    }else{

        switch(opcion){
            case 108: //-l //mostrar logins
                printf("%s\n", login_noa);
                printf("%s\n", login_fatima);
                break;
            case 110: //-n //mostrar nombres
                printf("%s\n", nombre_noa);
                printf("%s\n", nombre_fatima);
                break;
            default: printf("no ha introducido un especificador válido\n");
                break;
        }

    }

} //creo que check, pero como aún no se ha probado nada pues no sé

void pillar_pid( char modo){

    char p;
    int PID;

    if( strcmp(&modo, "\0") == 0 ){ //significa que no se ha especificado el -p
        PID = getpid();
        printf("Pid de shell: %d", PID);
    }else{

        if( strcmp(&modo, &p) == 0 ){ //hay que sacar el proceso padre de la shell
            PID = getppid();
            printf("Pid del padre del shell: %d", PID);
        }else{

            printf("no ha introducido un especificador válido\n");

        }

    }

} //falta pid -p

void carpeta(char modo){

    if ( strcmp(&modo, "\0") == 0 ){ //no se han recibido argumentos

        char *get_current_dir_name(void); //así queda guardada aquí la cadena que contiene la ruta al directorio actual
        printf("%s", get_current_dir_name());

    }else { //se cambia de directorio

        chdir(&modo);

    }

} //falta que salte un aviso cuando metes un directorio raro


//FUNCIONES QUE NO SON COMANDOS

void printPrompt(){
    printf(">>");
} //check

bool salir(char *cadena[]){

    int fin = strcmp(*cadena, "fin");

	if ( fin == 0 ){ //si es fin O salir
		return true;
	}else{

        int salir = strcmp(*cadena, "salir");
        if (salir == 0){
            return true;
        }else{

            int bye = strcmp(*cadena, "bye");
            if( bye == 0 ) { //si es bye
                return true;
            }else{ //si NO ES NINGUNO de los tres
                return false;
            }

        }


    }
}//check //devuelve true si hay que salir, false si no

int TrocearCadena(char *cadena, char *trozos[]){

    int i=1;

    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    else{
        while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
            i++;
        return i;
    }

} //no tengo ni idea de cómo funciona así que no sé cómo usarla

void leerEntrada( char *entrada[], char *comandos_separados[]){

	fgets(*entrada, MAX_INPUT, stdin);
	
	TrocearCadena(*entrada, comandos_separados);

}

void new_historial(char *comando, int numero, tList hist){
    //llamar a la función de insertar nodo, y le pasamos el comando mdo texto y el número
} //recibe una cadena que almacena en la lista, falta la lista y sus funciones



int main(int argc, char *arvg[]){ //nº de argumentos recibidos, array con las direcciones a dichos argumentos

    tList historial;
    bool salida = false;
    int contador = 0;

    while( salida == false ){ //mientras no se cumpla ninguna condición de salida

        contador++;
        char *orden_procesada[] = {}; //comandos separados

        printPrompt();
        leerEntrada(arvg,orden_procesada);
        printf("\n");

        salida = salir(arvg);
        new_historial( *arvg,contador, historial); //se tiene que guardar el comando si no está bien escrito -> SÍ
        procesarEntrada(/**/);

    }

    return 0;
}







