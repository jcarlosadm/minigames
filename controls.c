/*
 * controls.c
 *
 */

#include "controls.h"

typedef struct event_queue_controls{
    ALLEGRO_EVENT_QUEUE *event_queue;
}Event_queue_controls;

typedef struct mouse_game{
    int previous_state;
    int current_state;

    int move_state;
    int position_x;
    int position_y;
}Mouse_game;

Event_queue_controls* event_queue;

int start_mouse(){

    if (!al_install_mouse()){
        fprintf(stderr, "Falha ao inicializar o mouse\n");
        return false;
    }

    // tenta atribuir o cursor padrão do sistema para ser usado
    if (!set_mouse_cursor_window())
        return false;

    mouse = (Mouse_game*)malloc(sizeof(Mouse_game));
    if(!mouse){
        puts("falha ao alocar mouse");
        return false;
    }

    mouse->previous_state = false;
    mouse->current_state = false;
    mouse->move_state = false;
    mouse->position_x = 0;
    mouse->position_y = 0;

    return true;
}

int start_event_queue_controls(){
    event_queue = (Event_queue_controls*)malloc(sizeof(Event_queue_controls));
    event_queue->event_queue = al_create_event_queue();
    if(!(event_queue->event_queue)) {
        fprintf(stderr, "falha ao criar lista de eventos de controles\n");
        free(event_queue);
        return false;
    }
    return true;
}

void desaloc_mouse(){
    if(mouse){
        free(mouse);
        puts("mouse desalocado");
    }

}

void desaloc_event_queue_controls(){
    if(event_queue){
        al_destroy_event_queue(event_queue->event_queue);
        free(event_queue);
        puts("lista de eventos de controle desalocada");
    }

}

void check_event_queue_controls(){
    ALLEGRO_EVENT event;
    while (!al_is_event_queue_empty(event_queue->event_queue)){

        // pega um evento da lista e põe em event
        al_wait_for_event(event_queue->event_queue, &event);

        // verifica qual o evento capturado

        // se o evento for o movimento do mouse
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES){
            set_mouse_move_state(true);
            set_mouse_coords(event.mouse.x, event.mouse.y);
        }
        // se o evento for o clique do mouse
        else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            set_mouse_current_state(true);
        }
        else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
            set_mouse_current_state(false);
        }

    }
}

void update_controls(){
    mouse->previous_state = mouse->current_state;
    mouse->current_state = false;
    mouse->move_state = false;
}

void set_mouse_current_state(int state){
    mouse->current_state = state;
}

int get_mouse_move_state(){
    return mouse->move_state;
}

void set_mouse_move_state(int state){
    mouse->move_state = state;
}

int get_mouse_x(){
    return mouse->position_x;
}

int get_mouse_y(){
    return mouse->position_y;
}

void set_mouse_coords(int position_x, int position_y){
    mouse->position_x = position_x;
    mouse->position_y = position_y;
}

void register_event_queue_controls(){
    al_register_event_source(event_queue->event_queue, al_get_mouse_event_source());
}
