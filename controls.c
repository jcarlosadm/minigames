/*
 * controls.c
 * Módulo que gerencia os controles
 *
 */

#include "controls.h"

/***************************************************************
 * *************************************************************
 * Estruturas
 * *************************************************************
 ***************************************************************/

/*------------------------------------------------------------
 * Estrutura da lista de eventos dos controles
 * -----------------------------------------------------------
 * Membros:
 * ALLEGRO_EVENT_QUEUE *event_queue : lista de eventos dos
 *                                    controles
 -------------------------------------------------------------*/
typedef struct event_queue_controls{
    // objetos allegro
    ALLEGRO_EVENT_QUEUE *event_queue;
}Event_queue_controls;

/* ------------------------------------------------------------
 * Estrutura do mouse do jogo
 * ------------------------------------------------------------
 * Membros:
 * int previous_state : estado do mouse no loop anterior (true/false)
 * int current_state : estado do mouse no loop atual (true/false)
 *
 * int move_state : se o mouse se moveu (true/false)
 * int position_x : coordenada x do mouse
 * int position_y : coordenada y do mouse
 --------------------------------------------------------------*/
typedef struct mouse_game{
    int previous_state;
    int current_state;

    int move_state;
    int position_x;
    int position_y;
}Mouse_game;

/*********************************************************************
 * *******************************************************************
 * Objetos do módulo
 * *******************************************************************
 *********************************************************************/

// mouse
Mouse_game* mouse;

// lista de eventos de controles
Event_queue_controls* event_queue;

/*********************************************************************
 * *******************************************************************
 * Construtores
 * *******************************************************************
 *********************************************************************/

// protótipos necessários
void register_event_queue_controls();

/* ------------------------------------------------------------------
 * Inicia mouse
 * ------------------------------------------------------------------
 * Retorna false se não conseguir inicializar ou alocar memória
 * para o objeto mouse
 --------------------------------------------------------------------*/
int start_mouse(){

    // se não conseguir instalar mouse, informa e sai com false
    if (!al_install_mouse()){
        fprintf(stderr, "Falha ao inicializar o mouse\n");
        return false;
    }

    // tenta atribuir o cursor padrão do sistema para ser usado
    // (função do módulo window)
    if (!set_mouse_cursor_window())
        return false;

    // aloca objeto Mouse_game
    mouse = (Mouse_game*)malloc(sizeof(Mouse_game));
    // se não conseguir, retorna false
    if(!mouse){
        puts("falha ao alocar mouse");
        return false;
    }

    /*
     * configura variáveis membro de mouse
     */
    // estado anterior
    mouse->previous_state = false;
    // estado atual
    mouse->current_state = false;
    // estado de movimento
    mouse->move_state = false;
    // posição x do mouse
    mouse->position_x = 0;
    // posição y do mouse
    mouse->position_y = 0;

    // conseguiu executar operações com sucesso
    return true;
}

/* ---------------------------------------------------------------------------
 * Inicia lista de eventos de controle
 * ---------------------------------------------------------------------------
 * Se não conseguir, retorna false
 -----------------------------------------------------------------------------*/
int start_event_queue_controls(){
    // tenta alocar objeto Event_queue_controls
    event_queue = (Event_queue_controls*)malloc(sizeof(Event_queue_controls));
    // se não conseguir sai com false
    if(!event_queue){
        fprintf(stderr, "falha ao criar objeto Event_queue_controls\n");
        return false;
    }

    // tenta alocar objeto ALLEGRO_EVENT_QUEUE
    event_queue->event_queue = al_create_event_queue();
    // se não conseguir, desaloca event_queue e sai com false
    if(!(event_queue->event_queue)) {
        fprintf(stderr, "falha ao criar lista de eventos de controles\n");
        free(event_queue);
        return false;
    }

    // operações bem sucedidas
    return true;
}

/* --------------------------------------------------------------
 * Inicia controles
 * --------------------------------------------------------------
 * Essa função inicia todos os sistemas de controles.
 *
 * Inicia mouse e lista de eventos de controle
 ----------------------------------------------------------------*/
int start_controls(){

    // inicia mouse
    if(!start_mouse()) return false;
    // inicia lista de eventos de controles
    if(!start_event_queue_controls()) return false;

    // registra controles na lista de eventos de controles
    register_event_queue_controls();

    // operações bem sucedidas
    return true;
}

/* -------------------------------------------------
 * Desaloca mouse
 ---------------------------------------------------*/
void dealloc_mouse(){
    // se mouse estiver alocado...
    if(mouse){
        // desaloca mouse
        free(mouse);
        mouse = NULL;
        // informa
        puts("mouse desalocado");
    }

}

/* --------------------------------------------------------
 * Desaloca lista de eventos de controles
 ----------------------------------------------------------*/
void dealloc_event_queue_controls(){
    // se a lista de eventos de controles estiver alocada...
    if(event_queue){
        // desaloca objeto ALLEGRO_EVENT_QUEUE
        al_destroy_event_queue(event_queue->event_queue);
        // desaloca objeto Event_queue_controls
        free(event_queue);
        // informa
        puts("lista de eventos de controle desalocada");
    }
}

/* ----------------------------------------------------------
 * Desaloca todos os sistemas de controles de uma vez
 * ----------------------------------------------------------
 * Desaloca mouse e lista de eventos de controles
 ------------------------------------------------------------*/
void dealloc_controls(){
    // desaloca mouse
    dealloc_mouse();
    // desaloca lista de eventos de controles
    dealloc_event_queue_controls();
}

/*************************************************************************
 * ***********************************************************************
 * Funções de controls
 * ***********************************************************************
 *************************************************************************/

/* ---------------------------------------------------------------
 * Verifica lista de eventos de controles
 * ---------------------------------------------------------------
 * Coloque essa função dentro do loop interno ao loop principal
 * do game, logo no início. Ela irá verificar a ocorrência de eventos
 * de controle e executará códigos de acordo com essa ocorrência.
 -----------------------------------------------------------------*/
void check_event_queue_controls(){

    // retém evento da lista de eventos
    ALLEGRO_EVENT event;

    // enquanto a lista de eventos não estiver vazia...
    while (!al_is_event_queue_empty(event_queue->event_queue)){

        // pega um evento da lista e põe em event
        al_wait_for_event(event_queue->event_queue, &event);

        // verifica qual o evento capturado

        // se o evento for o movimento do mouse...
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES){
            // configura o estado de movimento para true
            mouse->move_state = true;
            // muda as coordenadas x e y
            mouse->position_x = event.mouse.x;
            mouse->position_y = event.mouse.y;
        }

        // se o evento for o clique do mouse...
        else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            // muda o estado atual do mouse para true
            mouse->current_state = true;
        }

        // se o evento for soltar o clique do mouse...
        else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
            // muda o estado atual do mouse para false
            mouse->current_state = false;
        }
    }
}

/* -----------------------------------------------------
 * Atualiza controles
 * ----------------------------------------------------
 * Coloque no final do loop interno ao loop principal
 * do jogo
 ------------------------------------------------------*/
void update_controls(){
    // atualiza estado anterior do mouse
    mouse->previous_state = mouse->current_state;
    // atualiza estado atual do mouse
    mouse->move_state = false;
}

/* ----------------------------------------------------
 * Pega valor (true ou false) do estado de movimento
 * do mouse
 ------------------------------------------------------*/
int get_mouse_move_state(){
    return mouse->move_state;
}

/* ----------------------------------------------------
 * Pega coordenada x do mouse
 ------------------------------------------------------*/
int get_mouse_x(){
    return mouse->position_x;
}

/* ----------------------------------------------------
 * Pega coordenada y do mouse
 ------------------------------------------------------*/
int get_mouse_y(){
    return mouse->position_y;
}

/* ---------------------------------------------------------------------------------
 * Registra lista de eventos de controles
 * ---------------------------------------------------------------------------------
 * Registra eventos do mouse
 -----------------------------------------------------------------------------------*/
void register_event_queue_controls(){
    al_register_event_source(event_queue->event_queue, al_get_mouse_event_source());
}

/* ---------------------------------------------------------
 * Verifica se houve clique único (não segurou)
 -----------------------------------------------------------*/
int mouseIsClicked(){
    // se o estado anterior é false e o atual é true, houve clique único
    if(!(mouse->previous_state) && mouse->current_state)
        return true;
    else
        return false;
}

/* ---------------------------------------------------------
 * Verifica se segurou o mouse
 -----------------------------------------------------------*/
int mouseIsPressed(){
    // se o estado anterior é true e o atual é true, o mouse
    // está sendo segurado
    if(mouse->previous_state && mouse->current_state)
        return true;
    else
        return false;
}

/* ------------------------------------------------------------
 * Verifica se soltou o clique do mouse
 --------------------------------------------------------------*/
int mouseIsReleased(){
    // se o estado anterior é true e o atual é false,
    // o clique do mouse foi solto
    if(mouse->previous_state && !(mouse->current_state))
        return true;
    else
        return false;
}
