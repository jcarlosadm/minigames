/*
 * window.c
 * Módulo que gerencia a janela
 *
 */

#include "window.h"

/**************************************************
 * ************************************************
 * Estruturas
 * ************************************************
 **************************************************/

/* ------------------------------------------------------------------
 * Estrutura Timer_game
 * ------------------------------------------------------------------
 * Membros:
 * ALLEGRO_TIMER *timer : objeto allegro de controle de tempo
 * ALLEGRO_EVENT_QUEUE *event_queue_time : objeto allegro de evento que
 *                                         verifica alterações no timer
 --------------------------------------------------------------------*/
typedef struct timer_game{
    // objetos allegro
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *event_queue_time;
}Timer_game;


/* --------------------------------------------------------------------------------------
 * Estrutura Window_game
 * --------------------------------------------------------------------------------------
 * Membros:
 * ALLEGRO_DISPLAY *window : A janela propriamente dita
 * ALLEGRO_EVENT_QUEUE *event_queue_window : guarda objeto ALLEGRO_EVENT_QUEUE do allegro
 * Timer_game *timer : objeto Timer_game
 *
 * int exit : controla se sai do loop principal do jogo (true or false)
 * int tick : controla a execução do loop principal de acordo com os frames
 *            por segundo configurados (true or false)
 * int current_window : controla a tela atual (recebe um valor que representa uma janela)
 ----------------------------------------------------------------------------------------*/
typedef struct window_game {
    // Objetos do Allegro
    ALLEGRO_DISPLAY *window;
    ALLEGRO_EVENT_QUEUE *event_queue_window;
    Timer_game* timer;

    // flags da janela
    int exit;
    int tick;
    int current_window;
}Window_game;

/**************************************************
 * ************************************************
 * Construtores
 * ************************************************
 **************************************************/

// protótipos necessários (veja essas funções mais adiante)
void register_event_queue_window(Window_game** window);
int generate_event_queue_window(Window_game **window);
int build_timer(Window_game** window);

/* ---------------------------------------------------------------
 * Aloca memória para objeto window, do tipo Window_game
 * ---------------------------------------------------------------
 * Retorna false se falhar
 * ---------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro*** para Window_game
 -----------------------------------------------------------------*/
int create_window_game(Window_game** window){

    /*
     * Declaração de variáveis locais
     */
    // guarda informações do monitor de vídeo
    ALLEGRO_MONITOR_INFO monitor_info;
    // guardarão informações de largura e altura
    int width,height;

    // aloca memória no objeto Window_game
    (*window) = malloc(sizeof(Window_game));

    // se a alocação falhar, sai da função
    if(!(*window)){
        fprintf(stderr, "falha ao alocar objeto Window_Game\n");
        return false;
    }

    /*
     * configura variáveis de controle (flags)
     */
    // a janela começa aberta, então exit é configurada como false
    (*window)->exit = false;
    // permite o primeiro loop de execução
    (*window)->tick = true;
    // começa na tela de título
    (*window)->current_window = WINDOW_TITLE_MAIN;

    /*
     * Configura altura e largura da janela
     */
    // pega os drivers de vídeo
    al_get_num_video_adapters();

    // se bem sucedido em obter as informações da largura e altura da janela...
    if(al_get_monitor_info(0,&monitor_info)){
        // largura e altura são configurados de acordo com essas informações
        width = monitor_info.x2 - monitor_info.x1;
        height = monitor_info.y2 - monitor_info.y1;
    }
    // se não for bem sucedido...
    else{
        // informa e configura com os valores padrões
        puts("nao foi posivel obter as dimensoes do monitor. usando os valores padroes.");
        width = WINDOW_WIDTH;
        height = WINDOW_HEIGHT;
    }

    // tenta alocar memória para o membro ALLEGRO_DISPLAY, de window
    (*window)->window = al_create_display(width,height);
    // se falhar, desaloca window (tipo Window_game) e sai da função com false
    if(!((*window)->window)){
        fprintf(stderr, "falha ao criar janela\n");
        free(*window);
        (*window) = NULL;
        return false;
    }

    // gera lista de eventos da janela
    if(!generate_event_queue_window(window)){
        al_destroy_display((*window)->window);
        free(*window);
        (*window) = NULL;
        return false;
    }

    // registra eventos da janela
    register_event_queue_window(window);

    // tenta criar timer
    if(!build_timer(window)){
        al_destroy_event_queue((*window)->event_queue_window);
        al_destroy_display((*window)->window);
        free(*window);

        return false;
    }

    // sai da função com sucesso
    return true;
}

/* -----------------------------------------------------------------
 * Cria lista de eventos da janela
 * -----------------------------------------------------------------
 * Retorna false se não conseguir gerar a lista
 * -----------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 -------------------------------------------------------------------*/
int generate_event_queue_window(Window_game **window){
    // aloca lista de eventos
    (*window)->event_queue_window = al_create_event_queue();

    // se a alocação não for bem sucedida, informar e retorna false
    if(!(*window)->event_queue_window) {
        fprintf(stderr, "falha ao criar evento de janela\n");
        return false;
    }
    // em caso de sucesso, retorna true
    else{
        return true;
    }
}

/* -----------------------------------------------------------------------
 * Tenta iniciar o timer que controla a quantidade de frames por segundo
 * -----------------------------------------------------------------------
 * Retorna false se não conseguir
 * -----------------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 -------------------------------------------------------------------------*/
int build_timer(Window_game** window){

    // tenta alocar memória para o objeto timer
    (*window)->timer = malloc(sizeof(Timer_game));
    // se não conseguir, retorna false
    if(!(*window)->timer){
        fprintf(stderr, "falha ao alocar objeto Timer_game\n");
        return false;
    }

    // tenta alocar objeto ALLEGRO_TIMER
    (*window)->timer->timer = (ALLEGRO_TIMER*) al_create_timer(1.0 / FPS_GAME);
    // se falhar, libera memória de timer e sai com erro
    if(!(*window)->timer->timer) {
        fprintf(stderr, "falha ao alocar objeto ALLEGRO_TIMER\n");
        free((*window)->timer);
        return false;
    }

    // tenta alocar memória para a lista de eventos do timer
    (*window)->timer->event_queue_time = al_create_event_queue();
    // se não conseguir, desaloca objetos ALLEGRO_TIMER e Timer_game
    // e sai com false
    if(!(*window)->timer->event_queue_time) {
        fprintf(stderr, "falha ao criar lista de eventos de tempo\n");
        al_destroy_timer((*window)->timer->timer);
        free((*window)->timer);
        return false;
    }

    // se conseguiu, registra eventos do timer
    al_register_event_source((*window)->timer->event_queue_time,
            al_get_timer_event_source((*window)->timer->timer));

    return true;
}

/**************************************************
 * ************************************************
 * Destrutores
 * ************************************************
 **************************************************/

/* -------------------------------------------------------------
 * Desaloca timer
 * -------------------------------------------------------------
 * Desaloca objetos ALLEGRO_TIMER, ALLEGRO_EVENT_QUEUE e
 * Timer_game
 * -------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 ---------------------------------------------------------------*/
void dealloc_timer(Window_game** window){
    // se timer estiver alocado
    if((*window)->timer){
        // desaloca objeto ALLEGRO_TIMER
        al_destroy_timer((*window)->timer->timer);
        // desaloca objeto ALLEGRO_EVENT_QUEUE
        al_destroy_event_queue((*window)->timer->event_queue_time);
        // desaloca objeto Timer_game
        free((*window)->timer);
        (*window)->timer = NULL;
        // informa
        puts("objeto Timer_game desalocado");
    }
}

/* ----------------------------------------------------------------
 * Desaloca window
 * ----------------------------------------------------------------
 * desaloca memória dos objetos allegro e do objeto Window_game
 * ----------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 ------------------------------------------------------------------*/
void dealloc_window(Window_game** window){
    // se window estiver alocada
    if(*window){
        // desaloca memória do objeto ALLEGRO_DISPLAY
        al_destroy_display((*window)->window);
        // desaloca memória do objeto ALLEGRO_EVENT_QUEUE
        al_destroy_event_queue((*window)->event_queue_window);
        // desaloca timer
        dealloc_timer(window);
        // desaloca memória do objeto Window_game
        free(*window);
        (*window) = NULL;
        // informa
        puts("desalocado window");

    }
}

/**************************************************
 * ************************************************
 * Funções de window
 * ************************************************
 **************************************************/

/* ------------------------------------------------------------
 * Inicia timer
 * ------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 --------------------------------------------------------------*/
void start_timer(Window_game** window){
    al_start_timer((*window)->timer->timer);
}

/* ---------------------------------------------------------------
 * Retorna valor de exit da janela
 * ---------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 -----------------------------------------------------------------*/
int get_window_exit_value(Window_game** window){
    return (*window)->exit;
}

/* ---------------------------------------------------------------
 * Retorna valor de tick da janela
 * ---------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 -----------------------------------------------------------------*/
int get_window_tick(Window_game** window){
    return (*window)->tick;
}

/* ----------------------------------------------------------------
 * Configura exit da janela para true
 * ----------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 ------------------------------------------------------------------*/
void set_window_exit_true(Window_game** window){
    (*window)->exit = true;
}

/* ----------------------------------------------------------------
 * Configura valor tick de window para false
 * ----------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 ------------------------------------------------------------------*/
void set_window_tick_false(Window_game** window){
    (*window)->tick = false;
}

/* ----------------------------------------------------------------
 * Torna window o local atual de desenho
 * ----------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 ------------------------------------------------------------------*/
void set_draw_current_window_game(Window_game** window){
    al_set_target_bitmap(al_get_backbuffer((*window)->window));
}

/* --------------------------------------------------------------------------------------
 * Configura cursor do mouse
 * --------------------------------------------------------------------------------------
 * Retorna false se não for possível configurar
 * --------------------------------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 ----------------------------------------------------------------------------------------*/
int set_mouse_cursor_window(Window_game** window){
    // tenta atribuir cursor do mouse
    if (al_set_system_mouse_cursor((*window)->window, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
        return true;
    // se não conseguir, retorna falha
    else{
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        return false;
    }
}

/* ---------------------------------------------------------------------------------------------------
 * Registra eventos da janela na lista de eventos da janela
 * ---------------------------------------------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 -----------------------------------------------------------------------------------------------------*/
void register_event_queue_window(Window_game** window){
    al_register_event_source((*window)->event_queue_window, al_get_display_event_source((*window)->window));
}

/* --------------------------------------------------------------------
 * Checa eventos do timer
 * --------------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 ----------------------------------------------------------------------*/
void check_event_queue_timer(Window_game** window){

    // variável que armazenará um evento
    ALLEGRO_EVENT event;

    // enquanto a lista de eventos não for vazia
    while (!(al_is_event_queue_empty((*window)->timer->event_queue_time))){

        // pega um evento da lista de eventos de tempo e põe em event
        al_wait_for_event((*window)->timer->event_queue_time, &event);

        // se o evento for de timer...
        if(event.type == ALLEGRO_EVENT_TIMER){
            // configura para que a janela possa ser executada
            (*window)->tick = true;
        }
    }
}

/* ----------------------------------------------------------------------------
 * Checa eventos da janela
 * ----------------------------------------------------------------------------
 * Parâmetros:
 * Window_game** window : ponteiro** para Window_game
 ------------------------------------------------------------------------------*/
void check_event_queue_window(Window_game** window){

    // declara variável de captura de eventos
    ALLEGRO_EVENT event;

    // enquanto a lista de eventos da janela não for vazia...
    while (!al_is_event_queue_empty((*window)->event_queue_window)){

        // pega um evento da lista de eventos e põe em event
        al_wait_for_event((*window)->event_queue_window, &event);

        // se o evento for de fechamento da janela...
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            // configura para que a janela possa ser fechada
            (*window)->exit = true;
        }
    }

    // verifica eventos do timer
    check_event_queue_timer(window);
}
