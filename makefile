CC = gcc
CFLAGS = -g -Wall
SRC = codigo_p0.c historial.c list.h static_tokens.c static_tokens.h
OBJ = codigo_p0.o historial.o static_tokens.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

tokens.o: static_tokens.c static_tokens.h
funciones.o: historial.c historial.h static_tokens.h
codigo_p0.o: codigo_p0.c historial.h static_tokens.h