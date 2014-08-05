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
    Attributes *player;
    player = new_attr();
    player->speed = 5;
    printf("%.2f\n",player->speed);
    desaloc_attr(player);

    return 0;
}
