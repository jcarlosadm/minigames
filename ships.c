/*
 * ships.c
 *
 *  Created on: 05/08/2014
 *      Author: JCarlos
 */

#include "ships.h"

Attributes* new_attr(){
    Attributes *p;
    p = malloc(sizeof(Attributes));
    return p;
}

void desaloc_attr(Attributes* attr){
    free(attr);
}
