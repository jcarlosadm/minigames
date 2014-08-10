/*
 * main.c
 *
 */

// bibliotecas básicas
#include <stdio.h>
#include <allegro5/allegro.h>
// módulos do jogo
#include "graphics.h"
#include "window.h"
#include "ships.h"
#include "controls.h"

/*
 * Inicialização principal
 * Inicialização do allegro, criação do timer, criação da janela, lista de eventos normais e de tempo
 * Se sucesso, retorna 1
 * Caso contrário, retorna 0
 */
int init_();

/*
 * Desalocação de memória dos objetos de jogo
 * Desaloca window
 * Desaloca player
 *
 * Parâmetros:
 * player : ponteiro para Player_ship
 * timer : objeto ALLEGRO_TIMER do allegro
 * event_queue : objeto ALLEGRO_EVENT_QUEUE do allegro
 * event_queue_time : objeto ALLEGRO_EVENT_QUEUE do allegro
 */
void finish_game(Player_ship** player);


int main()
{
    /*******************************
     * variáveis locais
     *******************************/

    /*
     * objetos de jogo
     */

    // jogador
    Player_ship *player = NULL;

    /*******************************
     * Inicialização
     *******************************/

    /*
     * Inicializações do Allegro
     */

    // se falhar na inicialização, finaliza programa
    if(!init_()){
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
    player = new_player_ship("player","blue");
    // se falhar, finaliza programa
    if(player == NULL){
        finish_game(&player);
        exit(EXIT_FAILURE);
    }

    /************************************
     * Processo principal
     ************************************/

    // apenas configura a cor branca e torna visível
    al_clear_to_color(al_map_rgb_f(0,0,0));
    al_flip_display();

    // inicia timer
    start_timer();

    while(!get_window_exit_value()){

        // verifica se algum evento da janela ocorre
        check_event_queue_window();

        // se a janela puder ser executada no fps configurado
        if(get_window_tick()){

            /*
             * Lógica
             */

            check_event_queue_controls();
            update_player(player);

            /*
             * Desenho
             */

            //printPower(getBase(player));
            al_clear_to_color(al_map_rgb_f(0,0,0));
            draw_ship(getBase(player));
            al_flip_display();

            /*
             * Finalização do loop
             */

            // configura para que a janela não possa mais ser executada até o próximo fps
            set_window_tick_false();

            update_controls();

        // se a janela não puder ser executada, verifica timer para o próximo loop
        }

    }

    /************************************
     * Finalização
     ************************************/

    // desaloca memória
    finish_game(&player);

    // fecha programa com sucesso
    return EXIT_SUCCESS;
}

/*
 * Inicialização principal
 */
int init_(){
    // guarda true se sucesso e false se fracasso
    int success = true;

    // tenta inicializar o allegro
    if(!al_init()){
        fprintf(stderr, "falha ao iniciar o allegro\n");
        success = false;
    }

    al_init_image_addon();

    // tenta criar janela
    if(!create_window_game()) success = false;

    // tenta criar lista de eventos de controles
    if(!start_controls()) success = false;

    if(!create_atlas()) success = false;

    // se não for bem sucedido em alguma inicialização, desaloca o que foi alocado
    if(!success){
        dealloc_controls();
        dealloc_window();
        dealloc_atlas();
        return success;
    }

    // retorna true ou false
    return success;
}

/*
 * Desaloca o que foi alocado
 */
void finish_game(Player_ship** player){

    // desaloca player
    dealloc_player_ship(*player);
    // desaloca controles
    dealloc_controls();
    dealloc_atlas();
    dealloc_window();

}
