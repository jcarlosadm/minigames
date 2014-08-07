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

    // tenta alocar memória para o membro ALLEGRO_DISPLAY, de window
    window->window = al_create_display(640,480);
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
    free(window);
}

/**************************************************
 * Funções de window
 **************************************************/

// torna window o local atual de desenho
void set_draw_current_window_game(){
    al_set_target_bitmap(al_get_backbuffer(window->window));
}
