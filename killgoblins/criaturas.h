#ifndef CRIATURAS_H
#define CRIATURAS_H

#include "basico.h"
#include "controle.h"
#include "mapa.h"

// Funções para todas as criaturas
void preenche_criatura(Tcriatura *ser,float x,float y,float largura, float altura,int direcao,int f,int h,int r,int a,int pdf,
                       int at_ajusteX,int at_ajusteY,int at_largura,int at_altura,int quadro_at, int id_flecha);
void movimento_direita(Tcriatura *ser,int deslocamento,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],
                       int mudar_direcao,int mudar_sprite,int sprite_inf,int sprite_sup);
void movimento_esquerda(Tcriatura *ser,int deslocamento,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],
                       int mudar_direcao,int mudar_sprite,int sprite_inf,int sprite_sup);
void pulo(Tcriatura *ser,int deslocamentoy,int deslocamentox,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3]);
void recuo(Tcriatura *ser,int deslocamento,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3]);
void recuo_por_dano(Tcriatura *ser,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3]);
void colide_chao(Tcriatura *ser,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],int eh_heroi);
void verificar_queda(Tcriatura *ser,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3]);
void verificar_status(Tcriatura *ser,Toponentes *inimigos, int tempo_jogo,Titens *itens);
void mudanca_sprite(int limite_inferior,int limite_superior,int *estado_sprite,int intervalo,int tempo_inicio,int tempo_jogo);
void ataque_ajustes(Tcriatura *atacante,int tempo_jogo,int confirmacao,int sprite_lim_inf,int sprite_lim_sup,SAMPLE* som_ataque);
void ataque(Tcriatura *atacante,Tcriatura *alvo,int tempo_jogo,int tipo_at,SAMPLE *som_dano);
int dano_ataque(Tcriatura* atacante, int tipo_ataque);
void calcular_dano(Tcriatura* atacante, Tcriatura* alvo,int tipo_ataque);
void deixa_item(Titens *itens,Tcriatura *goblin);

// Funções do guerreiro
void imagens_guerreiro(Tcriatura *guerreiro,DATAFILE* graficos);
void movimento_guerreiro(Tcriatura *guerreiro, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],
                         SAMPLE* som_recuo);
void ataque_guerreiro(Tcriatura *guerreiro,int tempo_jogo,Toponentes *inimigos,SAMPLE* som_ataque,SAMPLE *som_dano_goblin);
void tocou_oponente(Tcriatura *guerreiro,Toponentes *inimigos,int tempo_jogo,SAMPLE *som_dano);
void desenhar_guerreiro(BITMAP *buffer,Tcriatura *guerreiro,Toponentes *inimigos,int ajuste_x,int tempo_jogo,DATAFILE* graficos);

// Funções do goblin guerreiro
void imagens_goblin_guerreiro(Tcriatura *goblin1, int preenchida,DATAFILE* graficos);
void movimento_goblin_guerreiro(Tcriatura *goblin1,Tcriatura *guerreiro,int tempo_jogo, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3]);
void ataque_goblin_guerreiro(Tcriatura *goblin, Tcriatura *guerreiro, int tempo_jogo,SAMPLE *som_ataque,SAMPLE* som_dano_guerreiro);
void desenhar_goblin_guerreiro(BITMAP *buffer,Tcriatura *goblin1,int ajuste_x,int tempo_jogo);

// Funções para o goblin arqueiro
void imagens_goblin_arqueiro(Tcriatura *goblin, int preenchida,DATAFILE* graficos);
void movimento_goblin_arqueiro(Tcriatura *goblin1,Tcriatura *guerreiro, int tempo_jogo,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3]);
void ataque_goblin_arqueiro(Tcriatura *goblin, Tcriatura *guerreiro, int tempo_jogo,Titens *itens,SAMPLE* som_ataque);
void desenhar_goblin_arqueiro(BITMAP *buffer,Tcriatura *goblin1,int ajuste_x,int tempo_jogo);

// Funções para o goblin chefe
void imagens_goblin_chefe(Tcriatura *goblin, int preenchida,DATAFILE* graficos);
void movimento_goblin_chefe(Tcriatura *goblin1,Tcriatura *guerreiro, int tempo_jogo, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3]);
void ataque_goblin_chefe(Tcriatura *goblin, Tcriatura *guerreiro, int tempo_jogo,SAMPLE* som_ataque,SAMPLE* som_dano_guerreiro,
                         SAMPLE* som_paralisia);
void desenhar_goblin_chefe(BITMAP *buffer,Tcriatura *goblin1,int ajuste_x,int tempo_jogo);

// Funções para todos os goblins
void acoes_goblins(Toponentes *inimigos, Tcriatura *guerreiro, int tempo_jogo, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],
                   Titens *itens,SAMPLE* espada, SAMPLE* besta,SAMPLE* som_dano_guerreiro,SAMPLE* som_paralisia);
void desenhar_todos_goblins(Toponentes *inimigos,BITMAP *buffer, int ajuste_mapa,int tempo_jogo);

#endif // CRIATURAS_H
