#include "eventos.h"

void verificar_evento(int *pause,int fase,Teventos *eventos,Tcriatura *guerreiro,Tjanelas *janelas,
                      int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],int *ajuste_mapa,
                      MIDI* musica_boss1,MIDI* musica_boss2,MIDI*musica_boss3,SAMPLE *confirmar)
{
    int i;
    // percorre eventos da fase
    for(i=0;i<eventos->n_eventos;i++)
    {
        // se este evento não foi executado e (o heroi esta em uma posição ou evento atual a i+1)
        if(!eventos->eventos_executados[i] &&
           (guerreiro->x >= eventos->posicaoX_guerreiro[i] || eventos->evento_atual == i+1))
        {

            // trava o evento atual e pausa o jogo
            if(!eventos->evento_atual)
            {
                // diminui volume
                set_volume(volume-100,volume-100);

                eventos->evento_atual=i+1;
                eventos->tempo_evento_atual=timer;
            }

            if(*pause==0)
                *pause=1;

            // faz o guerreiro cair e andar um pouco
            if(eventos->tempo_evento_atual + eventos->tempo_movimento_guerreiro[i] >= timer)
            {
                guerreiro->pulando=0;
                guerreiro->caindo=1;
                guerreiro->permitir_pulo=1;
                colide_chao(guerreiro,matriz_tela,bloqueios,1);
                verificar_queda(guerreiro,matriz_tela,bloqueios);
                movimento_direita(guerreiro,1,matriz_tela,bloqueios,1,1,0,3);
                calcular_ajuste_mapa(guerreiro,ajuste_mapa);
            }

            // agora chama janela com texto
            if(janelas->janela_atual==0 && janelas->total[eventos->quadro_min[i]-1].tempo_fim==-1 &&
               timer >= eventos->tempo_evento_atual + eventos->tempo_movimento_guerreiro[i])
            {
                janelas->janela_atual=eventos->quadro_min[i];
                janelas->total[eventos->quadro_min[i]-1].tempo_inicio=timer;
                janelas->total[eventos->quadro_min[i]-1].tempo_fim=-1;
            }
            // se apertou enter ou espaço, chama a próxima janela ou fecha a atual
            if((apertou(KEY_ENTER)||apertou(KEY_SPACE)) && timer>=eventos->tempo_evento_atual + eventos->tempo_movimento_guerreiro[i])
            {
                play_sample(confirmar,255,128,1000,FALSE); // som de confirmação

                if(janelas->janela_atual==eventos->quadro_min[i])
                    janelas->total[eventos->quadro_min[i]-1].tempo_fim=timer;

                if(janelas->janela_atual!=eventos->quadro_max[i])
                {
                    janelas->janela_atual+=1;
                    janelas->total[janelas->janela_atual-1].tempo_inicio=timer;
                    janelas->total[janelas->janela_atual-1].tempo_fim=-1;
                }
                // se a janela atual é a última, configura o tempo fim dela
                else if(janelas->janela_atual==eventos->quadro_max[i] && janelas->total[eventos->quadro_min[i]-1].tempo_fim!=-1)
                {
                    janelas->total[eventos->quadro_max[i]-1].tempo_fim=timer+20;
                }
            }
            // agora a condição de saída é janelas->janela_atual==0
            if(janelas->janela_atual == 0  && timer >= eventos->tempo_evento_atual + eventos->tempo_movimento_guerreiro[i])
            {
                eventos->evento_atual=0; // configura para nenhum evento atual
                eventos->eventos_executados[i]=1; // configura para o evento não executar de novo
                *pause=0; // tira o pause
                // se o evento for de chefe de fase
                if(eventos->chefe[i])
                    stop_midi();

                if(fase==1 && eventos->chefe[i])
                    play_midi(musica_boss1,TRUE);
                else if(fase==2 && eventos->chefe[i])
                    play_midi(musica_boss2,TRUE);
                else if(fase==3 && eventos->chefe[i])
                    play_midi(musica_boss3,TRUE);

                set_volume(volume,volume);
            }
        }
    }
}

void desenhos_evento(BITMAP *buffer, int fase,Teventos *eventos,Tjanelas *janelas,Tcriatura *guerreiro,FONT* corpo_texto,
                     FONT *titulo_texto,Toponentes *inimigos)
{
    int efeito=0;
    if(janelas->janela_atual!=0)
    {
        if(janelas->janela_atual == eventos->quadro_min[eventos->evento_atual-1])
            efeito=1;
        if(janelas->total[janelas->janela_atual-1].ator==0)
            janela_dialogo(buffer,guerreiro,janelas->total[janelas->janela_atual-1].x,janelas->total[janelas->janela_atual-1].y,
                           titulo_texto,corpo_texto,janelas->total[janelas->janela_atual-1].tempo_inicio,
                           janelas->total[janelas->janela_atual-1].tempo_fim,timer,janelas->total[janelas->janela_atual-1].titulo,
                           janelas->total[janelas->janela_atual-1].conteudo,efeito);
        else
            janela_dialogo(buffer,&inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1],janelas->total[janelas->janela_atual-1].x,
                           janelas->total[janelas->janela_atual-1].y,
                           titulo_texto,corpo_texto,janelas->total[janelas->janela_atual-1].tempo_inicio,
                           janelas->total[janelas->janela_atual-1].tempo_fim,timer,janelas->total[janelas->janela_atual-1].titulo,
                           janelas->total[janelas->janela_atual-1].conteudo,efeito);

        if(timer==janelas->total[janelas->janela_atual-1].tempo_fim)
            janelas->janela_atual=0;
    }
}

void carregar_var_fase(int fase,Titens *itens, Tcriatura *guerreiro,Toponentes *inimigos,Tjanelas *janelas,BITMAP *background,
                       BITMAP *texturas[MAX_TERRENOS],Teventos *eventos,DATAFILE* graficos)
{
    int i;
    BITMAP *fundo;

    for(i=0;i<2;i++)
        eventos->eventos_executados[i]=0;

    eventos->evento_atual=0;

    if(fase==1)
    {
        eventos->tempo_movimento_guerreiro[0]=60;
        eventos->quadro_min[0]=1;
        eventos->quadro_max[0]=2;
        eventos->posicaoX_guerreiro[0]=100;
        eventos->chefe[0]=0;
        eventos->tempo_movimento_guerreiro[1]=60;
        eventos->quadro_min[1]=3;
        eventos->quadro_max[1]=5;
        eventos->posicaoX_guerreiro[1]=LARGURA_MAPA-450;
        eventos->chefe[1]=1;
        eventos->n_eventos=2;

        preencher_item(&itens->todosItens[0],550,NIVEL_CHAO-20,20,15,1,1,0,0,graficos);
        preencher_item(&itens->todosItens[1],0,0,8,15,0,2,0,1,graficos);
        preencher_item(&itens->todosItens[2],0,0,8,15,0,2,0,2,graficos);
        itens->n_itens=3;

        preenche_criatura(guerreiro,0,NIVEL_CHAO-34,20,34,1,2*POWER,3,2,1*POWER,0,-16,-4,20,25,4,0); // preenche status guerreiro
        imagens_guerreiro(guerreiro,graficos); // preenche vetor de imagens do guerreiro

        preenche_criatura(&inimigos->goblins_guerreiros.goblins[0],SCREEN_W-50,NIVEL_CHAO-32,28,32,2,1,1,2,0,0,-19,0,19,26,2,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[0],0,graficos); // preenche vetor de imagens do goblin tipo 1
        preenche_criatura(&inimigos->goblins_guerreiros.goblins[1],750,NIVEL_CHAO-32,28,32,2,1,2,1,0,0,-19,0,19,26,2,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[1],0,graficos); // preenche vetor de imagens do goblin tipo 1
        preenche_criatura(&inimigos->goblins_guerreiros.goblins[2],LARGURA_MAPA-250,NIVEL_CHAO-32,28,32,2,1,1,2,0,0,-19,0,19,26,2,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[2],0,graficos); // preenche vetor de imagens do goblin tipo 1
        preenche_criatura(&inimigos->goblins_guerreiros.goblins[3],LARGURA_MAPA-200,NIVEL_CHAO-32,28,32,2,1,2,1,0,0,-19,0,19,26,2,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[3],0,graficos); // preenche vetor de imagens do goblin tipo 1
        preenche_criatura(&inimigos->goblins_guerreiros.goblins[4],1770,NIVEL_CHAO-32,28,32,2,1,1,2,0,0,-19,0,19,26,2,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[4],0,graficos); // preenche vetor de imagens do goblin tipo 1
        preenche_criatura(&inimigos->goblins_guerreiros.goblins[5],2100,NIVEL_CHAO-32,28,32,2,1,1,2,0,0,-19,0,19,26,2,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[5],0,graficos); // preenche vetor de imagens do goblin tipo 1
        inimigos->goblins_guerreiros.n_goblins=6;

        preenche_criatura(&inimigos->goblins_arqueiros.goblins[0],1020,NIVEL_CHAO-32,28,32,2,0,2,1,1,2,0,0,1,1,2,2);
        imagens_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[0],0,graficos);
        preenche_criatura(&inimigos->goblins_arqueiros.goblins[1],1520,NIVEL_CHAO-32,28,32,2,0,2,1,1,2,0,0,1,1,2,3);
        imagens_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[1],0,graficos);
        inimigos->goblins_arqueiros.n_goblins=2;

        preenche_criatura(&inimigos->chefes.chefe[0],LARGURA_MAPA-300,NIVEL_CHAO-32,28,32,2,3,3,2,2,2,-19,0,19,26,2,0);
        imagens_goblin_chefe(&inimigos->chefes.chefe[0],0,graficos);
        inimigos->chefes.chefe_atual=1;

        carrega_texturas(texturas,graficos); // prepara as texturas

        preencher_janela(&janelas->total[0],70,300,0,0,0,0,-1,"Heroi","Droga, me perdi nesta maldita floresta infestada de goblins.",0);
        preencher_janela(&janelas->total[1],70,300,0,0,0,0,-1,"Heroi","Agora tenho de sair daqui...",0);
        preencher_janela(&janelas->total[2],70,300,0,0,0,0,-1,"Heroi","Droga, esse goblin parece ser forte. Terei de derrota-lo.",0);
        preencher_janela(&janelas->total[3],70,300,0,0,0,0,-1,"Goblin","Grrr!! FrknknfK k feknkn xinksd.",1);
        preencher_janela(&janelas->total[4],70,300,0,0,0,0,-1,"Heroi","Acho que ele nao vai com a minha cara...",0);
        janelas->janela_atual=0;
        janelas->n_janelas=5;

        fundo = (BITMAP*) graficos[FLORESTA].dat;
        clear_bitmap(background);
        draw_sprite(background,fundo,0,0);
    }

    if(fase==2)
    {
        eventos->tempo_movimento_guerreiro[0]=60;
        eventos->quadro_min[0]=1;
        eventos->quadro_max[0]=2;
        eventos->posicaoX_guerreiro[0]=100;
        eventos->chefe[0]=0;
        eventos->tempo_movimento_guerreiro[1]=60;
        eventos->quadro_min[1]=3;
        eventos->quadro_max[1]=5;
        eventos->posicaoX_guerreiro[1]=LARGURA_MAPA-450;
        eventos->chefe[1]=1;
        eventos->n_eventos=2;

        preencher_item(&itens->todosItens[0],550,NIVEL_CHAO-20,20,15,1,1,1,0,graficos);
        preencher_item(&itens->todosItens[1],0,0,8,15,0,2,1,1,graficos);
        preencher_item(&itens->todosItens[2],0,0,8,15,0,2,1,2,graficos);
        preencher_item(&itens->todosItens[3],0,0,8,15,0,2,0,3,graficos);
        preencher_item(&itens->todosItens[4],0,0,8,15,0,2,0,4,graficos);
        preencher_item(&itens->todosItens[5],0,0,8,15,0,2,0,5,graficos);
        preencher_item(&itens->todosItens[6],0,0,8,15,0,2,0,6,graficos);
        preencher_item(&itens->todosItens[7],550,NIVEL_CHAO-20,20,15,0,1,0,0,graficos);
        itens->n_itens=8;

        preenche_criatura(guerreiro,0,NIVEL_CHAO-34,20,34,1,2*POWER,3,2,2*POWER,0,-16,-4,20,25,4,0); // preenche status guerreiro

        preenche_criatura(&inimigos->goblins_guerreiros.goblins[0],SCREEN_W-50,NIVEL_CHAO-32,28,32,2,1,1,2,0,0,-19,0,19,26,2,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[0],1,graficos); // preenche vetor de imagens do goblin tipo 1
        preenche_criatura(&inimigos->goblins_guerreiros.goblins[1],750,NIVEL_CHAO-32,28,32,2,1,2,1,0,0,-19,0,19,26,2,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[1],1,graficos); // preenche vetor de imagens do goblin tipo 1
        inimigos->goblins_guerreiros.n_goblins=2;

        preenche_criatura(&inimigos->goblins_arqueiros.goblins[0],1020,NIVEL_CHAO-32,28,32,2,0,2,1,1,2,0,0,1,1,2,2);
        imagens_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[0],1,graficos);
        preenche_criatura(&inimigos->goblins_arqueiros.goblins[1],1520,NIVEL_CHAO-32,28,32,2,0,2,1,1,2,0,0,1,1,2,3);
        imagens_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[1],1,graficos);
        preenche_criatura(&inimigos->goblins_arqueiros.goblins[2],LARGURA_MAPA-250,NIVEL_CHAO-32,28,32,2,0,2,1,1,2,0,0,1,1,2,4);
        imagens_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[2],0,graficos);
        preenche_criatura(&inimigos->goblins_arqueiros.goblins[3],LARGURA_MAPA-200,NIVEL_CHAO-32,28,32,2,0,2,1,1,2,0,0,1,1,2,5);
        imagens_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[3],0,graficos);
        preenche_criatura(&inimigos->goblins_arqueiros.goblins[4],1700,NIVEL_CHAO-32,28,32,2,0,2,1,1,2,0,0,1,1,2,6);
        imagens_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[4],0,graficos);
        preenche_criatura(&inimigos->goblins_arqueiros.goblins[5],2100,NIVEL_CHAO-32,28,32,2,0,3,1,1,2,0,0,1,1,2,7);
        imagens_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[5],0,graficos);
        inimigos->goblins_arqueiros.n_goblins=6;

        preenche_criatura(&inimigos->chefes.chefe[0],LARGURA_MAPA-300,NIVEL_CHAO-32,28,32,2,3,3,3,2,2,-19,0,19,26,2,0);
        imagens_goblin_chefe(&inimigos->chefes.chefe[0],1,graficos);
        inimigos->chefes.chefe_atual=1;

        preencher_janela(&janelas->total[0],70,300,0,0,0,0,-1,"Heroi","Terei de atravessar essa montanha se quiser sair desta floresta.",0);
        preencher_janela(&janelas->total[1],70,300,0,0,0,0,-1,"Heroi","Nao acho que sera facil...",0);
        preencher_janela(&janelas->total[2],70,300,0,0,0,0,-1,"Heroi","O que? Voce de novo?",0);
        preencher_janela(&janelas->total[3],70,300,0,0,0,0,-1,"Goblin","Grrr!! FrknknfK k feknkn xinksd.",1);
        preencher_janela(&janelas->total[4],70,300,0,0,0,0,-1,"Heroi","Mas que droga!",0);
        janelas->janela_atual=0;
        janelas->n_janelas=5;

        fundo = (BITMAP*) graficos[CAVERNA].dat;
        clear_bitmap(background);
        draw_sprite(background,fundo,0,0);
    }

    if(fase==3)
    {
        eventos->tempo_movimento_guerreiro[0]=60;
        eventos->quadro_min[0]=1;
        eventos->quadro_max[0]=2;
        eventos->posicaoX_guerreiro[0]=100;
        eventos->chefe[0]=0;
        eventos->tempo_movimento_guerreiro[1]=60;
        eventos->quadro_min[1]=3;
        eventos->quadro_max[1]=5;
        eventos->posicaoX_guerreiro[1]=LARGURA_MAPA-450;
        eventos->chefe[1]=1;
        eventos->n_eventos=2;

        preencher_item(&itens->todosItens[0],550,NIVEL_CHAO-20,20,15,1,1,1,0,graficos);
        preencher_item(&itens->todosItens[1],0,0,8,15,0,2,1,1,graficos);
        preencher_item(&itens->todosItens[2],0,0,8,15,0,2,1,2,graficos);
        preencher_item(&itens->todosItens[3],0,0,8,15,0,2,1,3,graficos);
        preencher_item(&itens->todosItens[4],0,0,8,15,0,2,1,4,graficos);
        preencher_item(&itens->todosItens[5],0,0,8,15,0,2,1,5,graficos);
        preencher_item(&itens->todosItens[6],550,NIVEL_CHAO-20,20,15,0,1,1,0,graficos);
        itens->n_itens=7;

        preenche_criatura(guerreiro,0,NIVEL_CHAO-34,20,34,1,3*POWER,3,2,2*POWER,0,-16,-4,20,25,4,0); // preenche status guerreiro

        preenche_criatura(&inimigos->goblins_guerreiros.goblins[0],SCREEN_W-50,NIVEL_CHAO-32,28,32,2,1,1,2,0,0,-19,0,19,26,2,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[0],1,graficos); // preenche vetor de imagens do goblin tipo 1
        preenche_criatura(&inimigos->goblins_guerreiros.goblins[1],750,NIVEL_CHAO-32,28,32,2,1,2,1,0,0,-19,0,19,26,2,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[1],1,graficos); // preenche vetor de imagens do goblin tipo 1
        preenche_criatura(&inimigos->goblins_guerreiros.goblins[2],1800,NIVEL_CHAO-32,28,32,2,3,1,2,1,0,-19,0,19,26,2,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[2],1,graficos); // preenche vetor de imagens do goblin tipo 1
        preenche_criatura(&inimigos->goblins_guerreiros.goblins[3],LARGURA_MAPA-200,NIVEL_CHAO-32,28,32,2,1,2,1,0,0,-19,0,19,26,2,0); // preenche status goblin
        imagens_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[3],1,graficos);
        inimigos->goblins_guerreiros.n_goblins=4;

        preenche_criatura(&inimigos->goblins_arqueiros.goblins[0],720,NIVEL_CHAO-32,28,32,2,0,2,1,1,2,0,0,1,1,2,2);
        imagens_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[0],1,graficos);
        preenche_criatura(&inimigos->goblins_arqueiros.goblins[1],1320,NIVEL_CHAO-32,28,32,2,0,2,1,1,2,0,0,1,1,2,3);
        imagens_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[1],1,graficos);
        preenche_criatura(&inimigos->goblins_arqueiros.goblins[2],LARGURA_MAPA-250,NIVEL_CHAO-32,28,32,2,0,2,1,1,2,0,0,1,1,2,4);
        imagens_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[2],1,graficos);
        preenche_criatura(&inimigos->goblins_arqueiros.goblins[3],1720,NIVEL_CHAO-32,28,32,2,0,2,1,1,2,0,0,1,1,2,5);
        imagens_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[3],1,graficos);
        preenche_criatura(&inimigos->goblins_arqueiros.goblins[4],2520,NIVEL_CHAO-32,28,32,2,0,2,1,1,2,0,0,1,1,2,6);
        imagens_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[4],1,graficos);
        inimigos->goblins_arqueiros.n_goblins=5;

        preenche_criatura(&inimigos->chefes.chefe[0],LARGURA_MAPA-300,NIVEL_CHAO-32,28,32,2,3,3,2,2,2,-19,0,19,26,2,0);
        imagens_goblin_chefe(&inimigos->chefes.chefe[0],1,graficos);
        inimigos->chefes.chefe_atual=1;

        preencher_janela(&janelas->total[0],70,300,0,0,0,0,-1,"Heroi","Agora estou mais perto de sair daqui!",0);
        preencher_janela(&janelas->total[1],70,300,0,0,0,0,-1,"Heroi","Basta continuar andando...",0);
        preencher_janela(&janelas->total[2],70,300,0,0,0,0,-1,"Heroi","Eu nao acredito nisso!",0);
        preencher_janela(&janelas->total[3],70,300,0,0,0,0,-1,"Goblin","Grrr!! FrknknfK k feknkn xinksd.",1);
        preencher_janela(&janelas->total[4],70,300,0,0,0,0,-1,"Heroi","Cala a boca!",0);
        janelas->janela_atual=0;
        janelas->n_janelas=5;

        fundo = (BITMAP*) graficos[FLORESTA].dat;
        clear_bitmap(background);
        draw_sprite(background,fundo,0,0);
    }
}

void pausar(int *pause, int *selecionar,SAMPLE* som_pause)
{
    if (apertou(KEY_ESC) && !inicio_fase && !fim_fase)
    {
        opacidade=0;
        if (*pause == TRUE)
        {
            *pause = FALSE;
            set_volume(volume,volume);
        }

        else
        {
            *pause = TRUE;
            set_volume(volume-100,volume-100);
        }

        play_sample(som_pause,255,128,1000,FALSE);

        if(*selecionar!=0 && *pause==TRUE)
            *selecionar=0;
    }
}

void verifica_nova_fase(Tcriatura *guerreiro, int *fase, int *carrega_fase, int *tela, int *loading_time, int *estagio_loading,
                        int *tela_destino,Tchefes *chefes_,Teventos *eventos,int *tocando,int *pause,BITMAP* buffer)
{
    if(chefes_->chefe_atual!=0)
    {
        if(chefes_->chefe[chefes_->chefe_atual-1].caracteristicas.hp<=0  && *fase<N_FASES)
        {
            if(fim_fase==0)
            {
                fim_fase=1;
                *pause=1;
                eventos->evento_atual=20;
            }

            opacidade+=2;
            drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
            set_trans_blender(255,0,0,opacidade);
            rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(0,0,0));
            solid_mode();

            if(opacidade==254 || opacidade==255)
            {
                eventos->evento_atual=0;
                *pause=0;
                *carrega_fase=1;
                *fase=*fase+1;
                *tela=9; // a próxima tela será a de loading game
                *loading_time = timer;
                *estagio_loading=0;
                *tela_destino=1;
                stop_midi();
                *tocando=0;
            }
        }
        else if(chefes_->chefe[chefes_->chefe_atual-1].caracteristicas.hp<=0  && *fase==N_FASES)
        {
            if(fim_fase==0)
            {
                fim_fase=1;
                *pause=1;
                eventos->evento_atual=20;
            }

            opacidade+=2;
            drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
            set_trans_blender(255,0,0,opacidade);
            rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(0,0,0));
            solid_mode();
            if(opacidade==254 || opacidade==255)
            {
                eventos->evento_atual=0;
                *pause=0;
                *tela=4;
                stop_midi();
                *tocando=0;
            }
        }
    }
}
void tocar(int *tocando,int fase,MIDI* musica_floresta,MIDI* musica_caverna)
{
    if(*tocando==0)
    {
        if(fase==1 || fase==3)
            play_midi(musica_floresta,TRUE);
        else
            play_midi(musica_caverna,TRUE);
        *tocando=1;
    }
}

void efeito_inicio_fase(Teventos *eventos,int *pause,BITMAP* buffer,DATAFILE* graficos, SAMPLE* confirmar,int fase,int *tutorial)
{
    if(inicio_fase) // se for o início da fase
    {
        // informa um evento que não existe, apenas para pausar sem o menu de pause
        eventos->evento_atual=20;
        *pause=1;
        if(opacidade>=2)opacidade-=2; // diminui a opacidade nesse loop
        drawing_mode(DRAW_MODE_TRANS,NULL,0,0); // modo de desenho transparente
        set_trans_blender(255,0,0,opacidade);
        // desenha um retangulo preto que vai ficando cada vez mais transparente
        rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(0,0,0));
        solid_mode(); // modo de desenho sólido

        if(opacidade==0 && (fase!=1 || *tutorial==0)) // outras fases que não 1 ou já apresentou o tutorial
        {
            inicio_fase=0; // não mais é o inicio fase
            *pause=0; // tira o pause
            eventos->evento_atual=0; // retira o evento atual
        }
        // se fase 1 e tutorial == 1, desenha tutorial
        else if(opacidade==0 && fase==1)
        {
            drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
            set_trans_blender(255,0,0,150);
            draw_sprite_ex(buffer,(BITMAP*)graficos[TUTORIAL].dat,30,50,DRAW_SPRITE_TRANS,DRAW_SPRITE_NO_FLIP);
            solid_mode();
            // desenha botão animado no canto do tutorial
            if((timer/16)%2==0)
            {
                rectfill(buffer,SCREEN_W-35,255,SCREEN_W-45,255+10,makecol(255,0,0));
                rectfill(buffer,SCREEN_W-37,255-2,SCREEN_W-47,255+10-2,makecol(180,0,0));
            }
            else
                rectfill(buffer,SCREEN_W-35,255,SCREEN_W-45,255+10,makecol(180,0,0));
            // se apertar enter, esc ou espaço, sai do tutorial
            if(apertou(KEY_ENTER)||apertou(KEY_ESC)||apertou(KEY_SPACE))
            {
                play_sample(confirmar,volume,128,1000,FALSE);
                inicio_fase=0;
                *pause=0;
                eventos->evento_atual=0;
                *tutorial=0;
            }
        }
    }
}

void final_de_jogo(BITMAP* buffer,DATAFILE* graficos,int *tela,SAMPLE* confirmar)
{
    if(opacidade>0)
    {
        if(opacidade==244||opacidade==255)
            rest(2000);

        opacidade-=2;

        drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
        set_trans_blender(255,0,0,opacidade);

        draw_sprite_ex(buffer,(BITMAP*)graficos[CONTINUE].dat,0,0,DRAW_SPRITE_TRANS,DRAW_SPRITE_NO_FLIP);

        solid_mode();

    }
    else if(opacidade==0 || opacidade==1)
    {
        draw_sprite_ex(buffer,(BITMAP*)graficos[CREDITOS].dat,0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);

        if(apertou(KEY_ENTER)||apertou(KEY_SPACE)||apertou(KEY_ESC))
        {
            *tela=0;
            play_sample(confirmar,volume,128,1000,FALSE);
        }
    }
    draw_sprite(screen,buffer,0,0);
}
