/*
 * ships.c
 *
 *  Módulo que trata do player e das naves inimigas.
 */

#include "ships.h"

/*******************************************
 * Tipos
 *******************************************/


// Parte da nave que descreve seus atributos
typedef struct attributes{
    int power; // poder de tiro
    float speed; // velocidade
}Attributes;


// Descreve a nave, usada pelo inimigo e como base do player
typedef struct ship{
    Attributes attr; // atributos da nave
}Ship;

// tipo Ship usado pelo player
typedef struct player_ship{
    Ship base;
}Player_ship;

/*******************************************
 * Construtores - alocação de memória
 *******************************************/

// define atributos básicos (usado nos construtores)
void build_Attr(Attributes* attr, int power, int speed){
    attr->power = power;
    attr->speed = speed;
}

// aloca nave inimiga
Ship* new_ship(int power, float speed){
    // aloca nave com malloc
    Ship *ship_ = (Ship*) malloc (sizeof(Ship));
    // define os atributos
    build_Attr(&(ship_->attr),power,speed);
    // retorna ponteiro para a nave
    return ship_;
}

// aloca nave do player
Player_ship* new_player_ship(int power, float speed){
    // aloca nave com malloc
    Player_ship *ship_ = (Player_ship*) malloc (sizeof(Player_ship));
    // define os atributos
    build_Attr(&(ship_->base.attr),power,speed);
    // retorna ponteiro para a nave
    return ship_;
}

/*************************************************
 * Destrutores - desalocação de memória
 *************************************************/

// desaloca nave inimiga
void desaloc_ship(Ship* ship){
    // desaloca com free
    free(ship);
}

// desaloca nave player
void desaloc_player_ship(Player_ship* ship){
    // desaloca com free
    free(ship);
}

/*************************************************
 * Funções comuns das naves
 *************************************************/

// imprime poder de tiro
void printPower(Ship* ship){
    // imprime atributo power
    printf("power: %d\n",ship->attr.power);
}

/**************************************************
 * Funções somente para o player
 *************************************************/

//Pega a nave do jogador e retorna um ponteiro para a sua base
Ship* getBase(Player_ship* ship){
    return &(ship->base);
}
