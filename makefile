CC = gcc
CFLAGS = -Wall
SRC = p2.c funciones.c funciones.h historial.c historial.h static_tokens.c static_tokens.h
OBJ = p2.o funciones.o historial.o static_tokens.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

tokens.o: static_tokens.c static_tokens.h
historial.o: historial.c historial.h static_tokens.h
funciones.o: funciones.c funciones.h historial.h static_tokens.h
p2.o: p2.c funciones.h historial.h static_tokens.h