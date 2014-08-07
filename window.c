/*
 * window.c
 *
 */

#include "window.h"

typedef struct window_game {
    ALLEGRO_DISPLAY *window;
}Window_game;


void create_window_game(){
    window = (Window_game*)malloc(sizeof(Window_game));

    if(window == NULL){
        fprintf(stderr, "falha ao alocar objeto Window_Game\n");
        return;
    }

    window->window = al_create_display(640,480);
    if(!(window->window)){
        fprintf(stderr, "falha ao criar janela\n");
        free(window);
        window=NULL;
        return;
    }
    fprintf(stderr, "janela criada com sucesso\n");
    return;
}

void desaloc_window(){
    al_destroy_display(window->window);
    free(window);
}

void set_draw_current_window_game(){
    al_set_target_bitmap(al_get_backbuffer(window->window));
}
