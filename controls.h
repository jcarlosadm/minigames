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

/* -------------------------------------------------------------------
 * Estrutura que contém os controles e a lista de eventos de controle
 * -------------------------------------------------------------------
 * Essa estrutura inclui informações de:
 * - Mouse
 * - Lista de eventos de controle
 ---------------------------------------------------------------------*/
typedef struct controls_game Controls_game;

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
 * Controls_game** controls : ponteiro** para Controls_game
 * Window_game** window : ponteiro** para Window_game
 -----------------------------------------------------------------------------*/
int start_controls(Controls_game** controls, Window_game** window);



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
 * Controls_game** controls : ponteiro** para Controls_game
 ----------------------------------------------------------------------------*/
void dealloc_controls(Controls_game** controls);



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
 * Controls_game** controls : ponteiro** para Controls_game
 ------------------------------------------------------------------------------*/
void check_event_queue_controls(Controls_game** controls);

/* ---------------------------------------------------------
 * Atualiza controles
 * ---------------------------------------------------------
 * atualiza as variáveis membro de mouse para uso das funções
 * de mouse abaixo
 * ---------------------------------------------------------
 * Parâmetros:
 * Controls_game** controls : ponteiro** para Controls_game
 -----------------------------------------------------------*/
void update_controls(Controls_game** controls);

/* ---------------------------------------------------------
 * Pega estado do movimento de mouse
 * ---------------------------------------------------------
 * Retorna true se o mouse se movimentou. Caso contrário,
 * retorna false
 * ---------------------------------------------------------
 * Parâmetros:
 * Controls_game** controls : ponteiro** para Controls_game
 -----------------------------------------------------------*/
int get_mouse_move_state(Controls_game** controls);

/* ---------------------------------------------------------
 * Retorna a coordenada x do mouse
 * ---------------------------------------------------------
 * Parâmetros:
 * Controls_game** controls : ponteiro** para Controls_game
 -----------------------------------------------------------*/
int get_mouse_x(Controls_game** controls);

/* ---------------------------------------------------------
 * Retorna a coordenada y do mouse
 * ---------------------------------------------------------
 * Parâmetros:
 * Controls_game** controls : ponteiro** para Controls_game
 -----------------------------------------------------------*/
int get_mouse_y(Controls_game** controls);

/* ---------------------------------------------------------
 * Retorna true se houver um clique no mouse (segurar o clique
 * retorna false)
 * ---------------------------------------------------------
 * Parâmetros:
 * Controls_game** controls : ponteiro** para Controls_game
 -----------------------------------------------------------*/
int mouseIsClicked(Controls_game** controls);

/* ----------------------------------------------------------
 * Retorna true se segurar o clique do mouse
 * ----------------------------------------------------------
 * Parâmetros:
 * Controls_game** controls : ponteiro** para Controls_game
 ------------------------------------------------------------*/
int mouseIsPressed(Controls_game** controls);

/* ----------------------------------------------------------
 * Retorna true se soltar o clique do mouse
 * ----------------------------------------------------------
 * Parâmetros:
 * Controls_game** controls : ponteiro** para Controls_game
 ------------------------------------------------------------*/
int mouseIsReleased(Controls_game** controls);

#endif /* CONTROLS_H_ */
