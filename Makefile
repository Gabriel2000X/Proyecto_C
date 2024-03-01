# Gabriel Gonzales
# Daniel Rodriguez
# Laisa Solis
# Este es el programa makefile, funciona para compilar p1 y p2 y crear su ejecutable

CC = gcc # Se selecciona c
LDFLAGS_p1 = `pkg-config --cflags --libs gtk+-3.0` # Se ponen todas las flags y libs para p1
LDLIBS_p2 = -lm # Se pone la lib necesaria p2

# Aca se definen los archivos para cada target y el nombre del ejecutable
SRCS_p1 = ./proyecto1.c
SRCS_p2 = ./proyecto2.c
EXE_p1 = exe
EXE_p2 = exe

# Se llama a compilar p1 y p2
all: p1 p2

# Se hace el target de p1
p1: $(SRCS_p1)
	$(CC) -o $(EXE_p1) $^ $(LDFLAGS_p1)

# Se hace el target de p2
p2: $(SRCS_p2)
	$(CC) $^ -o $(EXE_p2) $(LDLIBS_p2)

# Se elimina el archivo exe
clean:
	rm exe
