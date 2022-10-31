/*
 * Grupo 2.2
 * Noa Rodríguez Pache - noa.rpache
 * Fátima Ansemil - fatima.ansemil
 * */

#include "funciones.h"

//auxiliares
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
    if(convertido < 0 ) return convertido*(-1);
    else return convertido;
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
    char aux[MAX_LENGHT_PATH];

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

void ListarBloques(tHistMem bloques){
    printf("Bloques asignados al proceso %d\n",getpid());
    if(!isEmptyMem(bloques)){
        printf("aquí va la lista de bloques\n");
        for( tPosM p = primeroBlock(bloques); p != MNULL; p= nextBlock(p));
    }
}

void Recursiva (int n){
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0)
        Recursiva(n-1);
}

int asignarMalloc(tItemL entrada,tItemM *datos){
    tItemT tam;
    getToken(2,entrada.comandos,tam);
    int tamano = int_convert(tam);

    if ( malloc(sizeof tamano) == NULL && tamano != 0 ) return -1;

    (*datos).tipo = maloc;
    (*datos).tamano = tamano;
    (*datos).direccion = malloc(sizeof tamano);
    return 0;
}

int asignarCompartida(tItemL entrada,tItemM *datos){
    tItemT clave, tam;
    getToken(2,entrada.comandos,clave);
    getToken(3,entrada.comandos,tam);
    int tamano = int_convert(tam);

    if(shmget() == -1) return -1;

    (*datos).direccion = ;
    (*datos).tamano = tamano;
    (*datos).tipo = shared;
    //cuándo se asignó
    (*datos).clave = /*clave*/;
    return 0;
}

void * ObtenerMemoriaShmget (key_t clave, size_t tam)
{
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;

    if (tam)     /*tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL;
    if (clave==IPC_PRIVATE)  /*no nos vale*/
    {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);
    /* Guardar en la lista   InsertarNodoShared (&L, p, s.shm_segsz, clave); */
    return (p);
}

void do_AllocateCreateshared (char *tr[])
{
    key_t cl;
    size_t tam;
    void *p;

    if (tr[0]==NULL || tr[1]==NULL) {
        ImprimirListaShared(&L);
        return;
    }

    cl=(key_t)  strtoul(tr[0],NULL,10);
    tam=(size_t) strtoul(tr[1],NULL,10);
    if (tam==0) {
        printf ("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p=ObtenerMemoriaShmget(cl,tam))!=NULL)
        printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
    else
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}

int desasignarMalloc(tItemL entrada,tItemM *datos, tHistMem *bloques){
    tItemT tam;
    getToken(1, entrada.comandos,tam),
    int tamano = int_convert(tam);
    tPosM posicion = findMemblock(,bloques);

    if(posicion == MNULL){
        //se borra un bloque que no está en el historial
    }else{
        tItemM bloque = getMemBlock(posicion);
        free(bloque.direccion);
        deleteMemBlock(posicion,bloques);
    }
}








