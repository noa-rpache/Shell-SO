CC = gcc
CFLAGS = -g -Wall
SRC = p3.c funciones.c funciones.h memoria.h memoria.c historial.c historial.h static_tokens.c static_tokens.h
OBJ = p3.o funciones.o memoria.o historial.o static_tokens.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o p3 $(OBJ)

tokens.o: static_tokens.c static_tokens.h
historial.o: historial.c historial.h static_tokens.h
memoria.o: memoria.h memoria.c
funciones.o: funciones.c funciones.h memoria.h historial.h static_tokens.h
p3.o: p3.c funciones.h memoria.h historial.h static_tokens.h
