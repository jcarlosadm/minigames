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
#include "controls.h"

/*
 * Inicialização principal
 * Inicialização do allegro, criação do timer, criação da janela, lista de eventos normais e de tempo
 * Se sucesso, retorna 1
 * Caso contrário, retorna 0
 */
int init_(ALLEGRO_TIMER **timer, ALLEGRO_EVENT_QUEUE **event_queue_time);

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
void finish_game(Player_ship** player,ALLEGRO_TIMER **timer,
        ALLEGRO_EVENT_QUEUE **event_queue_time);


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

    /*
     * Objetos do allegro
     */

    // timer
    ALLEGRO_TIMER *timer = NULL;
    // lista de eventos de tempo
    ALLEGRO_EVENT_QUEUE *event_queue_time = NULL;
    // pega evento único
    ALLEGRO_EVENT event;

    /*******************************
     * Inicialização
     *******************************/

    /*
     * Inicializações do Allegro
     */

    // se falhar na inicialização, finaliza programa
    if(!init_(&timer,&event_queue_time)){
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
    player = new_player_ship(6, 7,get_mouse_x()-16,get_mouse_y()-16,32,32);
    // se falhar, finaliza programa
    if(player == NULL){
        finish_game(&player, &timer,&event_queue_time);
        exit(EXIT_FAILURE);
    }

    /************************************
     * Processo principal
     ************************************/

    // apenas configura a cor branca e torna visível
    al_clear_to_color(al_map_rgb_f(0,0,0));
    al_flip_display();

    // inicia timer
    al_start_timer(timer);

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

            // configura redraw de window para false
            set_window_redraw(false);
            // configura para que a janela não possa mais ser executada até o próximo fps
            set_window_tick(false);

            update_controls();

        // se a janela não puder ser executada, verifica timer para o próximo loop
        }else{

            // enquanto a lista de eventos de tempo não estiver vazia....
            while (!(al_is_event_queue_empty(event_queue_time))){

                // pega um evento da lista de eventos de tempo e põe em event
                al_wait_for_event(event_queue_time, &event);

                // se o evento for de timer
                if(event.type == ALLEGRO_EVENT_TIMER){
                    // configura para que a janela possa ser executada
                    set_window_tick(true);
                }
            }

        }

    }

    /************************************
     * Finalização
     ************************************/

    // desaloca memória
    finish_game(&player, &timer,&event_queue_time);

    // fecha programa com sucesso
    return EXIT_SUCCESS;
}

/*
 * Inicialização principal
 */
int init_(ALLEGRO_TIMER **timer, ALLEGRO_EVENT_QUEUE **event_queue_time){
    // guarda true se sucesso e false se fracasso
    int success = true;

    // tenta inicializar o allegro
    // se falhar, success recebe false
    if(!al_init()){
        fprintf(stderr, "falha ao iniciar o allegro\n");
        success = false;
    }

    *timer = (ALLEGRO_TIMER*) al_create_timer(1.0 / FPS_GAME);
    if(!*timer) {
        fprintf(stderr, "falha ao iniciar o timer\n");
        success = false;
    }

    // tenta criar janela
    if(!create_window_game()) success = false;

    // tenta criar lista de eventos de controles
    if(!start_event_queue_controls()) success = false;

    // tenta criar lista de eventos de tempo
    *event_queue_time = al_create_event_queue();
    if(!*event_queue_time) {
        fprintf(stderr, "falha ao criar lista de eventos de tempo\n");
        success = false;
    }

    // tenta criar lista de eventos da janela
    if(!generate_event_queue_window()) success = false;

    // tenta inicializar o mouse
    if(!start_mouse()) success = false;

    // se não for bem sucedido em alguma inicialização, desaloca o que foi alocado
    if(!success){
        if(*timer) al_destroy_timer(*timer);
        desaloc_event_queue_controls();
        if(*event_queue_time) al_destroy_event_queue(*event_queue_time);
        desaloc_window();
    }

    // caso seja bem sucedido
    else
    {

        // registra eventos da janela
        register_event_queue_window();
        // registra eventos do timer
        al_register_event_source(*event_queue_time, al_get_timer_event_source(*timer));
        // registra eventos dos controles
        register_event_queue_controls();

    }

    // retorna true ou false
    return success;
}

/*
 * Desaloca o que foi alocado
 */
void finish_game(Player_ship** player, ALLEGRO_TIMER **timer,
        ALLEGRO_EVENT_QUEUE **event_queue_time){

    // desaloca player
    desaloc_player_ship(*player);
    // desaloca timer
    if(*timer) al_destroy_timer(*timer);
    // desaloca lista de eventos de controle
    desaloc_event_queue_controls();
    // desaloca lista de evento de tempo
    if(*event_queue_time) al_destroy_event_queue(*event_queue_time);
    // desaloca janela
    desaloc_mouse();
    desaloc_window();
}
