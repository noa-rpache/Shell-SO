CC = gcc
CFLAGS = -g -Wall
SRC = codigo_p0.c list.c list.h
OBJ = codigo_p0.o list.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)
	

funciones.o: list. list.h 
codigo_p0.o: codigo_p0.c list.h
