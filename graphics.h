/*
 * graphics.h
 *
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <mxml.h>
#include "window.h"

#define ATLAS_NAME "resources/images/atlas/sheet.png"
#define ATLAS_XML "resources/images/atlas/sheet.xml"

typedef struct atlas_game Atlas_game;

int create_atlas();

void desaloc_atlas();

ALLEGRO_BITMAP* create_bitmap_from_atlas(const char* bitmap_name);

#endif /* GRAPHICS_H_ */
