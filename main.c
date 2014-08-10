/*
 * main.c
 *
 */

// bibliotecas padrão
#include <stdio.h>
// allegro
#include <allegro5/allegro.h>
// módulos do jogo
#include "graphics.h"
#include "window.h"
#include "ships.h"
#include "controls.h"

/*---------------------------------------------------------------------------------
 * protótipos necessários (veja a descrição dessas funções mais abaixo, após main)
 ----------------------------------------------------------------------------------*/
// inicialização
int basic_alloc();
// finalização
void basic_dealloc();

/*---------------------------------------------------------------------------------
 * main
 ----------------------------------------------------------------------------------*/
int main()
{
    /*-----------------------------------------
     * variáveis locais
     ------------------------------------------*/


    /*------------------------------------------
     * Inicialização
     -------------------------------------------*/

    // se falhar na inicialização, finaliza programa
    if(!basic_alloc()){
        fprintf(stderr, "falha ao inicializar\n");
        exit(EXIT_FAILURE);
    }

    /* -------------------------------------------
     * Alocações de objetos do jogo
     ---------------------------------------------*/

    // se falhar e alocar player, desaloca memória alocada e finaliza programa
    if(!new_player_ship("blue")){
        basic_dealloc();
        exit(EXIT_FAILURE);
    }

    /*------------------------------------------------
     * Processo principal
     -------------------------------------------------*/

    // apenas configura a cor branca e torna visível
    al_clear_to_color(al_map_rgb_f(0,0,0));
    al_flip_display();

    // inicia timer
    start_timer();

    // loop principal do jogo
    while(!get_window_exit_value()){

        // verifica se algum evento da janela ocorre
        check_event_queue_window();

        // se a janela puder ser executada no fps configurado...
        if(get_window_tick()){

            /* -------------------------------------------------------
             * Lógica
             ---------------------------------------------------------*/

            // checa eventos dos controles
            check_event_queue_controls();

            // atualizar naves (player e enemies) e tiros
            update_ships_objects();

            /* --------------------------------------------------------
             * Desenho
             ----------------------------------------------------------*/

            // limpa a tela
            al_clear_to_color(al_map_rgb_f(0,0,0));

            // desenha naves (player e enemies) e tiros
            draw_ships_objects();

            // torna visível
            al_flip_display();

            /* ---------------------------------------------------------
             * Finalização do loop
             -----------------------------------------------------------*/

            // configura para que a janela não possa mais ser executada até o próximo fps
            set_window_tick_false();

            // atualiza controles
            update_controls();
        }
    }

    /*-------------------------------------------
     * Finalização
     --------------------------------------------*/

    // desaloca memória alocada
    basic_dealloc();

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
int basic_alloc(){

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
    if(success && !create_window_game()) success = false;

    // tenta inicializar controles e eventos relacionados
    if(success && !start_controls()) success = false;

    // tenta carregar atlas
    if(success && !create_atlas()) success = false;

    // se não for bem sucedido em alguma inicialização, desaloca o que foi alocado
    if(!success){
        dealloc_controls();
        dealloc_window();
        dealloc_atlas();
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
void basic_dealloc(){

    // desaloca naves (player e enemies) e tiros
    dealloc_ships_objects();
    // desaloca controles
    dealloc_controls();
    // desaloca atlas
    dealloc_atlas();
    // desaloca window
    dealloc_window();

}
