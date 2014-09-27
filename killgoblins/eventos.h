#ifndef EVENTOS_H
#define EVENTOS_H

#include "basico.h"
#include "controle.h"
#include "item.h"
#include "criaturas.h"
#include "mapa.h"

void verificar_evento(int *pause,int fase,Teventos *eventos,Tcriatura *guerreiro,Tjanelas *janelas,
                      int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],int *ajuste_mapa,
                      MIDI* musica_boss1,MIDI* musica_boss2,MIDI*musica_boss3,SAMPLE *confirmar);

void desenhos_evento(BITMAP *buffer, int fase,Teventos *eventos,Tjanelas *janelas,Tcriatura *guerreiro,FONT* corpo_texto,
                     FONT *titulo_texto,Toponentes *inimigos);

void carregar_var_fase(int fase,Titens *itens, Tcriatura *guerreiro,Toponentes *inimigos,Tjanelas *janelas,BITMAP *background,
                       BITMAP *texturas[MAX_TERRENOS],Teventos *eventos,DATAFILE* graficos);

void pausar(int *pause, int *selecionar,SAMPLE* som_pause);
void verifica_nova_fase(Tcriatura *guerreiro, int *fase, int *carrega_fase, int *tela, int *loading_time, int *estagio_loading,
                        int *tela_destino,Tchefes *chefes_,Teventos *eventos,int *tocando,int *pause,BITMAP* buffer);
void tocar(int *tocando,int fase,MIDI* musica_floresta,MIDI* musica_caverna);
void efeito_inicio_fase(Teventos *eventos,int *pause,BITMAP* buffer,DATAFILE* graficos, SAMPLE* confirmar,int fase,int *tutorial);
void final_de_jogo(BITMAP* buffer,DATAFILE* graficos,int *tela,SAMPLE* confirmar);

#endif // EVENTOS_H
