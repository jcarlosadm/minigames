/*
 * main.c
 *
 */

#include <stdio.h>
#include <allegro5/allegro.h>
#include "ships.h"

int main()
{
    // ponteiro para janela
    ALLEGRO_DISPLAY *window = NULL;

    if(!al_init()){
        fprintf(stderr, "falha ao iniciar o allegro\n");
        return -1;
    }

    window = al_create_display(640,480);
    if(!window){
        fprintf(stderr, "falha ao criar janela\n");
        return -1;
    }

    al_clear_to_color(al_map_rgb_f(255,255,255));
    al_flip_display();

    Player_ship *player = new_player_ship(6, 7);
    printPower(getBase(player));
    desaloc_player_ship(player);

    al_rest(10.0);
    al_destroy_display(window);

    return 0;
}
