/*
 * main.c
 *
 */

// bibliotecas padrão
#include <stdio.h>
// allegro
#include <allegro5/allegro.h>
// módulos do jogo
#include "error_debug_game.h"
#include "graphics.h"
#include "window.h"
#include "ships.h"
#include "controls.h"

/*---------------------------------------------------------------------------------
 * protótipos necessários (veja a descrição dessas funções mais abaixo, após main)
 ----------------------------------------------------------------------------------*/
// inicialização
int basic_alloc(Window_game** window,Graphics_game** graphics,
        Controls_game** controls);
// finalização
void basic_dealloc(Window_game** window,Graphics_game** graphics,
        Controls_game** controls,Player_ship** player,Bullets** bullets);

/*---------------------------------------------------------------------------------
 * main
 ----------------------------------------------------------------------------------*/
int main()
{
    /*-----------------------------------------
     * variáveis locais
     ------------------------------------------*/

    // janela
    Window_game* window = NULL;
    // atlas
    Graphics_game* graphics = NULL;
    // controles
    Controls_game* controls = NULL;
    // Player
    Player_ship* player = NULL;
    // Bullets
    Bullets* bullets = NULL;

    /*------------------------------------------
     * Inicialização
     -------------------------------------------*/

    // se falhar na inicialização, finaliza programa
    if(!basic_alloc(&window,&graphics,&controls)){
        fprintf(stderr, "falha ao inicializar\n");
        exit(EXIT_FAILURE);
    }

    /* -------------------------------------------
     * Alocações de objetos do jogo
     ---------------------------------------------*/

    // se falhar e alocar player, desaloca memória alocada e finaliza programa
    if(!new_player_ship(&player,"blue",&window,&graphics)){
        basic_dealloc(&window,&graphics,&controls,&player,&bullets);
        exit(EXIT_FAILURE);
    }

    if(!makeListofBullets(&bullets)){
        basic_dealloc(&window,&graphics,&controls,&player,&bullets);
        exit(EXIT_FAILURE);
    }

    /*------------------------------------------------
     * Processo principal
     -------------------------------------------------*/

    // apenas configura a cor branca e torna visível
    al_clear_to_color(al_map_rgb_f(0,0,0));
    al_flip_display();

    // inicia timer
    start_timer(&window);

    // loop principal do jogo
    while(!get_window_exit_value(&window)){

        // verifica se algum evento da janela ocorre
        check_event_queue_window(&window);

        // se a janela puder ser executada no fps configurado...
        if(get_window_tick(&window)){

            /* -------------------------------------------------------
             * Lógica
             ---------------------------------------------------------*/

            // checa eventos dos controles
            check_event_queue_controls(&controls);

            // atualizar naves (player e enemies) e tiros
            update_ships_objects(&player,&bullets,&window,&graphics,&controls);

            /* --------------------------------------------------------
             * Desenho
             ----------------------------------------------------------*/

            // limpa a tela
            al_clear_to_color(al_map_rgb_f(0,0,0));

            // desenha naves (player e enemies) e tiros
            draw_ships_objects(&player,&bullets);

            // torna visível
            al_flip_display();

            /* ---------------------------------------------------------
             * Finalização do loop
             -----------------------------------------------------------*/

            // configura para que a janela não possa mais ser executada até o próximo fps
            set_window_tick_false(&window);

            // atualiza controles
            update_controls(&controls);
        }
    }

    /*-------------------------------------------
     * Finalização
     --------------------------------------------*/

    // desaloca memória alocada
    basic_dealloc(&window,&graphics,&controls,&player,&bullets);

    // fecha programa com sucesso
    return EXIT_SUCCESS;
}

/*-------------------------------------------------------------------------
 * Inicialização principal
 * ------------------------------------------------------------------------
 * Inicialização do allegro, inicialização dos módulos extras de imagem,
 * alocação de memória para a janela e sua fila de eventos, inicialização
 * do timer  e sua fila de eventos (ambos na inicialização da janela),
 * inicialização dos controles e de sua fila de eventos, carregamento
 * do atlas.
 * ------------------------------------------------------------------------
 * Se sucesso, retorna true
 * Caso contrário, desaloca o que foi alocado e retorna false
 --------------------------------------------------------------------------*/
int basic_alloc(Window_game** window,Graphics_game** graphics,
        Controls_game** controls){

    // guarda true se sucesso e false se fracasso
    int success = true;

    // se falhar em inicializar o allegro, informa e e configura para false
    if(success && !al_init()){
        fprintf(stderr, "falha ao iniciar o allegro\n");
        success = false;
    }

    // inicializa os módulos extras de imagens
    if(success) al_init_image_addon();

    // tenta criar janela e eventos relacionados, além do timer
    if(success && !create_window_game(window)) success = false;

    // tenta inicializar controles e eventos relacionados
    if(success && !start_controls(controls,window)) success = false;

    // tenta carregar atlas
    if(success && !create_graphics(graphics)) success = false;

    // se não for bem sucedido em alguma inicialização, desaloca o que foi alocado
    if(!success){
        dealloc_controls(controls);
        dealloc_window(window);
        dealloc_graphics(graphics);
        return success;
    }

    // retorna true ou false
    return success;
}

/*---------------------------------------------------------------------
 * Desalocação de memória dos objetos alocados
 * --------------------------------------------------------------------
 * Desaloca player, desaloca controls, desaloca atlas e window
 ----------------------------------------------------------------------*/
void basic_dealloc(Window_game** window,Graphics_game** graphics,
        Controls_game** controls,Player_ship** player, Bullets** bullets){

    // desaloca naves (player e enemies) e tiros
    dealloc_ships_objects(player,bullets);
    // desaloca controles
    dealloc_controls(controls);
    // desaloca atlas
    dealloc_graphics(graphics);
    // desaloca window
    dealloc_window(window);

}
