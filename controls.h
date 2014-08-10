/*
 * controls.h
 * Módulo que gerencia os controles
 *
 */

#ifndef CONTROLS_H_
#define CONTROLS_H_

// bibliotecas básicas
#include <allegro5/allegro.h>
#include <stdlib.h>
#include <stdio.h>
// outros módulos
#include "window.h"

/**********************************************************
 * ********************************************************
 * Construtores
 * ********************************************************
 **********************************************************/

/* ---------------------------------------------------------
 * Inicia e aloca controles
 * ---------------------------------------------------------
 * Inicia o mouse e a lista de eventos de controles
 *
 * Retorna false se não conseguir
 * ---------------------------------------------------------
 * Nota: esse módulo possui um objeto interno que representa
 * o mouse e outro que representa os eventos de controle.
 * Esses objetos é que são iniciados.
 -----------------------------------------------------------*/
int start_controls();



/************************************************************
 * **********************************************************
 * Destrutores
 * **********************************************************
 ************************************************************/

/* ---------------------------------------------------------
 * Desaloca controles
 * ---------------------------------------------------------
 * Desaloca mouse e lista de eventos de controles
 -----------------------------------------------------------*/
void dealloc_controls();



/************************************************************
 * **********************************************************
 * Funções de controls
 * **********************************************************
 ************************************************************/

/* ---------------------------------------------------------
 * Verifica lista de eventos de controles
 * ---------------------------------------------------------
 * Essa funções checa a ocorrência de eventos, e configura
 * os membros de mouse de acordo
 *
 * Use no loop interno ao loop principal de jogo, e depois
 * use as funções de mouse abaixo para verificar alguma mudança
 -----------------------------------------------------------*/
void check_event_queue_controls();

/* ---------------------------------------------------------
 * Atualiza controles
 * ---------------------------------------------------------
 * atualiza as variáveis membro de mouse para uso das funções
 * de mouse abaixo
 -----------------------------------------------------------*/
void update_controls();

/* ---------------------------------------------------------
 * Pega estado do movimento de mouse
 * ---------------------------------------------------------
 * Retorna true se o mouse se movimentou. Caso contrário,
 * retorna false
 -----------------------------------------------------------*/
int get_mouse_move_state();

/* ---------------------------------------------------------
 * Retorna a coordenada x do mouse
 -----------------------------------------------------------*/
int get_mouse_x();

/* ---------------------------------------------------------
 * Retorna a coordenada y do mouse
 -----------------------------------------------------------*/
int get_mouse_y();

/* ---------------------------------------------------------
 * Retorna true se houver um clique no mouse (segurar o clique
 * retorna false)
 -----------------------------------------------------------*/
int mouseIsClicked();

/* ----------------------------------------------------------
 * Retorna true se segurar o clique do mouse
 ------------------------------------------------------------*/
int mouseIsPressed();

/* ----------------------------------------------------------
 * Retorna true se soltar o clique do mouse
 ------------------------------------------------------------*/
int mouseIsReleased();

#endif /* CONTROLS_H_ */
