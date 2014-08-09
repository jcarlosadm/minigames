/*
 * controls.h
 *
 */

#ifndef CONTROLS_H_
#define CONTROLS_H_

#include <allegro5/allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include "window.h"

typedef struct mouse_game Mouse_game;

int start_mouse();

int start_event_queue_controls();

void desaloc_mouse();

void desaloc_event_queue_controls();

void check_event_queue_controls();

void update_controls();

void set_mouse_current_state(int state);

int get_mouse_move_state();

void set_mouse_move_state(int state);

int get_mouse_x();

int get_mouse_y();

void set_mouse_coords(int position_x, int position_y);

void register_event_queue_controls();

int mouseIsClicked();

int mouseIsPressed();

int mouseIsReleased();

#endif /* CONTROLS_H_ */
