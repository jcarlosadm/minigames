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

/*
 * Inicialização principal
 * Inicialização do allegro, criação da janela
 * Se sucesso, retorna 1
 * Caso contrário, retorna 0
 */
int init();

/*
 * Desalocação de memória dos objetos de jogo
 * Desaloca window
 * Desaloca player
 *
 * Parâmetros:
 * player : ponteiro para Player_ship
 */
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

    // se falhar na inicialização, finaliza programa
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

    // aloca memória para o objeto Player_ship
    player = new_player_ship(6, 7,400,200,32,32);
    // se falhar, finaliza programa
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

    // desaloca memória
    finish_game(player);

    // fecha programa com sucesso
    return EXIT_SUCCESS;
}

/*
 * Inicialização principal
 */
int init(){
    // guarda true se sucesso e false se fracasso
    int success = true;

    // ponteiro para janela global começa com NULL
    window = NULL;

    // tenta inicializar o allegro
    // se falhar, success recebe false
    if(!al_init()){
        fprintf(stderr, "falha ao iniciar o allegro\n");
        success = false;
    }

    // tenta criar janela
    // se falhar, success recebe false
    create_window_game();
    if(!window){
        fprintf(stderr, "falha ao criar janela\n");
        success = false;
    }

    // se não for bem sucedido em alguma inicialização, desaloca o que foi alocado
    if(!success){
        if(!window) desaloc_window();
    }

    // retorna true ou false
    return success;
}

/*
 * Desaloca o que foi alocado
 */
void finish_game(Player_ship* player){

    // desaloca player
    if(!player) desaloc_player_ship(player);

    // desaloca janela
    if(!window) desaloc_window();
}
