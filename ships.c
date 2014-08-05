/*
 * ships.c
 *
 *  Módulo que trata do player e das naves inimigas.
 */

#include "ships.h"

/*******************************************
 * Tipos
 *******************************************/

/*
 * Parte da nave que descreve seus atributos
 */
typedef struct attributes{
    int power; // poder de tiro
    float speed; // velocidade
}Attributes;


//Descreve a nave
typedef struct ship{
    Attributes attr; // atributos da nave
}Ship;

/*******************************************
 * Construtores - alocação de memória
 *******************************************/

// aloca nave
Ship* new_ship(int power, float speed){
    // aloca nave com malloc
    Ship *ship_ = (Ship*) malloc (sizeof(Ship));
    // define os atributos
    ship_->attr.power = power;
    ship_->attr.speed = speed;
    // retorna ponteiro para a nave
    return ship_;
}

/*************************************************
 * Destrutores - desalocação de memória
 *************************************************/

// desaloca nave
void desaloc_ship(Ship* ship_){
    // desaloca com free
    free(ship_);
}

/*************************************************
 * Funções comuns das naves
 *************************************************/

// imprime pode de tiro
void printPower(Ship* ship){
    // imprime atributo power
    printf("power: %d\n",ship->attr.power);
}
