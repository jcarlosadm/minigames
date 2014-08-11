/*
 * window.h
 * Módulo que gerencia a janela
 *
 */

#ifndef WINDOW_H_
#define WINDOW_H_

// bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
// allegro
#include <allegro5/allegro.h>

/*************************************************
 * ***********************************************
 * Constantes
 * ***********************************************
 *************************************************/

/* --------------------------------------------------------------------------
 * A largura e altura padrão
 * --------------------------------------------------------------------------
 * São usados quando não for possível capturar a largura e altura do monitor
 ----------------------------------------------------------------------------*/
// Largura padrão da janela principal
#define WINDOW_WIDTH 640
// Altura padrão da janela principal
#define WINDOW_HEIGHT 480

/* ---------------------------------------------------
 * Quantidade de frames (quadros) por segundo
 -----------------------------------------------------*/
#define FPS_GAME 60

/* -----------------------------------------------
 * Enumeradores da tela atual
 -------------------------------------------------*/
enum{
    // tela de título
    WINDOW_TITLE_MAIN,
    // tela de título quando escolhe start
    WINDOW_TITLE_START,
    // tela de título quando escolhe options
    WINDOW_TITLE_OPTIONS,
    // tela de título quando escolhe exit
    WINDOW_TITLE_EXIT,
    // tela de jogo
    WINDOW_GAME_MAIN,
    // tela de jogo quando estiver no menu
    WINDOW_GAME_MENU,
    // tela de jogo quando estiver no modo pause
    WINDOW_GAME_PAUSE
};


/**************************************************
 * ************************************************
 * Estruturas
 * ************************************************
 **************************************************/

/* --------------------------------------------------
 * Estrutura Window_game
 * --------------------------------------------------
 * A janela de jogo
 ----------------------------------------------------*/
typedef struct window_game Window_game;

/* --------------------------------------------------
 * Estrutura Timer_game
 * --------------------------------------------------
 * Controla a quantidade de frames por segundo
 ----------------------------------------------------*/
typedef struct timer_game Timer_game;


/***************************************************
 * *************************************************
 * Construtores
 * *************************************************
 ***************************************************/

/* ----------------------------------------------------------------
 * Cria a janela de jogo
 * ----------------------------------------------------------------
 * Aloca memória no objeto Window_game (a janela de jogo),
 * cria lista de eventos da janela e lista de eventos do
 * timer (que controlará o FPS)
 *
 * Retorna false se não for bem sucedido
 * ----------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro para ponteiro para Window_game
 * Timer_game** timer : ponteiro para ponteiro para Timer_game
 ------------------------------------------------------------------*/
int create_window_game(Window_game** window, Timer_game** timer);



/**************************************************
 * ************************************************
 * Destrutores
 * ************************************************
 **************************************************/

/* -----------------------------------------------------------------
 * Desaloca objeto Window_game
 * -----------------------------------------------------------------
 * Desaloca objeto display, a lista de eventos da janela,
 * o objeto Window_game e a lista de eventos do timer
 *
 * Execute após o loop principal do jogo
 * -----------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro para ponteiro para Window_game
 * Timer_game** timer : ponteiro para ponteiro para Timer_game
 -------------------------------------------------------------------*/
void dealloc_window(Window_game** window,Timer_game** timer);



/**************************************************
 * ************************************************
 * Funções de window
 * ************************************************
 **************************************************/

/* ---------------------------------------------------------------
 * Inicia timer
 * ---------------------------------------------------------------
 * Use antes do loop principal do jogo
 *
 * O timer controla a quantidade de frames por
 * segundo do jogo
 * ----------------------------------------------------------------
 * Parâmetros:
 * Timer_game** timer : ponteiro para ponteiro para Timer_game
 ------------------------------------------------------------------*/
void start_timer(Timer_game** timer);

/* -----------------------------------------------------------------
 * Retorna o valor de exit da janela
 * -----------------------------------------------------------------
 * Retorno true ou false
 *
 * O valor de exit é usado para verificar
 * se o loop principal deve continuar executando
 * (use na verificação do while principal)
 * -----------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro para ponteiro para Window_game
 -------------------------------------------------------------------*/
int get_window_exit_value(Window_game** window);

/* --------------------------------------------------------------
 * Retorna o valor de tick da janela
 * --------------------------------------------------------------
 * Retorno true ou false
 *
 * O valor de tick é usado para verificar
 * o momento de execução do próximo loop
 * de jogo, para ajustar de acordo com o
 * FPS_GAME especificado
 *
 * Crie um if dentro do loop principal
 * e use essa função para verificar se
 * esse if deve ser executado. tudo
 * no jogo deve ficar dentro desse if
 * ----------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro para ponteiro para Window_game
 ------------------------------------------------------------------*/
int get_window_tick(Window_game** window);

/* ----------------------------------------------------------------
 * Configura o valor de tick da janela para false
 * ----------------------------------------------------------------
 * Use no final da execução do if interno ao loop
 * principal do jogo
 * ----------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro para ponteiro para Window_game
 ------------------------------------------------------------------*/
void set_window_tick_false(Window_game** window);

/* ---------------------------------------------------------------
 * Coloca a janela como local de desenho atual
 * ---------------------------------------------------------------
 * Chame-a sempre que desenhar em outro local e
 * desejar voltar a desenhar na janela
 * ---------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro para ponteiro para Window_game
 -----------------------------------------------------------------*/
void set_draw_current_window_game(Window_game** window);

/* ------------------------------------------------------------------
 * Configura o mouse para usar o cursor padrão do sistema na janela
 * ------------------------------------------------------------------
 * Retorna false se não conseguir
 * ------------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro para ponteiro para Window_game
 --------------------------------------------------------------------*/
int set_mouse_cursor_window(Window_game** window);

/* -------------------------------------------------------------
 * Verifica eventos da janela e timer e os executa
 * -------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro para ponteiro para Window_game
 * Timer_game** timer : ponteiro para ponteiro para Timer_game
 ---------------------------------------------------------------*/
void check_event_queue_window(Window_game** window, Timer_game** timer);

#endif /* WINDOW_H_ */
