############ início da configuração ############################

# diretórios dos objetos e do binário
# tenha certeza de configurar a localização do binário nas opções de execução do eclipse
OBJ_DIR= objects
BIN_DIR= bin

# coloque aqui a lista de objetos do programa
_OBJ= error_debug_game.o graphics.o controls.o window.o ships.o main.o

# coloque as depedências de header files de cada objeto (exceto o header do próprio objeto)
# veja o header file de cada objeto para uma pista dessas depedências
DEP_ERROR_DEBUG_GAME=
DEP_GRAPHICS= window.h
DEP_CONTROLS= window.h
DEP_WINDOW=
DEP_SHIPS= error_debug_game.h graphics.h window.h controls.h
DEP_MAIN= error_debug_game.h graphics.h window.h ships.h controls.h

# as flags e opções usadas
CC= gcc
CFLAGS= -c -Wall
ALLEGROFLAGS= -lallegro -lallegro_image
MINIXMLFLAGS= -lmxml

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
	$(CC) -o $(BIN_DIR)/$@ $^ $(ALLEGROFLAGS) $(MINIXMLFLAGS)

$(OBJ_DIR)/error_debug_game.o: error_debug_game.c error_debug_game.h $(DEP_ERROR_DEBUG_GAME)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/graphics.o: graphics.c graphics.h $(DEP_GRAPHICS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/controls.o: controls.c controls.h $(DEP_CONTROLS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/window.o: window.c window.h $(DEP_WINDOW)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/ships.o: ships.c ships.h $(DEP_SHIPS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/main.o: main.c $(DEP_MAIN)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/$(BIN_NAME)