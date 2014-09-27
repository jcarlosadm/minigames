/*
 * error_debug_game.h
 *
 */

#ifndef ERROR_DEBUG_GAME_H_
#define ERROR_DEBUG_GAME_H_

// bibliotecas padrão
#include <stdio.h>

/*****************************************************************
 * ***************************************************************
 * Constantes
 * ***************************************************************
 *****************************************************************/

/* ---------------------------------------------------------------
 * Define se mostra informações de debug
 * ---------------------------------------------------------------
 * Valores possíveis: true ou false
 -----------------------------------------------------------------*/
#define DEBUG_ON true

/*****************************************************************
 * ***************************************************************
 * Funções do módulo
 * ***************************************************************
 *****************************************************************/

/* -------------------------------------------------------------
 * Mostra mensagem de erro, linha e arquivo onde ocorreu.
 * -------------------------------------------------------------
 * Parâmetros:
 * const char* message : mensagem a ser mostrada.
 * const char* file : nome do arquivo em que ocorre o erro
 * int line : linha em que ocorre o erro
 ---------------------------------------------------------------*/
void error_game_message(const char* message, const char* file, int line);

#endif /* ERROR_DEBUG_GAME_H_ */