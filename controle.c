#include "controle.h"

void keyboard_input()
{
    int i;
    for(i=0;i<KEY_MAX;i++)
        teclas_anteriores[i]=key[i];

    poll_keyboard();
}

int apertou(int TECLA)
{
    return (key[TECLA]==TRUE && teclas_anteriores[TECLA]==FALSE);
}
int segurou(int TECLA)
{
    return (key[TECLA]==TRUE && teclas_anteriores[TECLA]==TRUE);
}
int soltou(int TECLA)
{
    return (key[TECLA]==FALSE && teclas_anteriores[TECLA]==TRUE);
}
