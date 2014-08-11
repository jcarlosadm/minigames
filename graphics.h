/*
 * graphics.h
 * Módulo que gerencia o atlas e a criação de bitmaps e spritesheets
 *
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

// bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
// allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
// mini xml
#include <mxml.h>
// módulos do jogo
#include "window.h"


/*************************************************************
 * ***********************************************************
 * Constantes
 * ***********************************************************
 *************************************************************/

/*
 * Nome e caminho relativo da imagem e informações do atlas
 */
// nome e caminho relativo da imagem do atlas
#define ATLAS_NAME "resources/images/atlas/sheet.png"
// nome e caminho relativo das informações do atlas
#define ATLAS_XML "resources/images/atlas/sheet.xml"


/*************************************************************
 * ***********************************************************
 * Estruturas
 * ***********************************************************
 *************************************************************/

/* -------------------------------------------------
 * Estrutura relativa ao atlas do jogo
 ---------------------------------------------------*/
typedef struct atlas_game Atlas_game;


/*************************************************************
 * ***********************************************************
 * Construtores
 * ***********************************************************
 *************************************************************/

/* ----------------------------------------------------------
 * Carrega e aloca imagem do atlas
 * ----------------------------------------------------------
 * Retorna false se não conseguir
 * ----------------------------------------------------------
 * Parâmetros:
 * Atlas_game** atlas : ponteiro*** para Atlas_game
 ------------------------------------------------------------*/
int create_atlas(Atlas_game** atlas);


/*********************************************************************
 * *******************************************************************
 * Destrutures
 * *******************************************************************
 *********************************************************************/

/* ---------------------------------------------------------------
 * Desaloca objeto atlas
 * ---------------------------------------------------------------
 * Use essa função após o loop principal do jogo
 * ---------------------------------------------------------------
 * Parâmetros:
 * Atlas_game** atlas : ponteiro** para Atlas_game
 -----------------------------------------------------------------*/
void dealloc_atlas(Atlas_game** atlas);



/*********************************************************************
 * *******************************************************************
 * Funções de graphics
 * *******************************************************************
 *********************************************************************/

/*--------------------------------------------------------------------
 * Retorna uma imagem de acordo com o bitmap_name fornecido
 * -------------------------------------------------------------------
 * Parâmetros:
 * const char* bitmap_name : nome do bitmap presente no atlas
 * Window_game** window : ponteiro** para Window_game
 * Atlas_game** atlas : ponteiro** para Atlas_game
 ---------------------------------------------------------------------*/
ALLEGRO_BITMAP* create_bitmap_from_atlas(const char* bitmap_name,Window_game** window,
        Atlas_game** atlas);

#endif /* GRAPHICS_H_ */
