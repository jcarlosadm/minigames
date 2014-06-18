#ifndef BASICO_H
#define BASICO_H

/*  #############################################################################
    Todas as bibliotecas básicas usadas no projeto

*/
#include <stdio.h>
#include <allegro.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "imagensFile.h"
#include "musicasFile.h"
#include "efeitosFile.h"
#include "fontesFile.h"

//  #############################################################################

/*  #############################################################################
    Todos os defines usados no projeto

*/

// Altura e largura dos sprites do jogo
#define ALTURA_SPRITE 34
#define LARGURA_SPRITE 18
// controla velocidade padrão dos sprites
#define VELOCIDADE 3
// controla velocidade padrão de mudança dos estados dos sprites
#define ATUALIZAR_ESTADO 10
// Altura e largura da tela da screen
#define MODO_SCREEN GFX_AUTODETECT_WINDOWED// GFX_AUTODETECT_WINDOWED ou GFX_AUTODETECT_FULLSCREEN
#define ALTURA_SCREEN 480
#define LARGURA_SCREEN 640
// Altura e largura das fases
#define ALTURA_MAPA 480
#define LARGURA_MAPA 640*5
// Nível do chão
#define NIVEL_CHAO 13*32
// Altura máxima do pulo
#define ALTURA_PULO 130
// Quantidade de fases
#define N_FASES 3
// tempo da tela de loading
#define TEMPO_LOADING 1*60+30
// DEBUG
#define DEBUG 0
// multiplica a força e armadura do heroi
#define POWER 1

//  #############################################################################

volatile int exit_program; // variável de saída
volatile int timer; // variável de tempo
int volume;
int opacidade;
int inicio_fase;
int fim_fase;

//  ##############  ENUMERADORES   #####################

enum{
TERRA,
CHAO,
PEDRA,
ARVORE_MORTA,
ARVORE_1,
ARVORE_2,
ARVORE_3,
ARVORE_4,
ARVORE_5,
ARVORE_6,
ARVORE_7,
ARVORE_8,
ARVORE_9,
MAX_TERRENOS
};

//  ##############    TYPEDEFS    #########################

// tipos atributo e criatura
typedef struct atributo
{
    int forca;
    int habilidade;
    int resistencia;
    int armadura;
    int poder_de_fogo;
    int hp;
    int mp;
}Tatributo;

typedef struct criatura
{
    char nome[30];
    int x;
    int y;
    int largura;
    int altura;
    int at_ajusteX;
    int at_ajusteY;
    int at_largura;
    int at_altura;
    int quadro_at;
    int id_flecha; // indica que flecha usa, a partir de 1; se 0, não usa flecha
    int direcao; // direita 1 e esquerda 2
    int direcao_anterior;
    int estado_sprite; // estado atual
    int caindo;
    int pulando;
    int permitir_pulo;
    int nivel_plataforma;
    int atacando;
    int tempo_ataque;
    int levando_dano;
    int tempo_dano;
    int tempo_recuo;
    int alerta;
    int drop_item;
    int curando;
    int estrategia; // usado por chefes
    int tempo_estrategia;
    int paralisado;
    int tempo_paralisado;
    int invencivel;
    int destino_chefe;
    BITMAP *sprite;
    BITMAP *vetor_sprite[8];
    BITMAP *face;
    BITMAP *barraHp[11];
    Tatributo caracteristicas;
}Tcriatura;

typedef struct goblinsA
{
    Tcriatura goblins[7];
    int n_goblins;
}TgoblinsA;

typedef struct goblinsB
{
    Tcriatura goblins[7];
    int n_goblins;
}TgoblinsB;

typedef struct chefes
{
    Tcriatura chefe[1];
    int chefe_atual;// se 0, então nenhum
}Tchefes;

typedef struct oponentes
{
    TgoblinsA goblins_guerreiros;
    TgoblinsB goblins_arqueiros;
    Tchefes chefes;
}Toponentes;

typedef struct item
{
    int x;
    int y;
    int largura;
    int altura;
    int tipo;
    int ativo;
    int id_arqueiro; // indique qual arqueiro, a partir de 1, pertence a flecha
    int direcao;
    int x_inicial;
    BITMAP *imagem;
    BITMAP *imagem_buffer;
}Titem;

typedef struct _itens
{
    Titem todosItens[9];
    int n_itens;
}Titens;

typedef struct janela
{
    int x;
    int y;
    int altura;
    int largura;
    int controle;
    int tempo_inicio;
    int tempo_fim;
    char titulo[30];
    char conteudo[256];
    int ator; // 0 heroi, 1 goblin
}Tjanela;

typedef struct _janelas
{
    Tjanela total[6];
    int n_janelas;
    int janela_atual;
}Tjanelas;

typedef struct _eventos
{
    int n_eventos;
    int evento_atual;
    int tempo_evento_atual;
    int eventos_executados[2];
    int tempo_movimento_guerreiro[2];
    int quadro_min[2];
    int quadro_max[2];
    int posicaoX_guerreiro[2];
    int chefe[2]; // 1 sim, 0 não
}Teventos;

/*
    Cabeçalhos das funções
*/

// funções que controlam as variáveis globais timer e exit_program
void fecha_programa();
void incrementa_timer();

// criação da janela, instalação do teclado, mouse e timer
void init();

// finaliza tudo
void deinit();

// Funções gerais que afetam diversos tipos de objetos
int colisao(float ax,float ay, float ah, float al, float bx, float by, float bh, float bl);

void carrega_elementos_fase(int *carrega_fase,int *estagio_loading,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32],
                            char nome_fase[N_FASES][10],int fase,Titens *itens, Tcriatura *guerreiro,Toponentes *inimigos,
                            Tjanelas *janelas, BITMAP *background,BITMAP *texturas[MAX_TERRENOS],Teventos *eventos,
                            BITMAP *mapa,DATAFILE* graficos);

// funções de janela ____________________________________________________________________________________________
void desenhar_retangulo(BITMAP *buffer,int pos_x,int pos_y,int largura,int altura,int opacidade);
void janela_texto(BITMAP *buffer,int pos_x,int pos_y,int largura,int altura,
                  char texto_titulo[256],char texto_corpo[256], FONT* titulo,FONT* corpo,int opacidade,
                  int inicio,int fim,int tempo_jogo,int efeito);
void janela_variavel(BITMAP *buffer,int pos_x,int pos_y,int largura,int altura,int variavel, FONT* fonte,int opacidade);
void janela_dialogo(BITMAP *buffer,Tcriatura *personagem,int pos_x,int pos_y,FONT* titulo,FONT* corpo,int inicio,
                    int fim, int tempo_jogo, char texto_titulo[50],char texto_corpo[256],int efeito);
void preencher_janela(Tjanela *janela_atual,float x, float y, int altura, int largura, int controle, int tempo_inicio,
                      int tempo_fim, char titulo[30],char conteudo[256],int ator);
void menu_inicial (BITMAP *buffer, int *selecionar, BITMAP *menu_iniciar, BITMAP *menu_options, BITMAP *menu_exit,BITMAP *menu_creditos,
                    int *loading_time, int *tela,int *estagio_loading,int *tela_destino, int *fase, int *carrega_fase, int *tempo_jogo,
                    SAMPLE* selecao,SAMPLE* confirmar,int *tocando,MIDI* musica);
void tela_carregamento (BITMAP *buffer, BITMAP *tela_loading[4], int *loading_time,int tela_destino, int *tela,BITMAP *logo);
void pause_menu(int *pause, Teventos *eventos, BITMAP *buffer,int *selecionar,int *tela,int tempo_jogo,int *tela_destino,
                int *loading_time,SAMPLE* selecao,SAMPLE* confirmar,int *tocando);
void game_over(int *pause, Teventos *eventos, BITMAP *buffer,int *selecionar,int *tela,int tempo_jogo,int *tela_destino,
                int *loading_time,SAMPLE* selecao,SAMPLE* confirmar,int *tocando,MIDI* musica_game_over,Tcriatura *guerreiro,
                int *estagio_loading,int *tocando_game_over,int *carrega_fase,DATAFILE* graficos);
void menu_opcoes(BITMAP* buffer, DATAFILE* graficos,SAMPLE* selecao,SAMPLE* confirmacao,int *tela);
void creditos(BITMAP* buffer,DATAFILE* graficos,SAMPLE *confirmacao,int *tela);
//_______________________________________________________________________________________________________________

#endif // BASICO_H
