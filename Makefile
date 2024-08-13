CC = gcc
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin
HDRS = $(INC)/pontos_recarga.h
OBJS = $(OBJ)/main.o $(OBJ)/pontos_recarga.o
CFLAGS = -Wall -g -c -I$(INC) -std=c11
EXE = $(BIN)/geracarga.out

all: dirs $(EXE)

dirs:
	mkdir -p $(OBJ) $(BIN)

# Executar o programa
run: $(EXE)
	$(EXE)


$(EXE): $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(LIBS)


$(OBJ)/%.o: $(SRC)/%.c $(HDRS)
	$(CC) $(CFLAGS) -o $@ $<

# Limpar arquivos compilados e diretórios
clean:
	rm -f $(EXE) $(OBJS)
	rm -rf $(BIN) $(OBJ)
