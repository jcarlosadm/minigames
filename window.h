/*
 * window.h
 *
 */

#ifndef WINDOW_H_
#define WINDOW_H_

// bibliotecas básicas
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

/*************************************************
 * Constantes
 *************************************************/

// Largura da janela principal
#define WINDOW_WIDTH 640
// Altura da janela principal
#define WINDOW_HEIGHT 480
// Quantidade de frames por segundo
#define FPS_GAME 60

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
 * Aloca memória no objeto global window, do tipo Window_game
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
 * Retorna o valor de exit da janela
 */
int get_window_exit_value();

/*
 * Retorna o valor de redraw da janela
 */
int get_window_redraw_value();

/*
 * Retorna o valor de tick da window
 */
int get_window_tick();

/*
 * Configura valor de exit da janela para true
 */
void set_window_exit_true();

/*
 * Permite redesenho da janela
 * Chame sempre que houver modificação visual na janela
 *
 * value : true ou false
 */
void set_window_redraw(int value);

/*
 * Configura o valor de tick da janela
 *
 * value : true ou false
 */
void set_window_tick(int value);

/*
 * Coloca a tela como local de desenho atual
 */
void set_draw_current_window_game();

/*
 * Configura o mouse para usar o cursor padrão do sistema na janela
 *
 * retorno booleano (true ou false)
 * retorna false se não conseguir
 */

int set_mouse_cursor_window();

/*
 * Registra eventos da janela
 */
void set_events_window(ALLEGRO_EVENT_QUEUE* event_queue);

ALLEGRO_DISPLAY* get_display_window();

int generate_event_queue_window();

void register_event_queue_window();

void verifying_event_queue_window();

#endif /* WINDOW_H_ */
