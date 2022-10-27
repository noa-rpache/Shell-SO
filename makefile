CC = gcc
CFLAGS = -Wall
SRC = codigo_p0.c funciones.c funciones.h historial.c historial.h static_tokens.c static_tokens.h
OBJ = codigo_p0.o funciones.o historial.o static_tokens.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

tokens.o: static_tokens.c static_tokens.h
historial.o: historial.c historial.h static_tokens.h
funciones.o: funciones.c funciones.h historial.h static_tokens.h
codigo_p0.o: codigo_p0.c funciones.h historial.h static_tokens.h