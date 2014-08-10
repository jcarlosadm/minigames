/*
 * window.h
 * Módulo que gerencia a janela
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
 * Estruturas
 **************************************************/

/* ------------------------------------------------
 * Estrutura da janela de jogo
 * ------------------------------------------------
 * (veja as funções de janela para mais detalhes)
 --------------------------------------------------*/
typedef struct window_game Window_game;

/***************************************************
 * Construtores
 **************************************************/

/* -----------------------------------------------------------
 * Cria a janela de jogo
 * -----------------------------------------------------------
 * Aloca memória no objeto Window_game (a janela de jogo) e
 * cria lista de eventos da janela
 *
 * Retorna false se não for bem sucedido
 * -----------------------------------------------------------
 * Nota: há um objeto window visível apenas dentro do módulo.
 * Esse objeto é alocado quando essa função é executada.
 -------------------------------------------------------------*/
int create_window_game();

/**************************************************
 * Destrutores
 **************************************************/

/* -----------------------------------------------------------
 * Desaloca objeto Window_game do módulo
 * -----------------------------------------------------------
 * Desaloca objeto display, a lista de eventos da janela, e
 * o objeto Window_game
 *
 * Execute após o loop principal do jogo
 -------------------------------------------------------------*/
void desaloc_window();

/**************************************************
 * Funções de window
 **************************************************/

/* ----------------------------------------------
 * Retorna o valor de exit da janela
 * ----------------------------------------------
 * Retorno true ou false
 *
 * O valor de exit é usado para verificar
 * se o loop principal deve continuar executando
 ------------------------------------------------*/
int get_window_exit_value();

/* ---------------------------------------
 * Retorna o valor de tick da janela
 * ---------------------------------------
 * Retorno true ou false
 *
 * O valor de tick é usado para verificar
 * o momento de execução do próximo loop
 * de jogo, para ajustar de acordo com o
 * FPS_GAME especificado
 -----------------------------------------*/
int get_window_tick();

/* ---------------------------------------------
 * Configura valor de exit da janela para true
 * ---------------------------------------------
 * Chamando essa função, a variável tick será
 * true, possibilitando sair do loop principal
 * do jogo.
 -----------------------------------------------*/
void set_window_exit_true();

/* -------------------------------------------------
 * Configura o valor de tick da janela
 * -------------------------------------------------
 * Use com funções de time para executar o loop
 * principal somente dentro do FPS_GAME especificado
 * -------------------------------------------------
 * Parâmetros:
 * value : true ou false
 ---------------------------------------------------*/
void set_window_tick(int value);

/* --------------------------------------------------
 * Coloca a janela como local de desenho atual
 * --------------------------------------------------
 * Chame-a sempre que desenhar em outro local e
 * desejar voltar a desenhar na janela
 ----------------------------------------------------*/
void set_draw_current_window_game();

/* ------------------------------------------------------------------
 * Configura o mouse para usar o cursor padrão do sistema na janela
 * ------------------------------------------------------------------
 * Retorna false se não conseguir
 --------------------------------------------------------------------*/
int set_mouse_cursor_window();

/* -------------------------------------------------------------
 * Verifica eventos da janela e os executa
 ---------------------------------------------------------------*/
void check_event_queue_window();

#endif /* WINDOW_H_ */
