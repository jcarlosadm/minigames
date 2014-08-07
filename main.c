/*
 * main.c
 *
 */

// bibliotecas básicas
#include <stdio.h>
#include <allegro5/allegro.h>
// módulos do jogo
#include "window.h"
#include "ships.h"

// inicialização principal
int init();

// desalocação de memória dos objetos de jogo
void finish_game(Player_ship* player);

int main()
{
    /*******************************
     * variáveis locais
     *******************************/

    /*
     * objetos de jogo
     */

    // jogador
    Player_ship *player;

    /*******************************
     * Inicialização
     *******************************/

    /*
     * Inicializações do Allegro
     */

    if(!init()){
        fprintf(stderr, "falha ao inicializar\n");
        exit(EXIT_FAILURE);
    }

    /************************************
     * Alocações de objetos do jogo
     ************************************/

    /*
     * Alocação do player
     */

    player = new_player_ship(6, 7,400,200,32,32);
    if(player == NULL){
        finish_game(player);
        exit(EXIT_FAILURE);
    }

    /************************************
     * Processo principal
     ************************************/

    al_clear_to_color(al_map_rgb_f(0,255,255));

    printPower(getBase(player));
    draw_ship(getBase(player));

    al_flip_display();
    al_rest(10.0);

    /************************************
     * Finalização
     ************************************/

    finish_game(player);

    return EXIT_SUCCESS;
}

int init(){
    // guarda true se sucesso e false se fracasso
    int success = true;

    // ponteiro para janela global começa com NULL
    window = NULL;

    if(!al_init()){
        fprintf(stderr, "falha ao iniciar o allegro\n");
        success = false;
    }

    create_window_game();
    if(!window){
        fprintf(stderr, "falha ao criar janela\n");
        success = false;
    }

    if(!success){
        if(!window) desaloc_window();
    }

    return success;
}

void finish_game(Player_ship* player){

    if(!player) desaloc_player_ship(player);

    if(!window) desaloc_window();
}
