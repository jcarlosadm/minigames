#include "item.h"

void preencher_item(Titem *objeto, float x, float y, int altura, int largura,
                    int ativo, int tipo,int imagem_preenchida,int id_arqueiro,DATAFILE* graficos)
{
    BITMAP *tiles;

    if(tipo==1)
        tiles = (BITMAP*) graficos[ITENS].dat;
    else if(tipo==2)
        tiles = (BITMAP*) graficos[FLECHA].dat;

    if(!imagem_preenchida)
        objeto->imagem = create_bitmap(32,32);
    else
        clear_bitmap(objeto->imagem);

    if(tipo==1)
        blit(tiles,objeto->imagem,6*32,12*32,0,0,32,32);
    else if(tipo==2)
    {
        rectfill(objeto->imagem,0,0,32,32,makecol(255,0,255));
        draw_sprite(objeto->imagem,tiles,0,0);
    }

    objeto->x=x;
    objeto->y=y;
    objeto->altura=altura;
    objeto->largura=largura;
    objeto->tipo=tipo;
    objeto->ativo=ativo;
    objeto->id_arqueiro=id_arqueiro;
    objeto->direcao=0;
    objeto->x_inicial=x;
    objeto->imagem_buffer = create_bitmap(32,32);
}

void movimento_itens(Titens *itens,Tcriatura *guerreiro,int tempo_jogo,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32],
                     int bloqueios[3])
{
    int i;
    for(i=0;i<itens->n_itens;i++)
    {
        if(itens->todosItens[i].ativo)
        {
            switch(itens->todosItens[i].tipo)
            {
            case 2:
                if(itens->todosItens[i].direcao==1)
                {
                    itens->todosItens[i].x+=2;
                    if(itens->todosItens[i].x - itens->todosItens[i].x_inicial > 300 ||
                       colisao_direita_mapa(itens->todosItens[i].x,itens->todosItens[i].y,itens->todosItens[i].altura,
                                            itens->todosItens[i].largura,matriz_tela,bloqueios))
                        itens->todosItens[i].ativo=0;
                }

                else
                {
                    itens->todosItens[i].x-=2;
                    if(itens->todosItens[i].x - itens->todosItens[i].x_inicial < -300 ||
                       colisao_esquerda_mapa(itens->todosItens[i].x,itens->todosItens[i].y,itens->todosItens[i].altura,
                                            itens->todosItens[i].largura,matriz_tela,bloqueios))
                        itens->todosItens[i].ativo=0;
                }
                break;
            }
        }
    }
}

void desenhar_itens(BITMAP *buffer,Titens *objetos,int ajuste_x)
{
    int i;

    for(i=0;i<objetos->n_itens;i++)
    {
        if(objetos->todosItens[i].ativo)
        {
            clear_bitmap(objetos->todosItens[i].imagem_buffer);
            rectfill(objetos->todosItens[i].imagem_buffer,0,0,32,32,makecol(255,0,255));
            if(objetos->todosItens[i].direcao==1)
                draw_sprite_ex(objetos->todosItens[i].imagem_buffer,objetos->todosItens[i].imagem,0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
            else
                draw_sprite_ex(objetos->todosItens[i].imagem_buffer,objetos->todosItens[i].imagem,0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
            draw_sprite(buffer, objetos->todosItens[i].imagem_buffer, objetos->todosItens[i].x + ajuste_x -(32-objetos->todosItens[i].largura)/2,
                        objetos->todosItens[i].y - (32-objetos->todosItens[i].altura)/2); // manda objeto para buffer
        }
    }
}

void verifique_efeito_item(Titens *itens,Tcriatura *guerreiro,Toponentes *inimigos,int tempo_jogo,SAMPLE *som_dano,SAMPLE* som_cura)
{
    int i;

    //percorre todos os itens da fase
    for(i=0;i<itens->n_itens;i++)
    {
        // se o item estiver ativo...
        if (itens->todosItens[i].ativo)
        {
            // e se o herói colidiu com ele...
            if (colisao(guerreiro->x,guerreiro->y,guerreiro->altura,guerreiro->largura,itens->todosItens[i].x,
                        itens->todosItens[i].y,itens->todosItens[i].altura,itens->todosItens[i].largura))
            {
                // verifica o tipo do item e seu efeito correspondente
                switch(itens->todosItens[i].tipo)
                {
                case 1: // poção
                    play_sample(som_cura,255,128,1000,FALSE); // toca som de cura
                    // cura o herói, desativa o item e deixa o herói com status curando por algum tempo
                    guerreiro->caracteristicas.hp+=5;
                    itens->todosItens[i].ativo=0;
                    guerreiro->curando=tempo_jogo;
                    break;
                case 2: // flecha
                    play_sample(som_dano,255,128,1000,FALSE); // toca som de dano
                    guerreiro->levando_dano=1; // status levando dano do guerreiro é verdadeiro
                    guerreiro->tempo_dano=tempo_jogo; // controla o tempo que ele leva dano
                    // causa dano e desativa o item
                    calcular_dano(&inimigos->goblins_arqueiros.goblins[itens->todosItens[i].id_arqueiro-1],guerreiro,1);
                    itens->todosItens[i].ativo=0;
                    break;
                }
            }
        }
    }
}
