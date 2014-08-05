/*
 * ships.h
 *
 *  Módulo que trata do player e das naves inimigas.
 */

#ifndef SHIPS_H_
#define SHIPS_H_

#include <stdio.h>
#include <stdlib.h>

/*******************************************
 * Tipos
 *******************************************/

// nave que é usada por inimigos e que serve como base para o player
typedef struct ship Ship;

// nave usada pelo player
typedef struct player_ship Player_ship;

/*******************************************
 * Construtores - alocação de memória
 *******************************************/

/*
 * Gera nave base, usada pelos inimigos
 * Aloca memória e define os atributos da nave
 *
 * Parâmetros:
 * power : nível de poder de tiro da nave
 * speed : velocidade da nave
 */
Ship* new_ship(int power, float speed);

/*
 * Nave usada pelo Player
 * Aloca memória e define os atributos da nave
 *
 * Parâmetros:
 * power : nível de poder de tiro da nave
 * speed : velocidade da nave
 */
Player_ship* new_player_ship(int power, float speed);

/*************************************************
 * Destrutores - desalocação de memória
 *************************************************/

/*
 * Desaloca nave base/inimiga
 *
 * Parâmetros:
 * ship : nave a ser desalocada
 */

void desaloc_ship(Ship* ship);

/*
 * Desaloca nave player
 *
 * Parâmetros:
 * ship : nave do player
 */
void desaloc_player_ship(Player_ship* ship);

/*************************************************
 * Funções comuns para todas as naves
 *************************************************/

/*
 * Observação: para usar essas funções com a nave do jogador, onde houver
 * o tipo Ship, use a função getBase()
 */

/*
 * Imprime poder de tiro de uma nave
 * Se a nave for do jogador, use a função getBase() no lugar de ship
 *
 * Parâmetros:
 * ship : nave a ser verificado o poder de tiro
 */
void printPower(Ship* ship);

/**************************************************
 * Funções somente para o player
 *************************************************/

/*
 * Pega um ponteiro Player_ship e retorna um ponteiro para Ship
 * Usada nas funções comuns para todas as naves
 *
 * Parâmetros:
 * ship : nave do Player
 */
Ship* getBase(Player_ship* ship);

#endif /* SHIPS_H_ */
