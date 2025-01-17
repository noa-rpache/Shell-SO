/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#include "funciones.h"

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


//auxiliares
int TrocearCadena(char *cadena, char *trozos[]) {
    int i = 1;

    if ((trozos[0] = strtok(cadena, " \n\t")) == NULL)
        return 0;
    else {
        while ((trozos[i] = strtok(NULL, " \n\t")) != NULL)
            i++;
        return i;
    }

}

int int_convert(tItemT cadena) {
    int convertido = atoi(cadena);
    return convertido * (-1);
}

void printComand(tItemL impresion, bool puesto, bool salto) {
    if (puesto) printf("%d: ", impresion.puesto);

    printf("%s", impresion.comando);
    if (impresion.tokens > 1) { //si hay algún token más que el comando ppal
        for (int i = 0; i < impresion.tokens - 1; i++) { //ntokens -1 == nº de especificadores
            printf(" %s", impresion.comandos.data[i]);
        }
    }

    if (salto) printf("\n");
}

char LetraTF(mode_t m) {//devuelve el tipo de un fichero
    switch (m & S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK:
            return 's'; /*socket */
        case S_IFLNK:
            return 'l'; /*symbolic link*/
        case S_IFREG:
            return '-'; /* fichero normal*/
        case S_IFBLK:
            return 'b'; /*block device*/
        case S_IFDIR:
            return 'd'; /*directorio */
        case S_IFCHR:
            return 'c'; /*char device*/
        case S_IFIFO:
            return 'p'; /*pipe*/
        default:
            return '?'; /*desconocido, no debería aparecer*/
    }
}

void getDir() {
    char directorio[MAX_LENGHT_PATH];
    getcwd(directorio, sizeof(directorio));
    if (errno == -1) { //se ha dado un error
        perror("no se ha podido conseguir la ruta actual");
        strerror(errno);
    } else {
        printf("%s\n", directorio);
    }
}

char *ConvierteModo2(mode_t m) {
    static char permisos[12];
    strcpy(permisos, "---------- ");

    permisos[0] = LetraTF(m);
    if (m & S_IRUSR) permisos[1] = 'r';    /*propietario*/
    if (m & S_IWUSR) permisos[2] = 'w';
    if (m & S_IXUSR) permisos[3] = 'x';
    if (m & S_IRGRP) permisos[4] = 'r';    /*grupo*/
    if (m & S_IWGRP) permisos[5] = 'w';
    if (m & S_IXGRP) permisos[6] = 'x';
    if (m & S_IROTH) permisos[7] = 'r';    /*resto*/
    if (m & S_IWOTH) permisos[8] = 'w';
    if (m & S_IXOTH) permisos[9] = 'x';
    if (m & S_ISUID) permisos[3] = 's';    /*setuid, setgid y stickybit*/
    if (m & S_ISGID) permisos[6] = 's';
    if (m & S_ISVTX) permisos[9] = 't';

    return permisos;
}

int opciones(tItemL entrada, modo *opciones) {
    int controlador = 0;

    for (int i = 0; i <= entrada.tokens - 2; i++) { //tokens es el total de tokens, incluido el ppal
        tItemT aux;
        getToken(i, entrada.comandos, aux);

        if (strcmp("-long", aux) == 0) {
            (*opciones).largo = true;
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

int printInfo(char rutaReal[MAX_LENGHT_PATH], char enlazada[MAX_LENGHT_PATH], const modo *opciones) {
    //la ruta enlazada se pasa siempre, la rutaReal puede ser NULL en un directorio
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
        if (strftime(tiempo, 100, format, time) == 0) {
            printf("el string no cabe en el tamaño proporcionado\n");
            return 0;
        }

        printf("%s ", tiempo);
    }

    if (opciones->largo) {
        struct group *grupinho = getgrgid(contenido.st_gid);
        if (grupinho == NULL) {
            perror("error al acceder al nombre del grupo");
            return -1;
        }

        struct passwd *user = getpwuid(contenido.st_uid);
        if (user == NULL) {
            perror("error al acceder al nombre del propietario");
            return -1;
        }

        printf("%5ld", (long) contenido.st_nlink);
        printf(" (%10ld)", (long) contenido.st_ino);
        printf("%7s", grupinho->gr_name);
        printf("%7s", user->pw_name);
        printf("%3c", LetraTF(contenido.st_mode));
        printf("%s", ConvierteModo2(contenido.st_mode));

    }

    printf("%10ld ", (long) contenido.st_size);

    if (opciones->listar == true) {
        printf("%s", rutaReal);

        if (opciones->largo && (opciones->link && LetraTF(contenido.st_mode) == 'l')) {
            //imprimir la ruta real a la que apunta el enlace
            char origen[MAX_LENGHT_PATH];
            realpath(enlazada, origen);
            printf(" -> %s", origen);
        }

    } else {
        printf("%s", enlazada); //se imprime siempre

        if (opciones->largo &&
            (opciones->link && LetraTF(contenido.st_mode) == 'l')) { //imprimir la ruta real a la que apunta
            printf(" -> %s", rutaReal); //con -link siempre se imprime LA RUTA real
        }

    }
    printf("\n");

    return 0;
}

int ListContent(char path[MAX_LENGHT_PATH], const modo *opciones) {
    //pasar el path requerido con las opciones pedidas y mostrarlo
    printf("************%s\n", path);

    DIR *directory_stream = opendir(path);
    if (directory_stream == NULL) {
        perror("error en opendir");
        return -1;
    }

    struct dirent *directorio;
    errno = 0;

    while ((directorio = readdir(directory_stream)) != NULL) {
        errno = 0;
        int tam = 2 * MAX_LENGHT_PATH;
        char rutaDir[tam];
        sprintf(rutaDir, "%s/%s", path, (*directorio).d_name);

        if (opciones->hid) {
            printInfo((*directorio).d_name, rutaDir, opciones); //enseñar todos los ficheros
        } else {
            if ((*directorio).d_name[0] != '.') {
                if (printInfo((*directorio).d_name, rutaDir, opciones) == -1) return -1;
            }

        }

        long sig = telldir(directory_stream);
        if (sig == -1) {
            perror("error en telldir");
            return -1;
        }
        seekdir(directory_stream, sig);
    }

    if (directorio == NULL && errno != 0) {
        printf("errno: %d\n", errno);
        perror("error en readdir list");
        return -1;
    }

    if (closedir(directory_stream) == -1) {
        perror("error al cerrar el directorio");
        return -1;
    }

    return 0;
}

int ListReca(char path[MAX_LENGHT_PATH], const modo *opciones) {
    //pasar el path requerido con las opciones pedidas y mostrarlo
    errno = 0;
    printf("************%s\n", path);

    tList DirRecord;
    if (!createList(&DirRecord)) {
        perror("no hay espacio para la recursividad");
        return -1;
    }

    DIR *directory_stream = opendir(path);
    if (directory_stream == NULL) return -1;

    struct dirent *directorio;

    while ((directorio = readdir(directory_stream)) != NULL) { //incluir detectar errores y no mostrar hidd
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
                    if (!insertElement(nuevo, &DirRecord)) {
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

int ListRecb(char path[MAX_LENGHT_PATH], const modo *opciones) {
    //pasar el path requerido con las opciones pedidas y mostrarlo
    errno = 0;

    DIR *directory_stream = opendir(path);
    if (directory_stream == NULL) return -1;

    struct dirent *directorio;

    while ((directorio = readdir(directory_stream)) != NULL) {
        int tam = 2 * MAX_LENGHT_PATH;
        char rutaDir[tam];
        sprintf(rutaDir, "%s/%s", path, (*directorio).d_name);

        if (opciones->hid) {
            if (isDirectory(rutaDir) == 1) {
                if (strcmp((*directorio).d_name, ".") != 0 && strcmp((*directorio).d_name, "..") != 0)
                    ListRecb(rutaDir,
                             opciones); //solo llama a la recursividad si no es ni . ni .., que si no nos hacemos un lío
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


    if (closedir(directory_stream) == -1) return -1;

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
    char aux[MAX_LENGHT_PATH];

    if ((dirp = opendir(dir)) == NULL) {
        return -1;
    }

    while ((tlist = readdir(dirp)) != NULL) {
        strcpy(aux, dir);
        strcat(strcat(aux, "/"), tlist->d_name);

        //comprobamos que sea un directorio valido
        if (strcmp(tlist->d_name, "..") == 0 || strcmp(tlist->d_name, ".") == 0)
            //no vuelve a comenzar la recursividad si es el directorio actual o el anterior
            continue;


        if (isDirectory(aux)) {//aqui se comprueba que sea un directorio

            borrar_dir(aux);//aqui repetimos recursivamente la funcion
        }

        if (remove(aux)) {//aqui se borra
            return -1;
        }
    }
    closedir(dirp);

    return 0;
}

int isDirEmpty(char *dirname) {   //ver si un directorio esta o no vacio
    int n = 0;
    struct dirent *p;
    DIR *dir = opendir(dirname);

    if (dir == NULL)
        return 1;

    while ((p = readdir(dir)) != NULL) {
        if (++n > 2)break;
    }

    if (p == NULL && errno != 0) {
        perror("error en readdir DirEmpty");
        return -1;
    }

    closedir(dir);

    if (n <= 2) //directorio vacio
        return 1;
    else
        return 0;
}


void ListarBloques(tHistMem bloques, int modo) { //modo == 4 si se listan todos

    if (modo == 4) { //se listan todos

        ListBLocks(bloques);

    } else {

        if (modo < 0 || 4 <
                        modo) { //no debería de hacer falta la comparación, pero por si acaso, si tal esto se quita para la entrega
            printf("no hay opción válida para ese número\n");
        } else {
            tmem tipo = (tmem) modo;
            printBLocks(bloques, tipo);
        }

    }

}


void Recursiva(int n) {
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf("parametro:%3d(%p) array %p, arr estatico %p\n", n, &n, automatico, estatico);

    if (n > 0)
        Recursiva(n - 1);
}


int asignarMalloc(tItemL entrada, tItemM *datos) {
    tItemT tam;
    getToken(1, entrada.comandos, tam);
    int tamano = (int) strtoul(tam, NULL, 10);
    time_t now;
    void *direccion;

    if ((direccion = malloc(sizeof tamano)) == NULL && tamano != 0) return -1;
    else if (tamano == 0) {
        printf("No se asignan bloques de 0 bytes\n");
        return 0;
    }

    time(&now);
    if (errno == -1) return -1;
    else {
        struct tm *local = localtime(&now);
        (*datos).tiempo = *local;
    }

    (*datos).tipo = maloc;
    (*datos).tamano = tamano;
    (*datos).direccion = direccion;

    printf("Asignados %ld bytes en %p\n", (*datos).tamano, (*datos).direccion);

    return 0;
}

void *ObtenerMemoriaShmget(key_t clave, size_t tam) {
    void *p;
    int aux, id, flags = 0777;
    struct shmid_ds s;

    if (tam)     /*tam distito de 0 indica crear */
        flags = flags | IPC_CREAT | IPC_EXCL;

    if (clave == IPC_PRIVATE)  /*no nos vale*/{
        errno = EINVAL;
        return NULL;
    }

    if ((id = shmget(clave, tam, flags)) == -1)
        return (NULL);

    if ((p = shmat(id, NULL, 0)) == (void *) -1) {
        aux = errno;
        if (tam)
            shmctl(id, IPC_RMID, NULL);

        errno = aux;
        return (NULL);
    }

    shmctl(id, IPC_STAT, &s);
    /* Guardar en la lista   InsertarNodoShared (&L, p, s.shm_segsz, clave); */ //-> ya se hace fuera si no se produce ningún error
    return (p);
}

int asignarCompartida(tItemL entrada, tItemM *datos, bool crear) {
    key_t cl;
    size_t tam;
    void *p;
    time_t now;
    tItemT clave, tamano;

    getToken(1, entrada.comandos, clave);
    cl = (key_t) strtoul(clave, NULL, 10);

    if (crear) { //createshared
        getToken(2, entrada.comandos, tamano);
        tam = (size_t) strtoul(tamano, NULL, 10);

        if (tam == 0) {
            printf("No se asignan bloques de 0 bytes\n");
            return 0; //no se guarda en la lista
        }

    } else { //shared
        tam = 0;
    }

    if ((p = ObtenerMemoriaShmget(cl, tam)) != NULL) {

        time(&now);
        if (errno == -1) return -1;
        else {
            struct tm *local = localtime(&now);
            (*datos).tiempo = *local;
        }

        (*datos).direccion = p;
        (*datos).tamano = tam;
        (*datos).tipo = shared;
        (*datos).clave = cl;

        printf("Asignados %lu bytes en %p\n", (unsigned long) tam, p);

        return 1;
    } else {
        printf("Imposible asignar memoria compartida clave %lu:%s\n", (unsigned long) cl, strerror(errno));
        return -1;
    }

}

void *MapearFichero(char *fichero, int protection, tItemM *datos) {
    int df, map = MAP_PRIVATE, modo = O_RDONLY; //tipo de mapeo,
    struct stat s;
    void *p;
    //printf("en el mapeado fichero %s permisos %d\n",fichero, protection);

    if (protection & PROT_WRITE)
        modo = O_RDWR; //lectura-escritura
    if (stat(fichero, &s) == -1 || (df = open(fichero, modo)) == -1) {
        printf("error en stat\n");
        return NULL;
    }
    if ((p = mmap(NULL, s.st_size, protection, map, df, 0)) == MAP_FAILED) {
        printf("error en mmap\n");
        return NULL;
    }

    (*datos).tamano = s.st_size;
    (*datos).file_descriptor = df;
    return p;
}

int asignarMap(tItemL entrada, tItemM *datos) {
    tItemT permisos, nombre;
    getToken(1, entrada.comandos, nombre);
    if (entrada.tokens == 3) strcpy(permisos, "---"); //permisos por defecto
    else getToken(2, entrada.comandos, permisos);
    void *p;
    int protection = 0;
    time_t now;

    if (entrada.tokens == 3 && strlen(permisos) < 4) { //se han introducido los argumentos correctamente
        if (strchr(permisos, 'r') != NULL) protection |= PROT_READ;
        if (strchr(permisos, 'w') != NULL) protection |= PROT_WRITE;
        if (strchr(permisos, 'x') != NULL) protection |= PROT_EXEC;
    }
    if ((p = MapearFichero(nombre, protection, datos)) == NULL) {
        perror("Imposible mapear fichero");
        return -1;
    } else {

        time(&now);
        if (errno == -1) return -1;
        else {
            struct tm *local = localtime(&now);
            (*datos).tiempo = *local;
        }

        (*datos).direccion = p;
        strcpy((*datos).nombre_archivo, nombre);
        (*datos).tipo = mapped;

        printf("fichero %s mapeado en %p\n", nombre, p);
        return 1;
    }
}


void desasignarMalloc(size_t tamano, tHistMem *bloques) { //tamaño y bloque
    tPosM posicion = findBlockMalloc(*bloques, tamano);

    if (posicion == MNULL) {
        printf("no hay un bloque de ese tamaño asignado con malloc\n");
    } else {

        tItemM bloque = getMemBlock(posicion);
        free(bloque.direccion);
        deleteMemBlock(posicion, bloques);
        printf("Dirección %p desasignada\n", bloque.direccion);

    }
}

void desasignarClave(key_t clave, tHistMem *bloques) { //elimina un bloque, no la clave del bloque
    int id;

    if (clave == IPC_PRIVATE) { //entrada.tokens == 1 || clave == IPC_PRIVATE
        printf("      delkey necesita clave_valida\n");
        return;
    }
    if ((id = shmget(clave, 0, 0666)) == -1) {
        perror("shmget: imposible obtener memoria compartida\n");
        return;
    }
    if (shmctl(id, IPC_RMID, NULL) == -1) {           //el NULL da igual porque se ignpra ese campo con IPC_RMID
        perror("shmctl: imposible eliminar memoria compartida\n");
    }

    //if(!delkey) deleteMemBlock(findBlockShared(*bloques, clave), bloques);

    printf("Clave %d desasignada\n", clave);
}

void desasignarMapped(tItemT nombre, tHistMem *bloques) { //fich es un nombre de fichero
    tPosM posicion = findBlockMapped(*bloques, nombre); //problemas aquí

    if (posicion == MNULL) {
        printf("Fichero %s no mapeado\n", nombre);
        return;
    } else {

        tItemM bloque = getMemBlock(posicion);
        if (munmap(bloque.direccion, bloque.tamano) == -1) {
            strerror(errno);
            return;
        } else {
            //free(bloque.direccion); //esto no es redundante??
            deleteMemBlock(posicion, bloques); //problemas aquí
            printf("borrando el fichero %s\n", nombre);
            return;
        }

    }
}

void desasignarDireccion(tItemL entrada, tHistMem *bloques) {
    tItemT aux;
    getToken(0, entrada.comandos, aux); //aquí es 0 porque haces deallocate 0x7f6f5e4d2000
    void *buscado = (void *) strtoul(aux, NULL, 16); //aux -> a unsigned long
    tPosM p;

    if ((p = findAddress(buscado, *bloques)) == MNULL) {
        printf("Direccion %p no asignada con malloc, shared o mmap\n", buscado);
        printf("%s\n", aux);
        return;
    }

    //borra de distinta forma dependiendo del tipo de bloque
    if (getMemBlock(p).tipo == maloc) {

        desasignarMalloc(getMemBlock(p).tamano, bloques);

    } else if (getMemBlock(p).tipo == shared) {

        deleteMemBlock(p, bloques);

    } else {

        desasignarMapped(getMemBlock(p).nombre_archivo, bloques);

    }

}

//i-o es igual a memdump y memfill, pero con el disco
int modos_IO(tItemL entrada, modo_IO *opciones) {
    int controlador = 0;

    for (int i = 0; i <= entrada.tokens - 2; i++) { //tokens es el total de tokens, incluido el ppal
        tItemT aux;
        getToken(i, entrada.comandos, aux);

        if (strcmp("-read", aux) == 0) {
            (*opciones).read = true;
            controlador++;
        } else if (strcmp("-write", aux) == 0) {
            (*opciones).write = true;
            controlador++;
        } else if (strcmp("-o", aux) == 0) {
            (*opciones).overwrite = true;
            controlador++;
        } else i = entrada.tokens - 2; //no se ha detectado ninguno -> actualizar i para terminar el bucle

    }
    return controlador;
}

ssize_t LeerFichero(char *f, void *p, size_t cont) { //nombre del fichero, dirección de memoria, tamaño
    //printf("p %p\n", p);
    struct stat s;
    ssize_t n;
    int df, aux;

    if (stat(f, &s) == -1 || (df = open(f, O_RDONLY)) == -1)
        return -1;
    if (cont == -1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont = s.st_size;
    if ((n = read(df, p, cont)) == -1) {
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close(df);
    return n;
}

ssize_t
EscribirFichero(char *f, const void *p, size_t cont, int overwrite) { //nombre, dirección, tamaño y si se sobreescribe
    ssize_t n;
    int df, aux, flags = O_CREAT | O_EXCL | O_WRONLY;

    if (overwrite)
        flags = O_CREAT | O_WRONLY | O_TRUNC;

    if ((df = open(f, flags, 0777)) == -1)
        return -1;

    if ((n = write(df, p, cont)) == -1) {
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close(df);
    return n;
}

int isNumber(char *string) {
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] < 48 || string[i] > 57) {
            if (string[i] != 45) {
                return 0;
            }
        }
    }
    return 1;
}

int minimo(int a, int b) {
    if (a < b) return a;
    else return b;
}

void dopmap() {

    pid_t pid;
    char npid[32];
    char *aux[3] = {"pmap", npid, NULL};

    sprintf(npid, "%d", (int) getpid());

    if ((pid = fork()) == -1) {
        perror("imposible crear el proceso");
        return;
    }
    if (pid == 0) {
        if (execvp(aux[0], aux) == -1)
            perror("no se puede ejecutar pmap");
        exit(1);

    }
    waitpid(pid, NULL, 0);

}

int BuscarVariable(char *var, char *e[]) {
    int pos = 0;
    char aux[MAX_LENGHT_PATH];

    strcpy(aux, var);
    strcat(aux, "=");

    while (e[pos] != NULL)
        if (!strncmp(e[pos], aux, strlen(aux)))
            return (pos);
        else
            pos++;
    errno = ENOENT;   /*no hay tal variable*/
    return (-1);
}

int CambiarVariable(char *var, char *valor, char *e[]) {
    int pos;
    char *aux;
    if ((pos = BuscarVariable(var, e)) == -1)//se busca la variable en el entorno e
        return -1;//si no está se devuelve -1

    if ((aux = (char *) malloc(strlen(var) + strlen(valor) + 2)) == NULL)
        return -1;
    strcpy(aux, var);
    strcat(aux, "=");
    strcat(aux, valor);
    e[pos] = aux;
    return (pos);

}

char *Ejecutable(char *s) { //te devuelve el programa al que tiene que llamar la llamada tipo exec
    char path[MAX_LENGHT_PATH];
    static char aux2[MAX_LENGHT_PATH];
    struct stat st;
    char *p;

    if (s == NULL || (p = getenv("PATH")) == NULL)
        return s;

    if (s[0] == '/' || !strncmp(s, "./", 2) || !strncmp(s, "../", 3))
        return s;       /*is an absolute pathname*/

    strncpy(path, p, MAX_LENGHT_PATH);

    for (p = strtok(path, ":"); p != NULL; p = strtok(NULL, ":")) {

        sprintf(aux2, "%s/%s", p, s);

        if (lstat(aux2, &st) != -1)
            return aux2;

    }

    return s;
}

//ejecutar en 1er plano con variables
int OurExecvpe(char *file, char *const argv[], char *const envp[]) {
    return (execve(Ejecutable(file), argv, envp));
}

int execute(char *prog, tItemL entrada, int argCounter, int envCounter, int prioridad, bool plano2) {
    int pid = 300; //pid que se obtendrá del hijo y currentPid con el que trabajamos para setear la prioridad
    char *argv[argCounter], *envp[envCounter];
    bool entorno = false;

    //printf("prog: %s\n",prog);
    if (envCounter != 0) {
        entorno = true;
        envp[0] = entrada.comando; //printf("env[0] = %s = %s\n",envp[0], entrada.comando);
        int j;
        for (j = 1; j < envCounter; j++) { //no vale el mismo contador porque se tiene en cuenta el comando ppal
            envp[j] = entrada.comandos.data[j - 1]; //printf("env[%d] %s = %s\n",j,envp[j], entrada.comandos.data[j-1]);
        }
        envp[j] = NULL; //printf("\n");
    }

    if (argCounter != 0) {
        int j, k = envCounter;
        argv[0] = prog; //printf("argv[0] %s = %s\n k = 0\n", argv[0], prog);
        //debería tener un -1 por el entrada.comando y un +1 por el programa en sí, el otro -1 es porque arrays
        for (j = 1; j < argCounter; j++) {
            argv[j] = entrada.comandos.data[k]; //printf("argv[%d] %s = %s\n k = %d\n",j,argv[j], entrada.comandos.data[k], k);
            k++;
        }
        argv[j] = NULL; //printf("\n");
    }
    printf("\n");


    if ((pid = fork()) == 0) { //este el código a ejecutarse

        if (prioridad != 0) {
            if (setpriority(PRIO_PROCESS, pid, prioridad) == -1) {
                strerror(errno);
                exit(0); //salir del proceso
            }
        }

        if (entorno) {
            if (OurExecvpe(prog, argv, envp) == -1) {
                perror("fallo al ejecutar el programa cambiando el entorno en primer plano");
                exit(0);
            }
        } else {
            //printf("Ejecutable: %s\n", Ejecutable(prog));
            if (execvp(Ejecutable(prog), argv) == -1) {
                perror("fallo al ejecutar el programa sin cambiar el entorno en primer plano");
                exit(0);
            }
        }
        exit(0); //al terminar la ejecución se sale

    }

    if (!plano2) waitpid(pid, NULL, 0); //esperar => prompt espera => el proceso padre

    //printf("el pid del proceso ejecutado es %d\n", pid);

    return pid; //va a devolver el pid del proceso creado
}

int convertPriority(tItemT prioridad) {
    int pri;
    pri = (int) strtoul(memmove(&prioridad[0], &prioridad[1], 256), NULL, 10);
    return pri;
}

//devuelve el número de señal a partir del nombre
int ValorSenal(char *sen) {
    int i;
    for (i = 0; sigstrnum[i].nombre != NULL; i++)
        if (!strcmp(sen, sigstrnum[i].nombre))
            return sigstrnum[i].valor;
    return -1;
}


//devuelve el nombre de la señal a partir de sen, para donde no hay sig2str
char *NombreSenal(int sen) {
    int i;
    for (i = 0; sigstrnum[i].nombre != NULL; i++)
        if (sen == sigstrnum[i].valor)
            return sigstrnum[i].nombre;
    return ("SIGUNKNOWN");
}

char *currentUser() {
    int pos = BuscarVariable("USER", environ);

    if (pos != -1) {
        return environ[pos];
    }
    return ("UNKNOWN");
}

void procInfo(tItemP p) {
    //obtenemos información del estado del proceso
    if (waitpid(p.pid, &(p.info), WNOHANG | WUNTRACED | WCONTINUED) == p.pid) {
        if (WIFEXITED(p.info) != 0) {
            p.estado = finished;
            p.info = WEXITSTATUS(p.info);
        } else if (WIFSIGNALED(p.info) != 0) {
            p.estado = signaled;
            p.info = WTERMSIG(p.info);
        } else if (WIFSTOPPED(p.info) != 0) {
            p.estado = stopped;
            p.info = WSTOPSIG(p.info);
        } else if (WIFCONTINUED(p.info) != 0) {
            p.estado = active;
            //no hace falta actualizar el int de información
        }
    }

    //"constante"
    printf("%6d %5s p = %d %s ", p.pid, currentUser(), getpriority(PRIO_PROCESS, p.pid), p.fecha);

    //lo que depende del estado
    if (p.estado == finished) {
        printf(" FINISHED (%03d) ", p.info);
    } else if (p.estado == stopped) {
        printf(" STOPPED (%s) ", NombreSenal(p.info));
    } else if (p.estado == signaled) {
        printf(" SIGNALED (%s) ", NombreSenal(p.info));
    } else if (p.estado == active) {
        printf(" ACTIVO (%03d) ", p.info);
    }

    printComand(p.comando, false, true);
}

