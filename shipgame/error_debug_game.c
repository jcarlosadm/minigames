/*
 * error_debug_game.c
 *
 */

#include "error_debug_game.h"


/* -------------------------------------------------------------
 * Mostra mensagem de erro, linha e arquivo onde ocorreu.
 * -------------------------------------------------------------
 * Parâmetros:
 * const char* message : mensagem a ser mostrada.
 * const char* file : nome do arquivo em que ocorre o erro
 * int line : linha em que ocorre o erro
 ---------------------------------------------------------------*/
void error_game_message(const char* message, const char* file, int line){

    // imprime a mensagem
    puts(message);
    // informa o nome do arquivo e a linha
    printf("file %s, line %d\n",file,line);

}
