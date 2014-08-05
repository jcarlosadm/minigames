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
    Ship *player = new_ship(4, 7);
    printPower(player);
    desaloc_ship(player);

    return 0;
}
