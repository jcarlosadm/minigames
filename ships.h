/*
 * ships.h
 *
 *  Created on: 05/08/2014
 *      Author: JCarlos
 */

#ifndef SHIPS_H_
#define SHIPS_H_

#include <stdlib.h>

typedef struct attributes{
    int power;
    float speed;
}Attributes;

Attributes* new_attr();
void desaloc_attr(Attributes *attr);

#endif /* SHIPS_H_ */
