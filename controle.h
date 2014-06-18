#ifndef CONTROLE_H
#define CONTROLE_H

#include "basico.h"

int teclas_anteriores[KEY_MAX];

void keyboard_input();
int apertou(int TECLA);
int segurou(int TECLA);
int soltou(int TECLA);

#endif // CONTROLE_H
