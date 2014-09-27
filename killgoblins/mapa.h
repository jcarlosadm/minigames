#ifndef MAPA_H
#define MAPA_H

#include "basico.h"

/*
Prepara as texturas individuais.
Só é chamada uma vez no jogo.
*/
void carrega_texturas(BITMAP *texturas[MAX_TERRENOS],DATAFILE* graficos);

/*
Prepara a matriz do mapa
*/
void prepara_mapa(int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32],char nome_mapa[10]);

/*
Desenha o mapa.
Chamada uma vez a cada fase.
*/
void carrega_mapa(BITMAP *mapa,BITMAP *texturas[MAX_TERRENOS],int matriz_tela[SCREEN_H/32][SCREEN_W/32]);

// Colisões das criaturas com o mapa
int colisao_direita_mapa(float x,float y, int altura, int largura, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3]);
int colisao_esquerda_mapa(float x,float y, int altura, int largura, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3]);
int colisao_abaixo_mapa(float x,float y, int altura, int largura, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3]);
int colisao_cima_mapa(float x,float y, int altura, int largura, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3]);

void calcular_ajuste_mapa(Tcriatura *guerreiro,int *ajuste_mapa);

#endif // MAPA_H
