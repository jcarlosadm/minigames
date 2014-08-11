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
int new_player_ship(const char *subtype,Window_game** window,Atlas_game** atlas);

/*************************************************
 * Destrutores - desalocação de memória
 *************************************************/

void dealloc_ships_objects();

/*************************************************
 * Funções comuns para todas as naves
 *************************************************/

void draw_ships_objects();

void update_ships_objects(Window_game** window,Atlas_game** atlas);

#endif /* SHIPS_H_ */
