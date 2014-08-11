/*
 * controls.h
 * Módulo que gerencia os controles
 *
 */

#ifndef CONTROLS_H_
#define CONTROLS_H_

// bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
// allegro
#include <allegro5/allegro.h>
// módulos do jogo
#include "window.h"

/***************************************************************
 * *************************************************************
 * Estruturas
 * *************************************************************
 ***************************************************************/

/*------------------------------------------------------------
 * Estrutura da lista de eventos dos controles
 -------------------------------------------------------------*/
typedef struct event_queue_controls Event_queue_controls;

/* ------------------------------------------------------------
 * Estrutura do mouse do jogo
 --------------------------------------------------------------*/
typedef struct mouse_game Mouse_game;

/**********************************************************
 * ********************************************************
 * Construtores
 * ********************************************************
 **********************************************************/

/* ---------------------------------------------------------------------------
 * Inicia e aloca controles
 * ---------------------------------------------------------------------------
 * Inicia o mouse e a lista de eventos de controles
 *
 * Retorna false se não conseguir
 * ---------------------------------------------------------------------------
 * Parâmetros:
 * Mouse_game** mouse : ponteiro** para Mouse_game
 * Event_queue_controls** event_queue : ponteiro** para Event_queue_controls
 * Window_game** window : ponteiro** para Window_game
 -----------------------------------------------------------------------------*/
int start_controls(Mouse_game** mouse,Event_queue_controls** event_queue,Window_game** window);



/************************************************************
 * **********************************************************
 * Destrutores
 * **********************************************************
 ************************************************************/

/* --------------------------------------------------------------------------
 * Desaloca controles
 * --------------------------------------------------------------------------
 * Desaloca mouse e lista de eventos de controles
 * --------------------------------------------------------------------------
 * Parâmetros:
 * Mouse_game** mouse : ponteiro** para Mouse_game
 * Event_queue_controls** event_queue : ponteiro** para Event_queue_controls
 ----------------------------------------------------------------------------*/
void dealloc_controls(Mouse_game** mouse, Event_queue_controls** event_queue);



/************************************************************
 * **********************************************************
 * Funções de controls
 * **********************************************************
 ************************************************************/

/* ---------------------------------------------------------------------------
 * Verifica lista de eventos de controles
 * ---------------------------------------------------------------------------
 * Essa funções checa a ocorrência de eventos, e configura
 * os membros de mouse de acordo
 *
 * Use no loop interno ao loop principal de jogo, e depois
 * use as funções de mouse abaixo para verificar alguma mudança
 * ----------------------------------------------------------------------------
 * Parâmetros:
 * Mouse_game** mouse : ponteiro** para Mouse_game
 * Event_queue_controls** event_queue : ponteiro** para Event_queue_controls
 ------------------------------------------------------------------------------*/
void check_event_queue_controls(Mouse_game** mouse, Event_queue_controls** event_queue);

/* ---------------------------------------------------------
 * Atualiza controles
 * ---------------------------------------------------------
 * atualiza as variáveis membro de mouse para uso das funções
 * de mouse abaixo
 * ---------------------------------------------------------
 * Parâmetros:
 * Mouse_game** mouse : ponteiro** para Mouse_game
 -----------------------------------------------------------*/
void update_controls(Mouse_game** mouse);

/* ---------------------------------------------------------
 * Pega estado do movimento de mouse
 * ---------------------------------------------------------
 * Retorna true se o mouse se movimentou. Caso contrário,
 * retorna false
 * ---------------------------------------------------------
 * Parâmetros:
 * Mouse_game** mouse : ponteiro** para Mouse_game
 -----------------------------------------------------------*/
int get_mouse_move_state(Mouse_game** mouse);

/* ---------------------------------------------------------
 * Retorna a coordenada x do mouse
 * ---------------------------------------------------------
 * Parâmetros:
 * Mouse_game** mouse : ponteiro** para Mouse_game
 -----------------------------------------------------------*/
int get_mouse_x(Mouse_game** mouse);

/* ---------------------------------------------------------
 * Retorna a coordenada y do mouse
 * ---------------------------------------------------------
 * Parâmetros:
 * Mouse_game** mouse : ponteiro** para Mouse_game
 -----------------------------------------------------------*/
int get_mouse_y(Mouse_game** mouse);

/* ---------------------------------------------------------
 * Retorna true se houver um clique no mouse (segurar o clique
 * retorna false)
 * ---------------------------------------------------------
 * Parâmetros:
 * Mouse_game** mouse : ponteiro** para Mouse_game
 -----------------------------------------------------------*/
int mouseIsClicked(Mouse_game** mouse);

/* ----------------------------------------------------------
 * Retorna true se segurar o clique do mouse
 * ----------------------------------------------------------
 * Parâmetros:
 * Mouse_game** mouse : ponteiro** para Mouse_game
 ------------------------------------------------------------*/
int mouseIsPressed(Mouse_game** mouse);

/* ----------------------------------------------------------
 * Retorna true se soltar o clique do mouse
 * ----------------------------------------------------------
 * Parâmetros:
 * Mouse_game** mouse : ponteiro** para Mouse_game
 ------------------------------------------------------------*/
int mouseIsReleased(Mouse_game** mouse);

#endif /* CONTROLS_H_ */
