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
 * Inicialização do allegro, criação do timer, criação da janela, lista de eventos normais e de tempo
 * Se sucesso, retorna 1
 * Caso contrário, retorna 0
 */
int init_(ALLEGRO_TIMER **timer, ALLEGRO_EVENT_QUEUE **event_queue,
        ALLEGRO_EVENT_QUEUE **event_queue_time);

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
void finish_game(Player_ship** player,ALLEGRO_TIMER **timer, ALLEGRO_EVENT_QUEUE **event_queue,
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
    // lista de eventos
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
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
    if(!init_(&timer, &event_queue,&event_queue_time)){
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
        finish_game(&player, &timer, &event_queue,&event_queue_time);
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
        verifying_event_queue_window();

        // se a janela puder ser executada no fps configurado
        if(get_window_tick()){

            /*
             * Verifica eventos
             */

            // enquanto a lista de eventos não for vazia
            while (!al_is_event_queue_empty(event_queue)){


                // pega um evento da lista e põe em event
                al_wait_for_event(event_queue, &event);

                // verifica qual o evento capturado

                // se o evento for o movimento do mouse
                if (event.type == ALLEGRO_EVENT_MOUSE_AXES){
                    puts("mov mouse\n");
                }
                // se o evento for o clique do mouse
                else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                    puts("click mouse\n");
                }

            }



            /*
             * Lógica
             */


            /*
             * Desenho
             */

            //printPower(getBase(player));
            draw_ship(getBase(player));
            al_flip_display();

            /*
             * Finalização do loop
             */

            // configura redraw de window para false
            set_window_redraw(false);
            // configura para que a janela não possa mais ser executada até o próximo fps
            set_window_tick(false);

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
    finish_game(&player, &timer, &event_queue,&event_queue_time);

    // fecha programa com sucesso
    return EXIT_SUCCESS;
}

/*
 * Inicialização principal
 */
int init_(ALLEGRO_TIMER **timer, ALLEGRO_EVENT_QUEUE **event_queue,
        ALLEGRO_EVENT_QUEUE **event_queue_time){
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

    *timer = (ALLEGRO_TIMER*) al_create_timer(1.0 / FPS_GAME);
    if(!*timer) {
        fprintf(stderr, "falha ao iniciar o timer\n");
        success = false;
    }

    // tenta criar janela
    // se falhar, success recebe false
    create_window_game();
    if(!window){
        fprintf(stderr, "falha ao criar janela\n");
        success = false;
    }

    // tenta criar lista de eventos
    *event_queue = al_create_event_queue();
    if(!*event_queue) {
        fprintf(stderr, "falha ao criar lista de eventos\n");
        success = false;
    }

    // tenta criar lista de eventos de tempo
    *event_queue_time = al_create_event_queue();
    if(!*event_queue_time) {
        fprintf(stderr, "falha ao criar lista de eventos de tempo\n");
        success = false;
    }

    // tenta criar lista de eventos da janela
    if(!generate_event_queue_window()){
        fprintf(stderr, "falha ao criar lista de eventos da janela\n");
        success = false;
    }

    // tenta inicializar o mouse
    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse\n");
        success = false;
    }

    // tenta atribuir o cursor padrão do sistema para ser usado
    if (!set_mouse_cursor_window())
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        success = false;
    }

    // se não for bem sucedido em alguma inicialização, desaloca o que foi alocado
    if(!success){
        if(*timer) al_destroy_timer(*timer);
        if(*event_queue) al_destroy_event_queue(*event_queue);
        if(*event_queue_time) al_destroy_event_queue(*event_queue_time);
        if(window) desaloc_window();
    }

    // caso seja bem sucedido
    else
    {

        // registra eventos da janela
        register_event_queue_window();
        // registra eventos do timer
        //al_register_event_source(event_queue, al_get_timer_event_source(timer));
        al_register_event_source(*event_queue_time, al_get_timer_event_source(*timer));
        // registra eventos do mouse
        al_register_event_source(*event_queue, al_get_mouse_event_source());

    }

    // retorna true ou false
    return success;
}

/*
 * Desaloca o que foi alocado
 */
void finish_game(Player_ship** player, ALLEGRO_TIMER **timer, ALLEGRO_EVENT_QUEUE **event_queue,
        ALLEGRO_EVENT_QUEUE **event_queue_time){

    // desaloca player
    if(*player) desaloc_player_ship(*player);
    // desaloca timer
    if(*timer) al_destroy_timer(*timer);
    // desaloca lista de eventos
    if(*event_queue) al_destroy_event_queue(*event_queue);
    // desaloca lista de evento de tempo
    if(*event_queue_time) al_destroy_event_queue(*event_queue_time);
    // desaloca janela
    if(window) desaloc_window();
}
