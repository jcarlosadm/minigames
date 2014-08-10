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
 * Construtores
 * ***********************************************************
 *************************************************************/

/* ----------------------------------------------------------
 * Carrega e aloca imagem do atlas
 * ----------------------------------------------------------
 * Retorna false se não conseguir
 * ----------------------------------------------------------
 * Nota: esse módulo possui um objeto interno que representa
 * a imagem do atlas. Essa função irá alocar esse objeto.
 ------------------------------------------------------------*/
int create_atlas();


/*********************************************************************
 * *******************************************************************
 * Destrutures
 * *******************************************************************
 *********************************************************************/

/* ---------------------------------------------------------------
 * Desaloca objeto atlas
 * ---------------------------------------------------------------
 * Use essa função após o loop principal do jogo
 -----------------------------------------------------------------*/
void dealloc_atlas();



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
 ---------------------------------------------------------------------*/
ALLEGRO_BITMAP* create_bitmap_from_atlas(const char* bitmap_name);

#endif /* GRAPHICS_H_ */
