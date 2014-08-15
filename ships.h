/*
 * ships.h
 *
 *  Módulo que trata do player e das naves inimigas.
 */

#ifndef SHIPS_H_
#define SHIPS_H_

// Bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// allegro
#include <allegro5/allegro.h>
// mini xml
#include <mxml.h>
// módulo do jogo
#include "graphics.h"
#include "window.h"
#include "controls.h"


/********************************************************
 * ******************************************************
 * Constantes
 * ******************************************************
 ********************************************************/

/* ------------------------------------------------------
 * Localização de informações dos objetos do jogo
 --------------------------------------------------------*/
// player ship
#define XML_PLAYER "info/player.xml"

// enemy ship
#define XML_ENEMIES "info/enemies.xml"

// bullets
#define XML_BULLETS "info/bullets.xml"


/****************************************************
 * **************************************************
 * Estruturas
 * **************************************************
 ****************************************************/

/* --------------------------------------------------------
 * Nave do jogador
 * --------------------------------------------------------
 * Possui informações de dimensão e localização, tipo (player),
 * subtipo, subtipo do tiro, velocidade e imagens.
 ----------------------------------------------------------*/
typedef struct player_ship Player_ship;

/* ----------------------------------------------------------
 * Estrutura de um único tiro
 * ----------------------------------------------------------
 * tipo (player, enemy), subtipo, poder, velocidade, localização
 * e imagens.
 ------------------------------------------------------------*/
typedef struct bullet Bullet;

/* -----------------------------------------------------------
 * Estrutura de uma fila de tiros
 -------------------------------------------------------------*/
typedef struct bullets Bullets;


/****************************************************
 * **************************************************
 * Construtores
 * **************************************************
 ****************************************************/


/* ---------------------------------------------------------------------------------
 * Nave usada pelo Player
 * Aloca memória e define os atributos da nave
 * ---------------------------------------------------------------------------------
 * Parâmetros:
 * Player_ship** player : ponteiro** para Player_ship
 * const char *subtype : subtipo da nave do player
 * Window_game** window : ponteiro** para a janela do jogo
 * Graphics_game** graphics : ponteiro** para os gráficos do jogo
 * ---------------------------------------------------------------------------------
 * Subtipos disponíveis atualmente : blue, red.
 -----------------------------------------------------------------------------------*/
int new_player_ship(Player_ship** player, const char *subtype, Window_game** window,
        Graphics_game** graphics);

/* -------------------------------------------------------------
 * Aloca a lista de balas
 * -------------------------------------------------------------
 * Parâmetros:
 * Bullets** bullets : lista de balas
 ---------------------------------------------------------------*/
int makeListofBullets(Bullets** bullets);


/*************************************************
 * ***********************************************
 * Destrutores - desalocação de memória
 * ***********************************************
 *************************************************/

/* ---------------------------------------------------------------------
 * Desaloca todos os objetos do módulo
 * ---------------------------------------------------------------------
 * Desaloca objeto Player_ship e objeto Bullets (que contém objetos
 * Bullet)
 * ---------------------------------------------------------------------
 * Parâmetros:
 * Player_ship** player_ship : ponteiro** para a nave do jogador
 * Bullets** bullets : ponteiro** para todas as balas alocadas
 -----------------------------------------------------------------------*/
void dealloc_ships_objects(Player_ship** player, Bullets** bullets);

/*************************************************
 * ***********************************************
 * Funções comuns para todas as naves
 * ***********************************************
 *************************************************/

/* ---------------------------------------------------------------------------------------
 * Desenha todos os objetos do módulo
 * ---------------------------------------------------------------------------------------
 * Parâmetros:
 * Player_ship** player : ponteiro** para a nave do jogador
 * Bullets** bullets : ponteiro** para a lista de balas do jogo
 -----------------------------------------------------------------------------------------*/
void draw_ships_objects(Player_ship** player, Bullets** bullets);

/* ----------------------------------------------------------------------------------------
 * Atualiza todos os objetos do módulo
 * ----------------------------------------------------------------------------------------
 * Parâmetros:
 * Player_ship** player: ponteiro** para o player
 * Bullets** bullets : ponteiro** para as balas
 * Window_game** window : ponteiro** para a janela de jogo
 * Graphics_game** graphics : ponteiro para os gráficos do jogo
 * Controls_game** controls : ponteiro para os controles
 ------------------------------------------------------------------------------------------*/
void update_ships_objects(Player_ship** player,Bullets** bullets, Window_game** window,
        Graphics_game** graphics, Controls_game** controls);

#endif /* SHIPS_H_ */
