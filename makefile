CC = gcc
CFLAGS = -g -Wall
SRC = codigo_p0.c historial.c historial.h
OBJ = codigo_p0.o historial.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)
	

funciones.o: historial.c historial.h 
codigo_p0.o: codigo_p0.c historial.h
