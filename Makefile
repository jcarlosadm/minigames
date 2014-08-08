############ início da configuração ############################

# diretórios dos objetos e do binário
# tenha certeza de configurar a localização do binário nas opções de execução do eclipse
OBJ_DIR= objects
BIN_DIR= bin

# coloque aqui a lista de objetos do programa
_OBJ= controls.o window.o ships.o main.o

# as flags e opções usadas
CC= gcc
CFLAGS= -c -Wall
ALLEGROFLAGS= -lallegro -lallegro_image

# nome do binário gerado
BIN_NAME= main

############ fim da configuração ###############################

# gera lista de objetos com caminhos relativos na pasta de objetos
OBJ= $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

# comando para criar diretórios
MK_DIR= mkdir -p

.PHONY: makedir_objects
.PHONY: makedir_bin

all: makedir_objects makedir_bin $(BIN_NAME)

makedir_objects:
	$(MK_DIR) $(OBJ_DIR)

makedir_bin:
	$(MK_DIR) $(BIN_DIR)

$(BIN_NAME): $(OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ $(ALLEGROFLAGS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/$(BIN_NAME)