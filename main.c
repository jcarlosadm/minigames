/*
 * main.c
 *
 *  Created on: 05/08/2014
 *      Author: JCarlos
 */

#include <stdio.h>
#include "ships.h"

int main()
{
    Player_ship *player = new_player_ship(6, 7);
    printPower(getBase(player));
    desaloc_player_ship(player);

    return 0;
}
