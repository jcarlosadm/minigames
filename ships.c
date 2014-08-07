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

// Parte da nave que determina a sua localização
typedef struct location{
    float position_x; // coordenada x
    float position_y; // coordenada y
}Location;

// Parte da nave que determina a sua dimensão
typedef struct dimension{
    float width; // largura
    float height; // altura
}Dimension;

// Descreve a nave, usada pelo inimigo e como base do player
typedef struct ship{
    Attributes attr; // atributos da nave
    Location location; // coordenadas x e y da nave
    Dimension dimension; // largura e altura da nave

    ALLEGRO_BITMAP *image; // imagem da nave
}Ship;

// tipo Ship usado pelo player
typedef struct player_ship{
    Ship* base;
}Player_ship;

/*******************************************
 * Construtores - alocação de memória
 *******************************************/

// define atributos básicos (usado nos construtores)
void build_Attr(Attributes* attr, int power, int speed){
    attr->power = power;
    attr->speed = speed;
}

// inicializa localização e dimensões
void initialize_loc_dim(Location* loc, Dimension* dim, float position_x,
        float position_y, float width, float height){
    loc->position_x = position_x;
    loc->position_y = position_y;
    dim->height = height;
    dim->width = width;
}

// aloca nave inimiga ou base para o player
Ship* new_ship(int power, float speed,float position_x,
        float position_y, float width, float height){
    // aloca nave com malloc
    Ship *ship_ = (Ship*) malloc (sizeof(Ship));

    if(ship_ == NULL){
        fprintf(stderr, "falha ao alocar objeto Ship\n");
        return NULL;
    }

    // define os atributos
    build_Attr(&(ship_->attr),power,speed);
    // define localização e dimensões
    initialize_loc_dim(&(ship_->location),&(ship_->dimension),position_x,
            position_y,width, height);
    // cria uma imagem bitmap
    ship_->image = al_create_bitmap(32,32);

    al_set_target_bitmap(ship_->image);
    al_clear_to_color(al_map_rgb(255, 100, 155));
    set_draw_current_window_game();

    // retorna ponteiro para a nave
    return ship_;
}

// aloca nave do player
Player_ship* new_player_ship(int power, float speed,float position_x,
        float position_y, float width, float height){
    // aloca nave com malloc
    Player_ship *ship = (Player_ship*) malloc (sizeof(Player_ship));

    if(ship == NULL){
        fprintf(stderr, "falha ao alocar objeto Player_ship\n");
        return NULL;
    }

    // constrói a base
    ship->base = new_ship(power, speed,position_x,
            position_y, width, height);

    if(ship->base == NULL){
        fprintf(stderr, "falha ao alocar base de Player_ship\n");
        free(ship);
        return NULL;
    }

    // retorna player
    return ship;
}

/*************************************************
 * Destrutores - desalocação de memória
 *************************************************/

// desaloca nave inimiga
void desaloc_ship(Ship* ship){
    // desaloca imagem
    al_destroy_bitmap(ship->image);
    // desaloca com free
    free(ship);
}

// desaloca nave player
void desaloc_player_ship(Player_ship* ship){
    // desaloca base
    desaloc_ship(ship->base);
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

void draw_ship(Ship* ship){
    al_draw_bitmap(ship->image, ship->location.position_x, ship->location.position_y, 0);
}

/**************************************************
 * Funções somente para o player
 *************************************************/

//Pega a nave do jogador e retorna um ponteiro para a sua base
Ship* getBase(Player_ship* ship){
    return ship->base;
}
