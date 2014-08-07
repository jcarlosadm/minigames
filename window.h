/*
 * window.h
 *
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

/**************************************************
 * Estruturas
 **************************************************/

// Estrutura da janela de jogo
typedef struct window_game Window_game;

/*************************************************
 * variáveis globais
 *************************************************/

// janela principal de jogo
Window_game *window;

/***************************************************
 * Construtores
 **************************************************/

/*
 * cria a janela de jogo
 * Aloca memória e retorna um ponteiro para Window_game
 */
void create_window_game();

/**************************************************
 * Destrutores
 **************************************************/

/*
 * Desaloca Window_game
 */
void desaloc_window();

/**************************************************
 * Funções de window
 **************************************************/

/*
 * Coloca a tela como local de desenho atual
 */
void set_draw_current_window_game();

#endif /* WINDOW_H_ */
