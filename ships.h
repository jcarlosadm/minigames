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

// tipo Ship que descreve uma nave básica
typedef struct ship Ship;


/*******************************************
 * Construtores - alocação de memória
 *******************************************/

/*
 * Gera Ship
 * Aloca memória e define os atributos da nave
 *
 * Parâmetros:
 * power : nível de poder de tiro da nave
 * speed : velocidade da nave
 */
Ship* new_ship(int power, float speed);

/*************************************************
 * Destrutores - desalocação de memória
 *************************************************/

/*
 * Desaloca nave e seus atributos
 *
 * Parâmetros:
 * ship : nave a ser desalocada
 */

void desaloc_ship(Ship* ship);

/*************************************************
 * Funções comuns das naves
 *************************************************/

/*
 * Imprime poder de tiro de uma nave
 *
 * Parâmetros:
 * ship : nave a ser verificado o poder de tiro
 */
void printPower(Ship* ship);

#endif /* SHIPS_H_ */
