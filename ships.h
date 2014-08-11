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
 ----------------------------------------------------------*/
typedef struct player_ship Player_ship;

typedef struct bullet Bullet;

typedef struct bullets Bullets;


/****************************************************
 * Construtores
 ****************************************************/


/*
 * Nave usada pelo Player
 * Aloca memória e define os atributos da nave
 *
 * Parâmetros:
 * power : nível de poder de tiro da nave
 * speed : velocidade da nave
 */
int new_player_ship(Player_ship** player, const char *subtype,Window_game** window,Graphics_game** graphics);

int makeListofBullets(Bullets** bullets);
/*************************************************
 * Destrutores - desalocação de memória
 *************************************************/

void dealloc_ships_objects(Player_ship** player, Bullets** bullets);

/*************************************************
 * Funções comuns para todas as naves
 *************************************************/

void draw_ships_objects(Player_ship** player, Bullets** bullets);

void update_ships_objects(Player_ship** player,Bullets** bullets, Window_game** window,
        Graphics_game** graphics, Controls_game** controls);

#endif /* SHIPS_H_ */
