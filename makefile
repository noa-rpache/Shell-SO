CC = gcc
CFLAGS = -g -Wall
<<<<<<< HEAD
SRC = codigo_p0.c list.c list.h #tokens_list.c tokens_list.h
OBJ = codigo_p0.o list.o #tokens_list.o
=======
SRC = codigo_p0.c historial.c historial.h
OBJ = codigo_p0.o historial.o
>>>>>>> a064e3bc3b30153746f3f1cc2f0e9c5e7ac809b3

all: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)
	

<<<<<<< HEAD
funciones.o: list.c list.h #tokens_list.c #tokens_list.h
codigo_p0.o: codigo_p0.c list.h #tokens_list.h
=======
funciones.o: historial.c historial.h 
codigo_p0.o: codigo_p0.c historial.h
>>>>>>> a064e3bc3b30153746f3f1cc2f0e9c5e7ac809b3
