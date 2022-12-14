/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#include "funciones.h"

//generales
bool procesarEntrada(tList *historial, tHistMem *bloques, tHistProc *procesos, char *envp[]);

void leerEntrada(char *entrada[], tList *historial);

void new_historial(char *comando[], tList *hist, int ntokens);

bool salir(char cadena[]);

//p0
void ayuda(tItemL comando);

void infosis();

void repetir_comando(tItemL entrada, tList *hist, tHistMem bloques, tHistProc procesos, char *envp[]);

void autores(tItemL comando);

void pillar_pid(tItemL comando);

void carpeta(tItemL comando);

void fecha(tItemL comando);

void hist(tItemL comando, tList *hist);

//p1
void status(tItemL comando);

void listar(tItemL comando);

void create(tItemL comando);

int delete(tItemL comando);//borra documentos o directorios vacios

int deleteTree(tItemL comando);//borra recursivamente documentos y directorios no vacios

//p2
void allocate(tItemL comando, tHistMem *bloques);

void deallocate(tItemL comando, tHistMem *bloques);

void recurse(tItemL comando);

void memory(tItemL comando, tHistMem *bloques);

int volcarmem(tItemL comando);

void memfill(tItemL comando);

void input_output(tItemL comando);

//p3
void listJobs(tHistProc procesos);

void deleteJobs(tItemL comando, tHistProc *procesos);

void cmd_fork();

void showenv(tItemL comando, char *envp[]);

void inputExecute(tItemL entrada);

void inputExecCreate(tItemL entrada, tHistProc *procesos);

int priority(tItemL comando);

int showvar(tItemL comando, char *envp[]);

int changevar(tItemL comando, char *envp[]);

void job(tItemL comando, tHistProc *procesos);


int main(int argc, char *arvg[], char *envp[]) {

    tList historial;
    createList(&historial);
    tHistMem bloques;
    createMem(&bloques);
    tHistProc procesos;
    createProcList(&procesos);
    bool salida = false;

    while (!salida) {
        leerEntrada(arvg, &historial);
        salida = procesarEntrada(&historial, &bloques, &procesos, envp);
    }

    deleteList(&historial);
    deleteHistMem(&bloques);
    deleteProcList(&procesos);
    return 0;
}


bool procesarEntrada(tList *historial, tHistMem *bloques, tHistProc *procesos, char *envp[]) {

    if (!isEmptyList(*historial)) {
        tItemL peticion = getItem(last(*historial), *historial);

        if (salir(peticion.comando)) return true;
        else {
            if (peticion.tokens == 0)
                deleteLast(last(*historial), historial);
            else if (strcmp(peticion.comando, "autores") == 0)
                autores(peticion); //p0
            else if (strcmp(peticion.comando, "comando") == 0)
                repetir_comando(peticion, historial, *bloques, *procesos, envp);
            else if (strcmp(peticion.comando, "pid") == 0)
                pillar_pid(peticion);
            else if (strcmp(peticion.comando, "carpeta") == 0)
                carpeta(peticion);
            else if (strcmp(peticion.comando, "fecha") == 0)
                fecha(peticion);
            else if (strcmp(peticion.comando, "infosis") == 0)
                infosis();
            else if (strcmp(peticion.comando, "ayuda") == 0)
                ayuda(peticion);
            else if (strcmp(peticion.comando, "hist") == 0)
                hist(peticion, historial);
            else if (strcmp(peticion.comando, "crear") == 0)
                create(peticion); //p1
            else if (strcmp(peticion.comando, "stat") == 0)
                status(peticion);
            else if (strcmp(peticion.comando, "list") == 0)
                listar(peticion);
            else if (strcmp(peticion.comando, "delete") == 0)
                delete(peticion);
            else if (strcmp(peticion.comando, "deltree") == 0)
                deleteTree(peticion);
            else if (strcmp(peticion.comando, "allocate") == 0)
                allocate(peticion, bloques); //p2
            else if (strcmp(peticion.comando, "deallocate") == 0)
                deallocate(peticion, bloques);
            else if (strcmp(peticion.comando, "memdump") == 0)
                volcarmem(peticion);
            else if (strcmp(peticion.comando, "memfill") == 0)
                memfill(peticion);
            else if (strcmp(peticion.comando, "memory") == 0)
                memory(peticion, bloques);
            else if (strcmp(peticion.comando, "recurse") == 0)
                recurse(peticion);
            else if (strcmp(peticion.comando, "i-o") == 0)
                input_output(peticion);
            else if (strcmp(peticion.comando, "priority") == 0)
                priority(peticion); //p3
            else if (strcmp(peticion.comando, "showvar") == 0)
                showvar(peticion, envp);
            else if (strcmp(peticion.comando, "changevar") == 0)
                changevar(peticion, envp);
            else if (strcmp(peticion.comando, "showenv") == 0)
                showenv(peticion, envp);
            else if (strcmp(peticion.comando, "fork") == 0)
                cmd_fork();
            else if (strcmp(peticion.comando, "listjobs") == 0)
                listJobs(*procesos);
            else if (strcmp(peticion.comando, "deljobs") == 0)
                deleteJobs(peticion, procesos);
            else if (strcmp(peticion.comando, "jobs") == 0)
                job(peticion, procesos);
            else if (strcmp(peticion.comando, "execute") == 0)
                inputExecute(peticion);
            else
                inputExecCreate(peticion, procesos);

            return false;
        }

    }
    return false;
}

void leerEntrada(char *entrada[], tList *historial) {
    char *orden_procesada[MAX_LENGHT_PATH];

    printf(">> ");
    fgets(*entrada, MAX_LENGHT_PATH, stdin);
    if (strcmp("\n", *entrada) != 0) {
        int ntokens = TrocearCadena(*entrada, orden_procesada);
        new_historial(&orden_procesada[0], historial, ntokens);
    } else {
        char *n = "vacia";
        new_historial(&n, historial, 0);
    }

}

void new_historial(char *comando[], tList *hist, int ntokens) {
    tItemL nuevo;
    nuevo.tokens = ntokens;
    strcpy(nuevo.comando, *comando);
    createEmptyTokensList(&nuevo.comandos);

    for (int i = 1; i < ntokens; i++) //empieza en 1 porque ya se copió el valor en 0
        if (!insertToken(comando[i], &nuevo.comandos)) printf("no se ha insertado el token %d\n", i);

    if (!insertElement(nuevo, hist)) printf("no se ha insertado el elemento\n");

}

bool salir(char cadena[]) {

    if (strcmp(cadena, "fin") == 0) return true;
    else if (strcmp(cadena, "salir") == 0) return true;
    else if (strcmp(cadena, "bye") == 0) return true;
    else return false;

}


//comandos
void ayuda(tItemL comando) {
    tItemT modo;
    getToken(0, comando.comandos, modo);

    if (comando.tokens == 1) {
        printf("'ayuda cmd' donde cmd es uno de los siguientes comandos:\n");
        printf("\tfin salir bye fecha pid autores hist comando carpeta infosis ayuda\n"
               "\tcrear delete deltree stat list\n"
               "\tallocate deallocate memdump memfill memory recurse\n"
               "\tlistjobs deljobs priority showvar changevar showenv fork jobs execute ****\n");
    } else if (strcmp(modo, "fin") == 0)
        printf("fin\tTermina la ejecucion del shell\n");
    else if (strcmp(modo, "salir") == 0)
        printf("salir\tTermina la ejecucion del shell\n");
    else if (strcmp(modo, "bye") == 0)
        printf("bye\tTermina la ejecucion del shell\n");
    else if (strcmp(modo, "autores") == 0)
        printf("autores [-n|-l]\tMuestra los nombres y logins de los autores\n");
    else if (strcmp(modo, "pid") == 0)
        printf("pid [-p]\tMuestra el pid del shell o de su proceso padre\n");
    else if (strcmp(modo, "carpeta") == 0)
        printf("carpeta [dir]\tCambia (o muestra) el directorio actual del shell\n");
    else if (strcmp(modo, "fecha") == 0)
        printf("fecha [-d|-h]\tMuestra la fecha y o la hora actual\n");
    else if (strcmp(modo, "hist") == 0)
        printf("hist [-c|-N]\tMuestra el historico de comandos, con -c lo borra\n");
    else if (strcmp(modo, "comando") == 0)
        printf("comando [-N]\tRepite el comando N (del historico)\n");
    else if (strcmp(modo, "infosis") == 0)
        printf("infosis \tMuestra informacion de la maquina donde corre el shell\n");
    else if (strcmp(modo, "crear") == 0)
        printf("crear [-f] [name]\tCrea un directorio o un fichero (-f)\n");
    else if (strcmp(modo, "delete") == 0)
        printf("delete [name1 name2 ..]\tBorra ficheros o directorios vacios\n");
    else if (strcmp(modo, "deltree") == 0)
        printf("deltree [name1 name2 ..]\tBorra ficheros o directorios no vacios recursivamente\n");
    else if (strcmp(modo, "stat") == 0) {
        printf("stat [-long][-link][-acc] name1 name2 ..\tlista ficheros;\n");
        printf("\t\t-long: listado largo\n");
        printf("\t\t-acc: acesstime\n");
        printf("\t\t-link: si es enlace simbolico, el path contenido\n");
    } else if (strcmp(modo, "list") == 0) {
        printf("list [-reca] [-recb] [-hid][-long][-link][-acc] n1 n2 ..\tlista contenidos de directorios\n");
        printf("\t\t-hid: incluye los ficheros ocultos\n");
        printf("\t\t-reca: recursivo (antes)\n");
        printf("\t\t-recb: recursivo (despues)\n");
        printf("\t\tresto parametros como stat\n");
    } else if (strcmp(modo, "allocate") == 0) {
        printf("allocate [-malloc|-shared|-createshared|-mmap]... Asigna un bloque de memoria\n");
        printf("\t-malloc tam: asigna un bloque malloc de tamano tam\n");
        printf("\t-createshared cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam\n");
        printf("\t-shared cl: asigna el bloque de memoria compartida (ya existente) de clave cl\n");
        printf("\t-mmap fich perm: mapea el fichero fich, perm son los permisos\n");
    } else if (strcmp(modo, "deallocate") == 0) {
        printf("deallocate [-malloc|-shared|-delkey|-mmap|addr]..\tDesasigna un bloque de memoria\n");
        printf("\t-malloc tam: desasigna el bloque malloc de tamano tam\n");
        printf("\t-shared cl: desasigna (desmapea) el bloque de memoria compartida de clave cl\n");
        printf("\t-delkey cl: elimina del sistema (sin desmapear) la clave de memoria cl\n");
        printf("\t-mmap fich: desmapea el fichero mapeado fich\n");
        printf("\taddr: desasigna el bloque de memoria en la direccion addr\n");
    } else if (strcmp(modo, "memdump") == 0)
        printf("memdump addr cont \tVuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr\n");
    else if (strcmp(modo, "memfill") == 0)
        printf("memfill addr cont byte \tLlena la memoria a partir de addr con byte\n");
    else if (strcmp(modo, "memory") == 0) {
        printf("memory [-blocks|-funcs|-vars|-all|-pmap] ..\tMuestra muestra detalles de la memoria del proceso\n");
        printf("\t\t-blocks: los bloques de memoria asignados\n");
        printf("\t\t-funcs: las direcciones de las funciones\n");
        printf("\t\t-vars: las direcciones de las variables\n");
        printf("\t\t-all: todo\n");
        printf("\t\t-pmap: muestra la salida del comando pmap(o similar)\n");
    } else if (strcmp(modo, "recurse") == 0)
        printf("recurse [n]\tInvoca a la funcion recursiva n veces\n");
    else if (strcmp(modo, "listjobs") == 0)
        printf("listjobs 	Lista los procesos en segundo plano\n");
    else if (strcmp(modo, "deljobs") == 0) {
        printf("deljobs [-term][-sig]	Elimina los procesos de la lista procesos en sp\n");
        printf("	-term: los terminados\n");
        printf("	-sig: los terminados por senal\n");
    } else if (strcmp(modo, "priority") == 0)
        printf("priority [pid] [valor] 	Muestra o cambia la prioridad del proceso pid a valor\n");
    else if (strcmp(modo, "showvar") == 0)
        printf("showvar var	Muestra el valor y las direcciones de la variable de entorno var\n");
    else if (strcmp(modo, "changevar") == 0) {
        printf("changevar [-a|-e|-p] var valor	Cambia el valor de una variable de entorno\n");
        printf("	-a: accede por el tercer arg de main\n");
        printf("	-e: accede mediante environ\n");
        printf("	-p: accede mediante putenv\n");
    } else if (strcmp(modo, "showenv") == 0) {
        printf("showenv [-environ|-addr] 	 Muestra el entorno del proceso\n");
        printf("	-environ: accede usando environ (en lugar del tercer arg de main)\n");
        printf("	-addr: muestra el valor y donde se almacenan environ y el 3er arg main\n");
    } else if (strcmp(modo, "fork") == 0)
        printf("fork 	El shell hace fork y queda en espera a que su hijo termine\n");
    else if (strcmp(modo, "jobs") == 0)
        printf("jobs 	Lista los procesos en segundo plano\n");
    else if (strcmp(modo, "execute") == 0) {
        printf("execute VAR1 VAR2 ..prog args....[@pri]	Ejecuta, sin crear proceso,prog con argumentos en un entorno que contiene solo las variables VAR1, VAR2...\n");
        printf("de todas formas, si quieres ejecutar un programa externo lo puedes hacer si lo introduces con *esta sintaxis* en vez de un comando\n");
    } else {
        printf("%s no encontrado\n", modo);
        printf("de todas formas, si quieres ejecutar un programa externo lo puedes hacer si lo introduces con *esta sintaxis* en vez de un comando\n");
    }
}

void infosis() {
    struct utsname informacion;
    uname(&informacion);
    if (errno == -1) {
        perror("no se ha conseguido la información del sistema");
        strerror(errno);
    }

    printf("%s (%s), OS: %s %s %s\n",
           informacion.nodename, informacion.machine, informacion.sysname, informacion.release, informacion.version);
}

void repetir_comando(tItemL entrada, tList *hist, tHistMem bloques, tHistProc procesos, char *envp[]) { //comando N

    tItemT cpos;
    getToken(0, entrada.comandos, cpos);
    int posicion = atoi(cpos);
    tPosL posrepeticion = findItem(posicion, *hist);
    tItemL repeticion = getItem(posrepeticion, *hist);

    tItemL nuevo;
    strcpy(nuevo.comando, repeticion.comando);
    nuevo.comandos = repeticion.comandos;
    nuevo.tokens = repeticion.tokens;

    if (!insertElement(nuevo, hist)) printf("no se ha insertado el elemento\n");
    procesarEntrada(hist, &bloques, &procesos, envp);

}

void autores(tItemL comando) {
    char nombre_noa[] = "Noa Rodriguez Pache", login_noa[] = "noa.rpache";
    char nombre_fatima[] = "Fátima Ansemil Barros", login_fatima[] = "fatima.ansemil";

    if (comando.tokens == 1) { //no se especifica nada
        printf("%s: %s\n", nombre_noa, login_noa);
        printf("%s: %s\n", nombre_fatima, login_fatima);

    } else {
        tItemT modo;
        getToken(0, comando.comandos, modo);
        if (strcmp("-l", modo) == 0) {
            printf("%s\n", login_noa);
            printf("%s\n", login_fatima);
        } else {
            if (strcmp(modo, "-n") == 0) {
                printf("%s\n", nombre_noa);
                printf("%s\n", nombre_fatima);
            } else {
                printf("%s no es un especificador válido\n", modo);
            }
        }
    }

}

void pillar_pid(tItemL comando) { //char *modo, int ntokens

    if (comando.tokens == 1) { //no se ha especificado -p
        printf("Pid de shell: %d\n", getpid());
    } else {
        tItemT modo;
        getToken(0, comando.comandos, modo);
        if (strcmp(modo, "-p") == 0) { //hay que sacar el proceso padre de la shell
            printf("Pid del padre del shell: %d\n", getppid());
        } else {
            printf("%s no es un especificador válido\n", modo);
        }

    }

}

void carpeta(tItemL comando) {

    if (comando.tokens == 1) { //no se han recibido argumentos adicionales
        getDir();
    } else { //se cambia de directorio
        tItemT modo;
        getToken(0, comando.comandos, modo);
        int error = chdir(modo);
        if (error == -1) {
            perror("no se ha podido cambiar de directorio");
            strerror(errno);
        }
    }

}

void fecha(tItemL comando) {

    int horas, minutos, segundos, dia, mes, anho;

    time_t now;//tipo de tiempo aritmetico
    time(&now);
    if (errno == -1) {
        perror("no se ha podido conseguir la fecha actual"); //'time()' devuelve la hora actual del sistema como un valor 'time_t'
        strerror(errno);
    }

    //localtime convierte un valor de 'time_t' a la hora del calendario

    struct tm *local = localtime(&now);

    horas = local->tm_hour; //aqui obtiene horas desde la medianoche
    minutos = local->tm_min; //obtiene los minutos
    segundos = local->tm_sec; //obtiene los segundos

    dia = local->tm_mday; //obtiene el dia del mes (1 al 31)
    mes = local->tm_mon + 1; //ontiene el mes del año del 0 al 11
    anho = local->tm_year + 1900; //obtiene el año desde 1900

    //ya tenemos hechas todas las variables

    if (comando.tokens == 1) {//tiene que imprimir la fecha y la hora

        printf("%02d:%02d:%02d \n", horas, minutos, segundos);
        printf("%02d/%02d/%d\n", dia, mes, anho);

    } else {
        tItemT modo;
        getToken(0, comando.comandos, modo);

        if (strcmp(modo, "-d") == 0) {//imprime solo la fecha actual
            printf("%02d/%02d/%d\n", dia, mes, anho);
        } else {
            if (strcmp(modo, "-h") == 0) {//imprime solo la hora actual
                printf("%02d:%02d:%02d \n", horas, minutos, segundos);
            } else {
                printf("%s no es un especificador válido\n", modo);
            }
        }
    }

}

void hist(tItemL comando, tList *hist) {

    if (comando.tokens == 1 && !isEmptyList(*hist)) { //imprimir la lista
        tPosL LastNode = primero(*hist);
        while (LastNode != LNULL) {
            tItemL objeto = getItem(LastNode, *hist);
            printComand(objeto, true, true);
            LastNode = LastNode->next;
        }
    } else {
        tItemT modo;
        getToken(0, comando.comandos, modo);

        if (strcmp(modo, "-c") == 0) {
            deleteList(hist);
            createList(hist);
        } else {
            int N = int_convert(modo);
            tPosL LastNode = primero(*hist);
            int i = 0;
            do {
                tItemL objeto = getItem(LastNode, *hist);
                printComand(objeto, true, true);
                LastNode = next(LastNode, *hist);
                i++;
            } while (i <= N - 1 && LastNode != NULL);
        }
    }
}

void status(tItemL comando) { //y si pasamos directorios y archivos a la vez??
    if (comando.tokens == 1) {
        getDir();
    } else {
        modo modo;
        modo.largo = false;
        modo.acc = false;
        modo.link = false;
        modo.reca = false;
        modo.recb = false;
        modo.hid = false;
        modo.listar = false;
        int controlador = opciones(comando, &modo);
        //controlador cuenta el número de -long, -link, -acc que hay; además de que es la posición del 1er path en el array de tokens

        if (comando.tokens - 1 ==
            controlador) { //se ha terminado el bucle sin ningún path/file posible -> imprimir ruta actual
            getDir();
        } else {
            for (int j = controlador - 1;
                 j <= comando.comandos.lastPos - 1; j++) { //recorrer todos los archivos que han entrado por terminal
                char path[MAX_LENGHT_PATH];
                getToken(j + 1, comando.comandos, path);
                char ruta[MAX_LENGHT_PATH];
                if (realpath(path, ruta) == NULL) {
                    perror("error al obtener el path");
                    strerror(errno);
                } else {
                    if (printInfo(ruta, path, &modo) == -1) strerror(errno);
                }
            }
        }
    }
}

void listar(tItemL comando) {
    if (comando.tokens == 1) {
        getDir();
    } else {
        modo modo;
        modo.largo = false;
        modo.acc = false;
        modo.link = false;
        modo.reca = false;
        modo.recb = false;
        modo.hid = false;
        modo.listar = true;
        int controlador = opciones(comando, &modo);

        //impresión de la petición
        if (comando.tokens - 1 == controlador) {
            getDir(); //se ha terminado el bucle sin ningún path/file posible -> imprimir ruta actual
        } else {
            char path[MAX_LENGHT_PATH];
            getToken(controlador, comando.comandos, path);

            if (!modo.reca && !modo.recb) {
                if (ListContent(path, &modo) == -1) strerror(errno);
            } else {
                if (modo.reca) {
                    if (ListReca(path, &modo) == -1) strerror(errno);
                }
                if (modo.recb) {
                    if (ListRecb(path, &modo) == -1) strerror(errno);
                }
            }

        }

    }
}

void create(tItemL comando) {

    if (comando.tokens == 1) {
        getDir();
    }

    if (comando.tokens != 0) {
        char path[MAX_LENGHT_PATH];
        char error[MAX_LENGHT_PATH] = "Error, imposible de crear";

        getcwd(path, sizeof(path));
        strcat(path, "/");

        tItemT modo;
        getToken(0, comando.comandos, modo);

        if (strcmp("-f", modo) == 0) {//crear un archivo

            getToken(1, comando.comandos, modo);

            char *name_folder = modo;

            if (creat(strcat(path, name_folder), 0666) == -1) {//creamos el documento y le pasamos el permiso
                perror(error);//salta error si no se puede crear
            }

        } else {

            char *name_folder = modo;

            if (mkdir(strcat(path, name_folder), 0755) == -1) {//creamos el directorio y le pasamos el permiso
                perror(error);//en el caso de que no se pueda crear salta el error
            }

        }


    }
}

int delete(tItemL comando) {//borra documentos o directorios vacios

    char error[MAX_LENGHT_PATH] = "Error, imposible de borrar";

    if (comando.tokens == 1) getDir();
    else {
        if (comando.tokens != 0) {
            int controlador1, controlador2, auxiliar, auxiliar1;
            tItemT aux;
            for (int i = 0; i < comando.tokens - 1; i++) {//aqui recorremos todos los tokens ns si hay que poner -2

                getToken(i, comando.comandos, aux);

                controlador1 = isDirectory(aux);
                controlador2 = isDirEmpty(aux);

                if (controlador1 != 0 && controlador2 == 1) {//si es directorio y esta vacío lo borra

                    auxiliar = remove(aux);

                    if (auxiliar != 0) {//si no ha sido posible borrarlo salta error

                        perror(error);

                    }

                } else {//sino es documento

                    auxiliar1 = remove(aux);//esto es para eliminar files

                    if (auxiliar1 != 0) {
                        perror(error);
                    }
                }
            }
        }
    }
    return 0;
}

int deleteTree(tItemL comando) { //borra recursivamente documentos y directorios no vacios

    int controlador1, controlador2, controlador3, auxiliar;
    char error[MAX_LENGHT_PATH] = "no se ha podido borrar";

    if (comando.tokens == 1) getDir();
    else {
        if (comando.tokens != 0) {

            tItemT aux;

            for (int i = 0; i < comando.tokens - 1; i++) {//ns si hay que poner el -2

                getToken(i, comando.comandos, aux);

                controlador1 = isDirectory(aux);
                controlador2 = isDirEmpty(aux);

                if (controlador1 != 0 && controlador2 == 0) { //si es directorio y está vacío

                    borrar_dir(aux); //aquí borra el contenido recursivamente
                    controlador3 = isDirEmpty(aux);

                    if (controlador3 != 1) {
                        auxiliar = remove(aux);

                        if (auxiliar != 0) {//si no ha sido posible borrarlo salta error
                            perror(error);
                        }
                    }

                } else {

                    unlink(aux); //si no es directorio lo borra con unlink

                }
            }
        }
    }
    return 0;
}


void allocate(tItemL comando, tHistMem *bloques) {
    if (comando.tokens == 1) {
        ListarBloques(*bloques, 4);
    } else {
        tItemT modo;
        getToken(0, comando.comandos, modo);
        tItemM datos;

        if (strcmp(modo, "-malloc") == 0) {

            if (comando.tokens == 2) {
                ListarBloques(*bloques, 0);
                return;
            } else {
                if (asignarMalloc(comando, &datos) == -1) {
                    printf("se ha producido un error en -malloc\n");
                    strerror(errno);
                    return;
                }
            }

        } else if (strcmp(modo, "-shared") == 0) {

            if (comando.tokens == 2) {
                ListarBloques(*bloques, 1);
                return;
            } else {
                if (asignarCompartida(comando, &datos, false) == -1) return;
            }

        } else if (strcmp(modo, "-createshared") == 0) {

            if (comando.tokens == 2) {
                ListarBloques(*bloques, 1);
                return;
            } else {
                if (asignarCompartida(comando, &datos, true) == -1) return;
            }

        } else if (strcmp(modo, "-mmap") == 0) {

            if (comando.tokens == 2) {
                ListarBloques(*bloques, 2);
                return;
            } else {
                if (asignarMap(comando, &datos) == -1) return;
            }

        } else {
            printf("%s no es una opción válida para este comando\n", modo);
            return;
        }

        if (!insertMemoryBlock(datos, bloques)) printf("no se ha podido insertar el bloque de memoria en la lista\n");

    }
}

void deallocate(tItemL comando, tHistMem *bloques) {
    if (comando.tokens == 1) {
        ListarBloques(*bloques, 4);
    } else {
        tItemT modo;
        getToken(0, comando.comandos, modo);

        if (strcmp(modo, "-malloc") == 0) {

            if (comando.tokens == 2) { //no se han recibido más argumentos
                ListarBloques(*bloques, 0);
                return;
            } else {
                tItemT tam;
                getToken(1, comando.comandos, tam);
                size_t tamano = (size_t) strtoul(tam, NULL, 10);

                desasignarMalloc(tamano, bloques);
            }

        } else if (strcmp(modo, "-shared") == 0) {

            if (comando.tokens == 2) { //no se han recibido más argumentos
                ListarBloques(*bloques, 1);
                return;
            } else {
                tItemT key;
                getToken(1, comando.comandos, key);
                key_t clave = (key_t) strtoul(key, NULL, 10);
                tPosM posicion = findBlockShared(*bloques, clave);
                deleteMemBlock(posicion, bloques);
            }

        } else if (strcmp(modo, "-delkey") == 0) {
            //printf("se elimina la clave de un posible uso");

            tItemT key;
            getToken(1, comando.comandos, key);
            key_t clave = (key_t) strtoul(key, NULL, 10);

            desasignarClave(clave, bloques);

        } else if (strcmp(modo, "-mmap") == 0) {

            if (comando.tokens == 2) { //no se han recibido más argumentos
                ListarBloques(*bloques, 2);
                return;
            } else {
                tItemT nombre;
                getToken(1, comando.comandos, nombre);
                desasignarMapped(nombre, bloques);
            }

        } else {
            desasignarDireccion(comando, bloques);
        }
    }
}

void recurse(tItemL comando) {
    if (comando.tokens > 1) {
        tItemT aux;
        getToken(0, comando.comandos, aux);
        int veces = atoi(aux);//comprobar que es un número
        Recursiva(veces);
    }
}


//variables para funcion memoria
int g1 = 0, g2 = 0, g3 = 0;

void memory(tItemL comando, tHistMem *bloques) {

    if (comando.tokens > 1) {

        tItemT modo;//asignamos la variable tItemT

        for (int i = 0; i < comando.tokens - 1; i++) {

            getToken(i, comando.comandos, modo); //recogemos el modo de cada comando

            //lo vamos comparando

            if (strcmp(modo, "-vars") == 0) {//aquí devuelve las variables

                auto int x = 0, y = 0, z = 0;
                static int a = 0, b = 0, c = 0;

                printf("automatic variables:\t%p, %p, %p\n", &x, &y, &z);
                printf("static variables:\t%p, %p, %p\n", &a, &b, &c);
                printf("global variables:\t%p, %p, %p\n", &g1, &g2, &g3);

            }
            if (strcmp(modo, "-funcs") == 0) {//aquí las funciones

                printf("program functions:\t%p, %p, %p\n", autores, pillar_pid, infosis);
                printf("library functions :\t%p, %p, %p\n", malloc, printf, strcmp);

            }
            if (strcmp(modo, "-blocks") == 0) {

                ListBLocks(*bloques);

            }
            if (strcmp(modo, "-all") == 0) {//tiene que hacer lo equivalente a blocks func y vars

                auto int x = 0, y = 0, z = 0;
                static int a = 0, b = 0, c = 0;

                printf("automatic variables:\t%p, %p, %p\n", &x, &y, &z);
                printf("static variables:\t%p, %p, %p\n", &a, &b, &c);
                printf("global variables:\t%p, %p, %p\n", &g1, &g2, &g3);

                printf("program functions:\t%p, %p, %p\n", autores, pillar_pid, infosis);
                printf("library functions :\t%p, %p, %p\n", malloc, printf, strcmp);

                ListBLocks(*bloques);


            } else if (strcmp(modo, "-pmap") == 0) {

                dopmap();
            }

        }

    } else {

        //sino se dan argumentos se hace la operacion equivalente a -all
        auto int x = 0, y = 0, z = 0;
        static int a = 0, b = 0, c = 0;

        printf("automatic variables:\t%p, %p, %p\n", &x, &y, &z);
        printf("static variables:\t%p, %p, %p\n", &a, &b, &c);
        printf("global variables:\t%p, %p, %p\n", &g1, &g2, &g3);

        printf("program functions:\t%p, %p, %p\n", autores, pillar_pid, infosis);
        printf("library functions :\t%p, %p, %p\n", malloc, printf, strcmp);

        deallocate(comando, bloques);
    }

}


int volcarmem(tItemL comando) {

    if (comando.tokens > 1) {
        int n = 25;
        char *p;
        tItemT cont, dir;

        getToken(0, comando.comandos, dir);
        long addr = strtoul(dir, &p, 16);

        if (comando.tokens == 3) { //se ha indicado un contenido específico
            getToken(1, comando.comandos, cont);
            if (isNumber(cont)) n = atoi(cont);
        }

        for (int i = 0; i < n; i += minimo(n - i, 25)) {
            long aux = addr;
            for (int j = 0; j < minimo(n - i, 25); j++) {
                printf(" %c ", (*(char *) aux == '\n') ? ' ' : *(char *) aux);
                aux++;
            }
            printf("\n");
            for (int j = 0; j < minimo(n - i, 25); j++) {
                printf("%02X ", *(char *) addr);
                addr++;
            }
            printf("\n");
        }

    }
    return 0;
}


void memfill(tItemL comando) {

    if (comando.tokens > 1) { //se han introducido datos
        int cont = 128; //valores predeterminados que cambiaremos si se introducen datos
        int c = 65;
        char *dir;
        tItemT address, content, byte;

        getToken(0, comando.comandos, address);
        long addr = strtoul(address, &dir, 16);

        if (comando.tokens > 2) { //se ha introducido un contenido

            getToken(1, comando.comandos, content);

            if (isNumber(content)) { //solo continuamos si el segundo elemento es un número
                cont = atoi(content);

                if (comando.tokens > 3) { //se han introducido los bytes

                    getToken(2, comando.comandos, byte);

                    if (isNumber(byte)) c = atoi(byte);
                    else c = strtoul(byte, &dir, 16);

                }
            }

        }

        //ejecutamos memfill en sí misma
        for (int i = 0; i < cont; i++) {
            *(int *) addr = c;
            addr++;
        }

    }
}

void input_output(tItemL comando) {
    if (comando.tokens == 1) printf("uso: e-s [read|write] ......\n");
    else {

        if (comando.tokens < 5) {
            printf("faltan parámetros\n");
            return;
        }

        modo_IO opciones;
        opciones.write = false;
        opciones.read = false;
        opciones.overwrite = false;
        modos_IO(comando, &opciones);
        tItemT fich, dir, tam;
        ssize_t n;
        void *addr;
        size_t cont;

        //se diferencia escoger o no -o por dónde estén colocados los tokens
        if (opciones.read || (opciones.write && !opciones.overwrite)) { // i-o -read fich addr cont

            getToken(1, comando.comandos, fich); //nombre del fichero
            getToken(2, comando.comandos, dir);
            addr = (void *) strtoul(dir, NULL, 16); //dirección del fichero

            if (opciones.read && comando.tokens == 4) {
                cont = -1; //se lee entero
            } else {
                getToken(3, comando.comandos, tam); //número de bytes a leer/escribir
                cont = (size_t) strtoul(tam, NULL, 10);
            }

            if (opciones.read) { //si se ha elegido -o también ya salta error al convertir los parámetros

                if ((n = LeerFichero(fich, addr, cont)) == -1) {
                    perror("Imposible leer fichero");
                } else {
                    printf("leidos %lld bytes de %s en %p\n", (long long) cont, fich, addr);
                }

            } else {

                if ((n = EscribirFichero(fich, addr, cont, 0)) == -1) {
                    perror("error al escribir en el fichero");
                    strerror(errno);
                } else {
                    printf("escritos %lld bytes de %s en %p\n", (long long) n, fich, addr);
                }

            }

        } else {

            if (opciones.overwrite) { //i-o -write -o fich addr cont

                getToken(2, comando.comandos, fich); //nombre del fichero
                getToken(3, comando.comandos, dir);
                addr = (void *) strtoul(dir, NULL, 16); //dirección del fichero
                getToken(4, comando.comandos, tam); //número de bytes a leer
                cont = (size_t) strtoul(tam, NULL, 10);

                if ((n = EscribirFichero(fich, addr, cont, 1)) == -1) {
                    perror("error al leer en el fichero");
                    strerror(errno);
                } else {
                    printf("sobreescritos %lld bytes de %s en %p\n", (long long) n, fich, addr);
                }

            } else {
                printf("has introducido mal alguno de los argumentos, probablemente te has olvidado de un '-'\n");
            }

        }

    }
}

//priority

int priority(tItemL comando) {

    int priority, pid;

    tItemT modo;
    tItemT prioridad;

    getToken(0, comando.comandos, modo);

    if (comando.tokens != 0 && isNumber(modo)) {
        pid = atoi(modo);

        if (comando.tokens == 1) {
            //conseguimos la prioridad del proceso

            priority = getpriority(PRIO_PROCESS, pid);

        } else {
            getToken(1, comando.comandos, prioridad);
            priority = atoi(prioridad);
        }

    } else {
        //sino cambiamos la prioridad del proceso
        priority = atoi(prioridad);
        //setpriority(PRIO_PROCESS, pid, priority);
    }
    printf("El proceso %d tiene prioridad %d\n", pid, priority);
    return 0;

}

//funcion mostrarvariable

int showvar(tItemL comando, char *envp[]) {
    tItemT variable;
    int pos;
    getToken(0, comando.comandos, variable);
    if (comando.tokens != 0) {
        if ((pos = BuscarVariable(variable, environ)) == -1) {
            //si devuelve -1 imposible encontrar la variable, salta el error

            perror("es imposible encontrar la variable");
            return 0;
        }

        printf("Con arg3 main %s(%p) @%p\n", environ[pos], environ[pos], envp[pos]);
        printf("Con environ $s(%p) @%p\n", getenv(variable), environ[pos]);
    } else showenv(comando, envp);
    return 0;
}


int changevar(tItemL comando, char *envp[]) {

    char *aux = malloc(MAX_LENGHT_PATH);
    tItemT Comando;
    tItemT variable;
    tItemT valor;
    getToken(0, comando.comandos, Comando);

    if (comando.tokens != 0) {
        if (comando.tokens == 3) {

            if (strcmp(Comando, "-a") == 0) {
                getToken(1, comando.comandos, variable);
                getToken(2, comando.comandos, valor);
                CambiarVariable(variable, valor, envp);

            } else if (strcmp(Comando, "-e") == 0) {
                CambiarVariable(variable, valor, environ);
            } else if (strcmp(Comando, "-p") == 0) {

                strcpy(aux, variable);
                strcat(aux, "="); //concateno con el simbolo =
                strcat(aux, valor); //le incluyo = valor
                putenv(aux);//añade o cambia el valor de variables de entorno o ambiente.
                // El argumento cadena es de la forma nombre=valor
            }
        }
    }

    return 0;
}


void listJobs(tHistProc procesos) {

    for (tPosP i = primerProc(procesos); i != PNULL; i = nextProc(i)) {
        procInfo(getProc(i));
    }

}

void deleteJobs(tItemL comando, tHistProc *procesos) {
    if (comando.tokens == 1) {
        //se borran todos -> efectivamente
        deleteProcList(procesos);
        createProcList(procesos);
    } else {
        tItemT modo;
        getToken(1, comando.comandos, modo);

        if (strcmp("-term", modo) == 0) {

            for (tPosP i = primerProc(*procesos); i != PNULL; i = nextProc(i)) {
                if (getProc(i).estado == finished || WIFEXITED(getProc(i).info)) deleteProc(i, procesos);
            }
        } else if (strcmp("-sig", modo) == 0) {

            for (tPosP i = primerProc(*procesos); i != PNULL; i = nextProc(i)) {
                if (getProc(i).estado == signaled || WIFSIGNALED(getProc(i).info)) deleteProc(i, procesos);
            }
        } else {
            perror("no has introducido un modo correcto");
        }
    }
}

void cmd_fork() { //¿¿cómo funciona la lista aquí???
    pid_t pid;

    if ((pid = fork()) == 0) { //ha habido éxito y estás situado en el proceso hijo
        printf("ejecutando proceso %d\n", getpid());
    } else if (pid != -1) waitpid(pid, NULL, 0);
    else strerror(errno);

}

void showenv(tItemL comando, char *envp[]) {
    if (comando.tokens == 1) { //accede por el 3er argumento de main
        for (int i = 0; envp[i] != NULL; i++) {
            printf("%p -> main arg3[%d] = %p %s\n", &envp[i], i, envp[i], envp[i]);
        }

    } else {
        tItemT modo;
        getToken(0, comando.comandos, modo);

        if (strcmp(modo, "-environ") == 0) { //recorres el array de environ

            for (int i = 0; environ[i] != NULL; i++) {
                printf("%p -> environ[%d] = %p %s\n", &environ[i], i, environ[i], environ[i]);
            }

        } else if (strcmp(modo, "-addr") == 0) { //muestras las direcciones de memoria

            printf("environ:   %p (almacenado en %p)\n", &environ[0], &environ);
            printf("main arg3: %p (almacenado en %p)\n", &envp[0], &envp);

        } else {

            printf("no has introducido un modo válido, puede que te hayas olvidado del '-'\n");

        }

    }
}

void inputExecCreate(tItemL entrada, tHistProc *procesos) { //este es el que sale al no meter comandos

    tItemP aux;
    int prioridad = 0; //valor neutral predeterminado
    bool env = false, plano2 = false;

    if (entrada.tokens == 1) {
        if ((aux.pid = execute(entrada.comando, NULL, NULL, prioridad, plano2, env) == -1))
            return;
    } else {

        int salir, envCounter = 1, argCounter = 1;
        char prog[MAX_LENGHT_PATH];
        tPosT i = -2;
        tItemT auxprog;

        if ((salir = BuscarVariable(entrada.comando, environ)) != -1) {

            i = firstToken(entrada.comandos);

            while (salir != -1 && i != TNULL) {
                if ((salir = BuscarVariable(entrada.comandos.data[i], environ)) != -1) {
                    envCounter++;
                    env = true;
                    i = nextToken(i, entrada.comandos); //si no se ha encontrado una VAR no se avanza
                }
            }
        }

        if (i == -2) {
            strcpy(prog, entrada.comando);
            i = firstToken(entrada.comandos);
        } else if (i == TNULL) {
            printf("solo se han introducido variables de entorno\n");
            return;
        } else { //se ha introducido alguna, así que hay que sacarla de los tokens
            getToken(previousToken(i), entrada.comandos, auxprog);
            strcpy(prog, auxprog);
        }


        while (i != TNULL) {

            if (strncmp("@", entrada.comandos.data[i], 1) == 0 && prioridad == 0) {
                prioridad = convertPriority(entrada.comandos.data[i]);
            } else if (strcmp("&", entrada.comandos.data[i]) == 0 && plano2 == false) {
                plano2 = true;
                i = entrada.comandos.lastPos;
            } else {
                argCounter++;
            }
            i = nextToken(i, entrada.comandos);
        }
        argCounter--;

        //ahora a copiar los arrays
        char *argv[argCounter], *envp[envCounter];

        if (envCounter != 0) {
            envp[0] = entrada.comando;
            int j;
            for (j = 1; j < envCounter; j++) {
                envp[j] = entrada.comandos.data[j -
                                                1]; //no vale el mismo contador porque se tiene en cuenta el comando ppal
                //printf("env[%d] %s = %s\n",j,env[j], entrada.comandos.data[j-1]);
            }
            envp[j + 1] = NULL;
        }

        if (argCounter != 0) {
            int j, k = envCounter -
                       1; //debería tener un -1 por el entrada.comando y un +1 por el programa en sí, el otro -1 es porque arrays
            for (j = 0; j < argCounter; j++) {
                argv[j] = entrada.comandos.data[k];
                //printf("argv[%d] %s = %s\n k = %d\n",j,argv[j], entrada.comandos.data[k], k);
                k++;
            }
            argv[j + 1] = NULL;
        }

        if ((aux.pid = execute(prog, argv, envp, prioridad, plano2, env)) == -1)
            return;
    }

    //lo quito para probar la lista con procesos en 1er plano//if(!plano2) return; //si no se ha ejecutado en 2o plano, no se añade a la lista

    aux.pid = -50;
    time_t now;
    time(&now);
    if (errno == -1) {
        perror("error al obtener el tiempo");
        return;
    }
    struct tm *local = localtime(&now);
    strftime(aux.fecha, TAM_FECHA, "%Y/%m/%d %H:%M:%S", local);
    aux.estado = active;
    aux.info = 0;
    aux.comando = entrada;

    if (!insertProc(aux, procesos)) perror("no se ha podido introducir el proceso en la lista");

}

void inputExecute(tItemL entrada) {

    if (entrada.tokens == 1) {
        printf("execute VAR1 VAR2... prog arg1 arg2... [@pri]\n");
        return;
    }

    int salir = 0, prioridad = 0;
    char *envp[MAX_TOKENS] = {}, prog[MAX_LENGHT_PATH], *argv[MAX_TOKENS] = {};
    tItemT auxprog;
    tPosT i = firstToken(entrada.comandos);

    while (salir == -1 && i != TNULL) {
        if ((salir = BuscarVariable(entrada.comandos.data[i], environ)) != -1) {
            envp[i] = entrada.comandos.data[i];
        }

        i = nextToken(i, entrada.comandos);

    }

    if (i == TNULL) {
        perror("solo se han introducido variables de entorno");
        return;
    }

    getToken(i, entrada.comandos, auxprog);
    strcpy(prog, auxprog); //este es el nombre del programa

    while (i != TNULL) {

        if (strncmp("@", entrada.comandos.data[i], 1) == 0 && prioridad == 0) {
            prioridad = convertPriority(entrada.comandos.data[i]);
        } else { //copiar el array de argumentos
            argv[i] = entrada.comandos.data[i];
        }

        i = nextToken(i, entrada.comandos);
    }


    if (prioridad != 0) { //"encontrar forma de convetirla"
        if (setpriority(PRIO_PROCESS, getpid(), prioridad) == -1) {
            strerror(errno);
            return;
        }
    }

    OurExecvpe(prog, argv, envp);

}

void job(tItemL comando, tHistProc *procesos) {

    if (comando.tokens == 1) {
        listJobs(*procesos);
        return;
    }

    if (comando.tokens == 2) { //no sabemos si se ha introducido bien la entrada
        tItemT modo;
        getToken(0, comando.comandos, modo);

        if (strcmp(modo, "-fg") == 0) listJobs(*procesos);
        else {
            int pid = atoi(modo);
            tPosP i;
            if ((i = findProc(pid, *procesos)) == PNULL) {
                printf("no hay ningún proceso con ese pid en la lista\n");
                return;
            }
            procInfo(getProc(i));
        }
        return;
    }

    if (comando.tokens == 3) { //se quiere ejecutar un pid en primer plano
        tItemT modo;
        getToken(0, comando.comandos, modo);
        if (strcmp(modo, "-fg") != 0) {
            printf("job [-fg] pid\tMuestra informacion del proceso pid.\n"
                   "\t\t-fg: lo pasa a primer plano\n");
        }

        tItemT valor;
        getToken(1, comando.comandos, valor);
        int pid = atoi(valor);
        tPosP i;
        if ((i = findProc(pid, *procesos) )== PNULL) {
            printf("no hay ningún proceso en segundo plano con ese pid\n");
            return;
        }

        waitpid(pid, NULL, 0);

        if (getProc(i).estado == active)
            printf("proceso %d ejecutado con normalidad. Se devuelve el valor %d\n", getProc(i).pid, getProc(i).info);
        else
            printf("proceso %d ya esta finalizado ", getProc(i).pid);

        deleteProc(i, procesos);

        return;
    }

}

