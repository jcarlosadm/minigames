/*
 * window.c
 *
 */

#include "window.h"

/**************************************************
 * Estruturas
 **************************************************/

// estrutura Window_game
typedef struct window_game {
    ALLEGRO_DISPLAY *window; // variável ALLEGRO_DISPLAY

    int exit; // controla se sai do loop principal do jogo

    // controla se deverá redesenhar a janela de jogo em um loop
    // qualquer modificação visual deverá chamar set_redraw_true()
    int redraw;

    // controla a execução da janela de acordo com os frames por segundo configurados
    int tick;

    int current_window;

    ALLEGRO_EVENT_QUEUE *event_queue_window;
}Window_game;

/***************************************************
 * Construtores
 **************************************************/

// aloca memória para objeto global window, do tipo Window_game
void create_window_game(){
    // aloca memória
    window = (Window_game*)malloc(sizeof(Window_game));

    // se a alocação falhar, sai da função
    if(window == NULL){
        fprintf(stderr, "falha ao alocar objeto Window_Game\n");
        return;
    }

    // configura variáveis de controle
    window->exit = false;
    window->redraw = false;
    window->tick = true;
    window->current_window = WINDOW_TITLE_MAIN;

    // tenta alocar memória para o membro ALLEGRO_DISPLAY, de window
    window->window = al_create_display(WINDOW_WIDTH,WINDOW_HEIGHT);
    // se falhar, desaloca window (tipo Window_game) e sai da função
    if(!(window->window)){
        fprintf(stderr, "falha ao criar janela\n");
        free(window);
        window=NULL;
        return;
    }

    // sai da função com sucesso
    fprintf(stderr, "janela criada com sucesso\n");
    return;
}

/**************************************************
 * Destrutores
 **************************************************/

// desaloca window
void desaloc_window(){
    al_destroy_display(window->window);
    if(window->event_queue_window) al_destroy_event_queue(window->event_queue_window);
    free(window);
    puts("desalocado window");
}

/**************************************************
 * Funções de window
 **************************************************/

// pega valor de exit da janela
int get_window_exit_value(){
    return window->exit;
}

// pega valor de redraw da janela
int get_window_redraw_value(){
    return window->redraw;
}

// pega valor de tick da janela
int get_window_tick(){
    return window->tick;
}

// configura exit da janela para true
void set_window_exit_true(){
    window->exit = true;
}

// permite redesenho da janela
void set_window_redraw(int value){
    if(value)
        window->redraw = true;
    else
        window->redraw = false;
}

// configura valor de tick da janela
void set_window_tick(int value){
    if(value)
        window->tick = true;
    else
        window->tick = false;
}

// torna window o local atual de desenho
void set_draw_current_window_game(){
    al_set_target_bitmap(al_get_backbuffer(window->window));
}

int set_mouse_cursor_window(){
    if (al_set_system_mouse_cursor(window->window, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
        return true;
    else
        return false;
}

void set_events_window(ALLEGRO_EVENT_QUEUE* event_queue){
    al_register_event_source(event_queue, al_get_display_event_source(window->window));
}

ALLEGRO_DISPLAY* get_display_window(){
    return window->window;
}

int generate_event_queue_window(){
    window->event_queue_window = al_create_event_queue();
    if(!window->event_queue_window) {
        fprintf(stderr, "falha ao criar evento de janela\n");
        return false;
    }else{
        return true;
    }
}

void register_event_queue_window(){
    al_register_event_source(window->event_queue_window, al_get_display_event_source(window->window));
}

void verifying_event_queue_window(){
    ALLEGRO_EVENT event;
    while (!al_is_event_queue_empty(window->event_queue_window)){

        // pega um evento da lista de eventos de tempo e põe em event
        al_wait_for_event(window->event_queue_window, &event);

        // se o evento for de timer
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            // configura para que a janela possa ser executada
            set_window_exit_true();
        }
    }
}
