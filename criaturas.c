#include "criaturas.h"

void preenche_criatura(Tcriatura *ser,float x,float y,float largura, float altura,int direcao,int f,int h,int r,int a,int pdf,
                       int at_ajusteX,int at_ajusteY,int at_largura,int at_altura,int quadro_at, int id_flecha)
{
    ser->x=x;
    ser->y=y;
    ser->altura = altura;
    ser->largura = largura;
    ser->at_ajusteX = at_ajusteX;
    ser->at_ajusteY = at_ajusteY;
    ser->at_largura = at_largura;
    ser->at_altura = at_altura;
    ser->quadro_at = quadro_at;
    ser->id_flecha = id_flecha;
    ser->direcao=direcao;
    ser->direcao_anterior=1;
    ser->estado_sprite=0;
    ser->caracteristicas.forca=f;
    ser->caracteristicas.habilidade=h;
    ser->caracteristicas.resistencia=r;
    ser->caracteristicas.armadura=a;
    ser->caracteristicas.poder_de_fogo=pdf;
    ser->caracteristicas.hp=ser->caracteristicas.resistencia*5;
    ser->caracteristicas.mp=ser->caracteristicas.resistencia*5;
    ser->caindo=0;
    ser->pulando=0;
    ser->permitir_pulo = 1;
    ser->atacando=0;
    ser->levando_dano=0;
    ser->alerta=0;
    ser->drop_item=0;
    ser->curando=-1;
    ser->estrategia=1;
    ser->paralisado=0;
    ser->sprite = create_bitmap(64,64);
    ser->invencivel=0;
    ser->destino_chefe=1;
}

void imagens_guerreiro(Tcriatura *guerreiro,DATAFILE* graficos)
{
    int i,j;
    BITMAP *tiles = (BITMAP*) graficos[GUERREIRO].dat;
    BITMAP *tilesHp = (BITMAP*) graficos[BARRA_HP].dat;

    for(i=0;i<2;i++)
    {
        for(j=0;j<4;j++)
        {
            guerreiro->vetor_sprite[j+4*i]=create_bitmap(64,64);
            blit(tiles,guerreiro->vetor_sprite[j+4*i],j*64,i*64,0,0,64,64);
        }
    }

    for(i=0;i<11;i++)
    {
        guerreiro->barraHp[i]=create_bitmap(123,16);
        blit(tilesHp,guerreiro->barraHp[i],0,i*16,0,0,123,16);
    }

    guerreiro->face = (BITMAP*) graficos[GUERREIRO_FACE].dat;
}

void movimento_guerreiro(Tcriatura *guerreiro, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],
                         SAMPLE* som_recuo)
{
    if(!guerreiro->levando_dano) // se não esta levando dano
    {
        // para movimento são os estados 0 a 3 do sprite
        if(guerreiro->estado_sprite > 3 && !guerreiro->atacando)
            guerreiro->estado_sprite = 0;

        // apertou direita
        if(segurou(KEY_RIGHT) && !guerreiro->paralisado)
            movimento_direita(guerreiro,guerreiro->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,0,3);

        // apertou esquerda
        if(segurou(KEY_LEFT) && !guerreiro->paralisado)
            movimento_esquerda(guerreiro,guerreiro->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,0,3);

        // apertou para cima
        if(segurou(KEY_UP) && !guerreiro->paralisado)
            pulo(guerreiro,guerreiro->caracteristicas.habilidade,0,matriz_tela,bloqueios);

        // apertou para baixo
        if(apertou(KEY_DOWN) && !guerreiro->paralisado)
        {
            play_sample(som_recuo,255,128,1000,FALSE);
            guerreiro->tempo_recuo=timer; // limita tempo de recuo
        }
        if(segurou(KEY_DOWN) && !guerreiro->paralisado)
            recuo(guerreiro,guerreiro->caracteristicas.habilidade,matriz_tela,bloqueios);

        // se soltar o botão de pulo
        if(!key[KEY_UP])
        {
            guerreiro->pulando=0;
            guerreiro->caindo=1;
            guerreiro->permitir_pulo=1;
        }
    }
    else // se sofre dano
        recuo_por_dano(guerreiro,matriz_tela,bloqueios);

    // verificações básicas
    colide_chao(guerreiro,matriz_tela,bloqueios,1); // colidiu com o chão? primeira verificação obrigatória
    verificar_queda(guerreiro,matriz_tela,bloqueios); // atingiu o limite de pulo? está em queda?
}

void verificar_status(Tcriatura *ser,Toponentes *inimigos, int tempo_jogo,Titens *itens)
{
    int i;
    if(ser->tempo_dano+20<=tempo_jogo)
        ser->levando_dano=0;
    if(tempo_jogo>=ser->tempo_paralisado+1*60+30)
        ser->paralisado=0;
    for(i=0;i<inimigos->goblins_guerreiros.n_goblins;i++)
    {
        if(inimigos->goblins_guerreiros.goblins[i].caracteristicas.hp <= 0 && !inimigos->goblins_guerreiros.goblins[i].drop_item)
        {
            inimigos->goblins_guerreiros.goblins[i].drop_item=1;
            deixa_item(itens,&inimigos->goblins_guerreiros.goblins[i]);
        }
    }
    for(i=0;i<inimigos->goblins_arqueiros.n_goblins;i++)
    {
        if(inimigos->goblins_arqueiros.goblins[i].caracteristicas.hp <= 0 && !inimigos->goblins_arqueiros.goblins[i].drop_item)
        {
            inimigos->goblins_arqueiros.goblins[i].drop_item=1;
            deixa_item(itens,&inimigos->goblins_arqueiros.goblins[i]);
        }
    }
}

void ataque_guerreiro(Tcriatura *guerreiro,int tempo_jogo,Toponentes *inimigos,SAMPLE* som_ataque,SAMPLE *som_dano_goblin)
{
    int i;
    int confirmacao=0;

    if(apertou(KEY_Z) && !guerreiro->paralisado)
        confirmacao=1;

    ataque_ajustes(guerreiro,tempo_jogo,confirmacao,4,7,som_ataque);

    for(i=0;i<inimigos->goblins_guerreiros.n_goblins && guerreiro->atacando;i++)
    {
        if(!inimigos->goblins_guerreiros.goblins[i].levando_dano && inimigos->goblins_guerreiros.goblins[i].caracteristicas.hp>0)
            ataque(guerreiro,&inimigos->goblins_guerreiros.goblins[i],tempo_jogo,0,som_dano_goblin);
    }
    for(i=0;i<inimigos->goblins_arqueiros.n_goblins && guerreiro->atacando;i++)
    {
        if(!inimigos->goblins_arqueiros.goblins[i].levando_dano && inimigos->goblins_arqueiros.goblins[i].caracteristicas.hp>0)
            ataque(guerreiro,&inimigos->goblins_arqueiros.goblins[i],tempo_jogo,0,som_dano_goblin);
    }
    if(inimigos->chefes.chefe_atual!=0 && guerreiro->atacando)
    {
        if(!inimigos->chefes.chefe[inimigos->chefes.chefe_atual -1].levando_dano &&
           inimigos->chefes.chefe[inimigos->chefes.chefe_atual -1].caracteristicas.hp > 0)
            ataque(guerreiro,&inimigos->chefes.chefe[inimigos->chefes.chefe_atual -1],tempo_jogo,0,som_dano_goblin);
    }
}

void tocou_oponente(Tcriatura *guerreiro,Toponentes *inimigos,int tempo_jogo,SAMPLE *som_dano)
{
    int i;
    for(i=0;i<inimigos->goblins_guerreiros.n_goblins;i++)
    {
        if(inimigos->goblins_guerreiros.goblins[i].caracteristicas.hp>0 && colisao(guerreiro->x,guerreiro->y,
            guerreiro->altura,guerreiro->largura, inimigos->goblins_guerreiros.goblins[i].x,
            inimigos->goblins_guerreiros.goblins[i].y,inimigos->goblins_guerreiros.goblins[i].altura,
            inimigos->goblins_guerreiros.goblins[i].largura) && !guerreiro->levando_dano)
        {
            play_sample(som_dano,255,128,1000,FALSE);
            guerreiro->levando_dano=1;
            guerreiro->tempo_dano=tempo_jogo;
            guerreiro->caracteristicas.hp-=1;
            if(guerreiro->x+guerreiro->largura < (inimigos->goblins_guerreiros.goblins[i].x+inimigos->goblins_guerreiros.goblins[i].largura))
                guerreiro->direcao=1;
            else
                guerreiro->direcao=2;
        }
    }
    for(i=0;i<inimigos->goblins_arqueiros.n_goblins;i++)
    {
        if(inimigos->goblins_arqueiros.goblins[i].caracteristicas.hp>0 && colisao(guerreiro->x,guerreiro->y,
            guerreiro->altura,guerreiro->largura, inimigos->goblins_arqueiros.goblins[i].x,
            inimigos->goblins_arqueiros.goblins[i].y,inimigos->goblins_arqueiros.goblins[i].altura,
            inimigos->goblins_arqueiros.goblins[i].largura) && !guerreiro->levando_dano)
        {
            play_sample(som_dano,255,128,1000,FALSE);
            guerreiro->levando_dano=1;
            guerreiro->tempo_dano=tempo_jogo;
            guerreiro->caracteristicas.hp-=1;
            if(guerreiro->x+guerreiro->largura < (inimigos->goblins_arqueiros.goblins[i].x+inimigos->goblins_arqueiros.goblins[i].largura))
                guerreiro->direcao=1;
            else
                guerreiro->direcao=2;
        }
    }
    if(inimigos->chefes.chefe_atual!=0)
    {
        if(inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].caracteristicas.hp>0 && colisao(guerreiro->x,guerreiro->y,
            guerreiro->altura,guerreiro->largura, inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].x,
            inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].y,inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].altura,
            inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].largura) && !guerreiro->levando_dano)
        {
            play_sample(som_dano,255,128,1000,FALSE);
            guerreiro->levando_dano=1;
            guerreiro->tempo_dano=tempo_jogo;
            guerreiro->caracteristicas.hp-=1;
            if(guerreiro->x+guerreiro->largura < (inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].x+inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].largura))
                guerreiro->direcao=1;
            else
                guerreiro->direcao=2;
        }
    }

    if(guerreiro->tempo_dano+20 <= tempo_jogo)
        guerreiro->levando_dano=0;
}

void desenhar_guerreiro(BITMAP *buffer,Tcriatura *guerreiro,Toponentes *inimigos,int ajuste_x,int tempo_jogo,DATAFILE* graficos)
{
    rectfill(guerreiro->sprite,0,0,64,64,makecol(255,0,255));
    BITMAP *mato=(BITMAP*) graficos[TERRENOS].dat;
    BITMAP *mato_recortado=create_bitmap(32,32);
    rectfill(mato_recortado,0,0,32,32,makecol(255,0,255));

    if(guerreiro->direcao==1)
        draw_sprite_ex(guerreiro->sprite,guerreiro->vetor_sprite[guerreiro->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
    else
        draw_sprite_ex(guerreiro->sprite,guerreiro->vetor_sprite[guerreiro->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);

    if(inimigos->chefes.chefe_atual!=0)
    {
        if(inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].estrategia==2 &&
           tempo_jogo<=inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].tempo_estrategia+3*60 &&
           !guerreiro->caindo && !guerreiro->pulando)
        {
            blit(mato,mato_recortado,5*32,8*32,0,0,32,32);
            draw_sprite(guerreiro->sprite,mato_recortado,16,32+(guerreiro->altura/2)-28);
        }
        else if(inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].estrategia==2 &&
           tempo_jogo>inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].tempo_estrategia+3*60 &&
           !guerreiro->caindo && !guerreiro->pulando)
       {
            blit(mato,mato_recortado,3*32,14*32,0,0,32,32);
            draw_sprite(guerreiro->sprite,mato_recortado,16,32+(guerreiro->altura/2)-28);
       }
    }

    if(guerreiro->levando_dano || guerreiro->paralisado)
    {
        set_trans_blender(255,255,255,150);
        if((tempo_jogo/10)%2==0)
        {
            draw_sprite_ex(buffer, guerreiro->sprite, ajuste_x + guerreiro->x - (64 - guerreiro->largura)/2,
                guerreiro->y - (64 - guerreiro->altura)/2,DRAW_SPRITE_TRANS,DRAW_SPRITE_NO_FLIP); // manda guerreiro para buffer
        }
        else
            draw_sprite_ex(buffer, guerreiro->sprite, ajuste_x + guerreiro->x - (64 - guerreiro->largura)/2,
                guerreiro->y - (64 - guerreiro->altura)/2,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP); // manda guerreiro para buffer
    }
    else if(guerreiro->curando!=-1)
    {
        set_trans_blender(255, 255, 255, 255);
        draw_lit_sprite(buffer, guerreiro->sprite, ajuste_x + guerreiro->x - (64 - guerreiro->largura)/2,
                guerreiro->y - (64 - guerreiro->altura)/2, (tempo_jogo-guerreiro->curando)*4);
        if(tempo_jogo - guerreiro->curando >= 60)
            guerreiro->curando=-1;
    }
    else
        draw_sprite(buffer, guerreiro->sprite, ajuste_x + guerreiro->x - (64 - guerreiro->largura)/2,
                guerreiro->y - (64 - guerreiro->altura)/2); // manda guerreiro para buffer

    if(guerreiro->caracteristicas.hp>10)
        guerreiro->caracteristicas.hp=10;

    int hpAtual = guerreiro->caracteristicas.hp;

    if(hpAtual>=0 && hpAtual<=10)
        draw_sprite(buffer, guerreiro->barraHp[hpAtual], 10, 10);
    else
        if(hpAtual<0) draw_sprite(buffer, guerreiro->barraHp[0], 10, 10);

    destroy_bitmap(mato_recortado);

}

void imagens_goblin_guerreiro(Tcriatura *goblin, int preenchida,DATAFILE* graficos)
{
    int i;
    BITMAP *tiles = (BITMAP*) graficos[GOBLIN_GUERREIRO].dat;

    if(preenchida)
    {
        for(i=0;i<8;i++)
            clear_bitmap(goblin->vetor_sprite[i]);
    }
    else
    {
        for(i=0;i<8;i++)
            goblin->vetor_sprite[i]=create_bitmap(64,64);
    }

    for(i=0;i<8;i++)
        blit(tiles,goblin->vetor_sprite[i],i*64,0,0,0,64,64);
}

void movimento_goblin_guerreiro(Tcriatura *goblin1,Tcriatura *guerreiro, int tempo_jogo, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{

    if(!goblin1->levando_dano && goblin1->caracteristicas.hp > 0)
    {
        if(goblin1->estado_sprite < 5 && !goblin1->atacando)
            goblin1->estado_sprite = 5;
        if(goblin1->alerta)
        {
            if (goblin1->x-3 > guerreiro->x+guerreiro->largura || goblin1->x-4 > guerreiro->x+guerreiro->largura || goblin1->x-5 > guerreiro->x+guerreiro->largura
                || goblin1->x-6 > guerreiro->x+guerreiro->largura)
            {
                if(!colisao_abaixo_mapa(goblin1->x-goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_esquerda_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || (colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       && guerreiro->y + guerreiro->altura < goblin1->y))
                    pulo(goblin1,3,-1,matriz_tela,bloqueios);
                else
                    movimento_esquerda(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                goblin1->direcao_anterior=2;
            }
            else if (goblin1->x+goblin1->largura+3 < guerreiro->x || goblin1->x+goblin1->largura+4 < guerreiro->x
                     || goblin1->x+goblin1->largura+5 < guerreiro->x || goblin1->x+goblin1->largura+6 < guerreiro->x)
            {
                if(!colisao_abaixo_mapa(goblin1->x+goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_direita_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || (colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       && guerreiro->y + guerreiro->altura < goblin1->y))
                    pulo(goblin1,3,1,matriz_tela,bloqueios);
                else
                    movimento_direita(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                goblin1->direcao_anterior=1;
            }
            else
                goblin1->direcao=0;

            // radar
            if((guerreiro->x + guerreiro->largura < goblin1->x && goblin1->x - (guerreiro->x + guerreiro->largura) >= SCREEN_W)
               || (goblin1->x + goblin1->largura < guerreiro->x && guerreiro->x - (goblin1->x +goblin1->largura) >= SCREEN_W))
                    goblin1->alerta=0;
        }
        else
        {
            if((tempo_jogo/120)%2==0)
                goblin1->direcao=1;
            else
                goblin1->direcao=2;
            if (goblin1->direcao==2)
            {
                if(!colisao_abaixo_mapa(goblin1->x-goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_esquerda_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,-1,matriz_tela,bloqueios);
                else
                    movimento_esquerda(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                goblin1->direcao_anterior=2;
            }
            else
            {
                if(!colisao_abaixo_mapa(goblin1->x+goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_direita_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,1,matriz_tela,bloqueios);
                else
                    movimento_direita(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                goblin1->direcao_anterior=1;
            }

            // radar
            if((guerreiro->x + guerreiro->largura < goblin1->x && goblin1->direcao==2
                 && goblin1->x - (guerreiro->x + guerreiro->largura) <= 150)
               || (goblin1->x + goblin1->largura < guerreiro->x && goblin1->direcao==1
                   && guerreiro->x - (goblin1->x +goblin1->largura) <= 150))
                    goblin1->alerta=1;
        }
    }
    else if(goblin1->levando_dano && (goblin1->caracteristicas.hp > 0 || goblin1->levando_dano))
    {
        goblin1->estado_sprite = 1;
        if(goblin1->tempo_dano+10>=tempo_jogo)
            recuo_por_dano(goblin1,matriz_tela,bloqueios);
        else
            goblin1->levando_dano=0;
    }

    // verificações básicas
    colide_chao(goblin1,matriz_tela,bloqueios,0); // colidiu com o chão? primeira verificação obrigatória
    verificar_queda(goblin1,matriz_tela,bloqueios); // atingiu o limite de pulo? está em queda?
}

void ataque_goblin_guerreiro(Tcriatura *goblin, Tcriatura *guerreiro, int tempo_jogo,SAMPLE *som_ataque,SAMPLE* som_dano_guerreiro)
{
    if(goblin->direcao==0 && goblin->caracteristicas.hp > 0) // se goblin não se movimenta e tem hp>0
    {
        // cai se estiver pulando
        goblin->caindo=1;
        goblin->pulando=0;
        // se guerreiro estiver na mesma altura do goblin
        if(guerreiro->y <= goblin->y + goblin->altura && guerreiro->y+guerreiro->altura >= goblin->y)
        {
            // pega a direção anterior do goblin para ajustar o ataque
            goblin->direcao=goblin->direcao_anterior;

            // realiza o ajuste do ataque e o sprite que aparecerá
            ataque_ajustes(goblin,tempo_jogo,1,2,4,som_ataque);
            // se guerreiro não está levando dano e goblin estiver atacando, ataca e causa dano
            if(!guerreiro->levando_dano && goblin->atacando)
                ataque(goblin,guerreiro,tempo_jogo,0,som_dano_guerreiro);

            goblin->direcao=0; // retorna ao estado de não movimentação do goblin
        }
    }
}

void desenhar_goblin_guerreiro(BITMAP *buffer,Tcriatura *goblin1,int ajuste_x,int tempo_jogo)
{
    rectfill(goblin1->sprite,0,0,64,64,makecol(255,0,255));
    if(goblin1->direcao==1)
    {
        draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
    }
    else if(goblin1->direcao==2)
    {
        draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
    }
    else if(goblin1->direcao==0)
    {
        if(goblin1->direcao_anterior==1)
        {
            draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
        }
        else
        {
            draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
        }
    }
    if(goblin1->levando_dano)
    {
        set_trans_blender(255,255,255,50);
        if((tempo_jogo/10)%2==0)
            draw_sprite_ex(buffer, goblin1->sprite, ajuste_x + goblin1->x-(64-goblin1->largura)/2,
                goblin1->y-(64-goblin1->altura)/2,DRAW_SPRITE_TRANS,DRAW_SPRITE_NO_FLIP);
        else
            draw_sprite_ex(buffer, goblin1->sprite, ajuste_x + goblin1->x-(64-goblin1->largura)/2,
                goblin1->y-(64-goblin1->altura)/2,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
    }
    else
        draw_sprite(buffer, goblin1->sprite, ajuste_x + goblin1->x-(64-goblin1->largura)/2,
                goblin1->y-(64-goblin1->altura)/2);
}

void imagens_goblin_arqueiro(Tcriatura *goblin, int preenchida,DATAFILE* graficos)
{
    int i;
    BITMAP *tiles = (BITMAP*) graficos[GOBLIN_ARQUEIRO].dat;

    if(preenchida)
    {
        for(i=0;i<7;i++)
            clear_bitmap(goblin->vetor_sprite[i]);
    }
    else
    {
        for(i=0;i<7;i++)
            goblin->vetor_sprite[i]=create_bitmap(64,64);
    }

    for(i=0;i<7;i++)
        blit(tiles,goblin->vetor_sprite[i],i*64,0,0,0,64,64);

}

void movimento_goblin_arqueiro(Tcriatura *goblin1,Tcriatura *guerreiro, int tempo_jogo,
                                int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{

    if(!goblin1->levando_dano && goblin1->caracteristicas.hp > 0)
    {
        if(goblin1->estado_sprite < 4 && !goblin1->atacando)
            goblin1->estado_sprite = 4;
        if(goblin1->alerta)
        {
            if (goblin1->x > guerreiro->x+(guerreiro->largura/2) && goblin1->x - (guerreiro->x + (guerreiro->largura/2)) < 150)
            {
                if(!colisao_abaixo_mapa(goblin1->x+goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_direita_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,1,matriz_tela,bloqueios);
                else
                    movimento_direita(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,4,6);
                goblin1->direcao_anterior=2;
            }
            else if (goblin1->x < guerreiro->x+(guerreiro->largura/2) && (guerreiro->x + (guerreiro->largura/2)) - goblin1->x < 150)
            {
                if(!colisao_abaixo_mapa(goblin1->x-goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_esquerda_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,-1,matriz_tela,bloqueios);
                else
                    movimento_esquerda(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,4,6);
                goblin1->direcao_anterior=1;
            }
            else
                goblin1->direcao=0;

            // radar
            if((guerreiro->x + guerreiro->largura < goblin1->x && goblin1->x - (guerreiro->x + guerreiro->largura) >= SCREEN_W)
               || (goblin1->x + goblin1->largura < guerreiro->x && guerreiro->x - (goblin1->x +goblin1->largura) >= SCREEN_W))
                    goblin1->alerta=0;
        }
        else
        {
            if((tempo_jogo/120)%2==0)
                goblin1->direcao=1;
            else
                goblin1->direcao=2;
            if (goblin1->direcao==2)
            {
                if(!colisao_abaixo_mapa(goblin1->x-goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_esquerda_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,-1,matriz_tela,bloqueios);
                else
                    movimento_esquerda(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,4,6);
                goblin1->direcao_anterior=2;
            }
            else
            {
                if(!colisao_abaixo_mapa(goblin1->x+goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_direita_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                   || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                    pulo(goblin1,3,1,matriz_tela,bloqueios);
                else
                    movimento_direita(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,4,6);
                goblin1->direcao_anterior=1;
            }

            // radar
            if((guerreiro->x + guerreiro->largura < goblin1->x && goblin1->direcao==2
                 && goblin1->x - (guerreiro->x + guerreiro->largura) <= 150)
               || (goblin1->x + goblin1->largura < guerreiro->x && goblin1->direcao==1
                   && guerreiro->x - (goblin1->x +goblin1->largura) <= 150))
                    goblin1->alerta=1;
        }
    }
    else if(goblin1->levando_dano && (goblin1->caracteristicas.hp > 0 || goblin1->levando_dano))
    {
        goblin1->estado_sprite = 1;
        if(goblin1->tempo_dano+10>=tempo_jogo)
            recuo_por_dano(goblin1,matriz_tela,bloqueios);
        else
            goblin1->levando_dano=0;
    }

    // verificações básicas
    colide_chao(goblin1,matriz_tela,bloqueios,0); // colidiu com o chão? primeira verificação obrigatória
    verificar_queda(goblin1,matriz_tela,bloqueios); // atingiu o limite de pulo? está em queda?
}

void ataque_goblin_arqueiro(Tcriatura *goblin, Tcriatura *guerreiro, int tempo_jogo,Titens *itens,SAMPLE* som_ataque)
{
    if(goblin->direcao==0 && goblin->caracteristicas.hp > 0)
    {
        goblin->caindo=1;
        goblin->pulando=0;
        if(guerreiro->y <= goblin->y + goblin->altura && guerreiro->y+guerreiro->altura >= goblin->y &&
           !itens->todosItens[goblin->id_flecha-1].ativo && goblin->alerta)
        {
            goblin->direcao=goblin->direcao_anterior;

            ataque_ajustes(goblin,tempo_jogo,1,2,3,som_ataque);
            if(goblin->tempo_ataque+10<=tempo_jogo)
            {
                itens->todosItens[goblin->id_flecha-1].direcao=goblin->direcao_anterior;
                itens->todosItens[goblin->id_flecha-1].ativo=1;
                itens->todosItens[goblin->id_flecha-1].x_inicial=goblin->x;
                itens->todosItens[goblin->id_flecha-1].x=goblin->x;
                itens->todosItens[goblin->id_flecha-1].y=goblin->y;
            }
            goblin->direcao=0;
        }
    }
}

void desenhar_goblin_arqueiro(BITMAP *buffer,Tcriatura *goblin1,int ajuste_x,int tempo_jogo)
{
    rectfill(goblin1->sprite,0,0,64,64,makecol(255,0,255));
    if(goblin1->direcao==1)
    {
        draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
    }
    else if(goblin1->direcao==2)
    {
        draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
    }
    else if(goblin1->direcao==0)
    {
        if(goblin1->direcao_anterior==1)
        {
            draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
        }
        else
        {
            draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
        }
    }
    if(goblin1->levando_dano)
    {
        set_trans_blender(255,255,255,50);
        if((tempo_jogo/10)%2==0)
            draw_sprite_ex(buffer, goblin1->sprite, ajuste_x + goblin1->x-(64-goblin1->largura)/2,
                goblin1->y-(64-goblin1->altura)/2,DRAW_SPRITE_TRANS,DRAW_SPRITE_NO_FLIP);
        else
            draw_sprite_ex(buffer, goblin1->sprite, ajuste_x + goblin1->x-(64-goblin1->largura)/2,
                goblin1->y-(64-goblin1->altura)/2,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
    }
    else
        draw_sprite(buffer, goblin1->sprite, ajuste_x + goblin1->x-(64-goblin1->largura)/2,
                goblin1->y-(64-goblin1->altura)/2);
}

void imagens_goblin_chefe(Tcriatura *goblin, int preenchida,DATAFILE* graficos)
{
    int i;
    BITMAP *tiles = (BITMAP*) graficos[GOBLIN_LIDER].dat;

    if(preenchida)
    {
        for(i=0;i<8;i++)
            clear_bitmap(goblin->vetor_sprite[i]);
    }
    else
    {
        for(i=0;i<8;i++)
            goblin->vetor_sprite[i]=create_bitmap(64,64);
    }
    for(i=0;i<8;i++)
        blit(tiles,goblin->vetor_sprite[i],i*64,0,0,0,64,64);

    goblin->face = (BITMAP*) graficos[GOBLIN_FACE].dat;

}

void movimento_goblin_chefe(Tcriatura *goblin1,Tcriatura *guerreiro, int tempo_jogo, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{
    int destino;
    if(goblin1->destino_chefe==1)
        destino=LARGURA_MAPA-100;
    else
        destino=LARGURA_MAPA-550;

    if(!goblin1->levando_dano && goblin1->caracteristicas.hp > 0)
    {
        if(goblin1->estado_sprite < 5 && !goblin1->atacando)
            goblin1->estado_sprite = 5;
        if(goblin1->alerta)
        {
            if(goblin1->estrategia==1)
            {
                if(goblin1->invencivel)
                    goblin1->invencivel=0;

                if (goblin1->x-3 > guerreiro->x+guerreiro->largura || goblin1->x-4 > guerreiro->x+guerreiro->largura || goblin1->x-5 > guerreiro->x+guerreiro->largura
                    || goblin1->x-6 > guerreiro->x+guerreiro->largura)
                {
                    if(!colisao_abaixo_mapa(goblin1->x-goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       || colisao_esquerda_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       || (colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                           && guerreiro->y + guerreiro->altura < goblin1->y))
                        pulo(goblin1,3,-1,matriz_tela,bloqueios);
                    else
                        movimento_esquerda(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                    goblin1->direcao_anterior=2;
                }
                else if (goblin1->x+goblin1->largura+3 < guerreiro->x || goblin1->x+goblin1->largura+4 < guerreiro->x
                         || goblin1->x+goblin1->largura+5 < guerreiro->x || goblin1->x+goblin1->largura+6 < guerreiro->x)
                {
                    if(!colisao_abaixo_mapa(goblin1->x+goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       || colisao_direita_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       || (colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                           && guerreiro->y + guerreiro->altura < goblin1->y))
                        pulo(goblin1,3,1,matriz_tela,bloqueios);
                    else
                        movimento_direita(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                    goblin1->direcao_anterior=1;
                }
                else
                    goblin1->direcao=0;
            }
            else if(goblin1->estrategia==2)
            {
                if(!goblin1->invencivel)
                    goblin1->invencivel=1;
                if (goblin1->x-3 > destino || goblin1->x-4 > destino || goblin1->x-5 > destino)
                {
                    if(!colisao_abaixo_mapa(goblin1->x-goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       || colisao_esquerda_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       || colisao_cima_mapa(goblin1->x-goblin1->caracteristicas.habilidade-40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                        pulo(goblin1,3,-1,matriz_tela,bloqueios);
                    else
                        movimento_esquerda(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                    goblin1->direcao_anterior=(guerreiro->direcao%2)+1;
                }
                else if (goblin1->x+3 < destino || goblin1->x+4 < destino || goblin1->x+5 < destino)
                {
                    if(!colisao_abaixo_mapa(goblin1->x+goblin1->caracteristicas.habilidade,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       || colisao_direita_mapa(goblin1->x,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+2,goblin1->y,goblin1->altura,goblin1->largura,matriz_tela,bloqueios)
                       || colisao_cima_mapa(goblin1->x+goblin1->caracteristicas.habilidade+40,goblin1->y-32,goblin1->altura,goblin1->largura,matriz_tela,bloqueios))
                        pulo(goblin1,3,1,matriz_tela,bloqueios);
                    else
                        movimento_direita(goblin1,goblin1->caracteristicas.habilidade,matriz_tela,bloqueios,1,1,5,7);
                    goblin1->direcao_anterior=(guerreiro->direcao%2)+1;
                }
                else
                    goblin1->direcao=0;

                if(tempo_jogo>=goblin1->tempo_estrategia+3*60+30)
                {
                    if(goblin1->destino_chefe==1)
                        goblin1->destino_chefe=2;
                    else
                        goblin1->destino_chefe=1;
                    goblin1->estrategia=1;
                }

            }

        }
        else
        {
            goblin1->direcao=2;
            goblin1->direcao_anterior=2;

            // radar
            if((guerreiro->x + guerreiro->largura < goblin1->x && goblin1->direcao==2
                 && goblin1->x - (guerreiro->x + guerreiro->largura) <= 150)
               || (goblin1->x + goblin1->largura < guerreiro->x && goblin1->direcao==1
                   && guerreiro->x - (goblin1->x +goblin1->largura) <= 150))
                    goblin1->alerta=1;
        }
    }
    else if(goblin1->levando_dano && (goblin1->caracteristicas.hp > 0 || goblin1->levando_dano))
    {
        goblin1->estado_sprite = 1;
        if(goblin1->tempo_dano+20>=tempo_jogo)
            recuo_por_dano(goblin1,matriz_tela,bloqueios);
        else
        {
            goblin1->levando_dano=0;
            goblin1->estrategia=2;
            goblin1->tempo_estrategia=tempo_jogo;
        }

    }
    if(goblin1->caracteristicas.hp <= 0)
        goblin1->estrategia=1;

    // verificações básicas
    colide_chao(goblin1,matriz_tela,bloqueios,0); // colidiu com o chão? primeira verificação obrigatória
    verificar_queda(goblin1,matriz_tela,bloqueios); // atingiu o limite de pulo? está em queda?
}

void ataque_goblin_chefe(Tcriatura *goblin, Tcriatura *guerreiro, int tempo_jogo,SAMPLE* som_ataque,SAMPLE* som_dano_guerreiro,
                         SAMPLE* som_paralisia)
{
    if(goblin->direcao==0 && goblin->caracteristicas.hp > 0)
    {
        if(goblin->estrategia==1)
        {
            goblin->caindo=1;
            goblin->pulando=0;
            if(guerreiro->y <= goblin->y + goblin->altura && guerreiro->y+guerreiro->altura >= goblin->y)
            {
                goblin->direcao=goblin->direcao_anterior;

                ataque_ajustes(goblin,tempo_jogo,1,2,4,som_ataque);
                if(!guerreiro->levando_dano && goblin->atacando)
                    ataque(goblin,guerreiro,tempo_jogo,0,som_dano_guerreiro);

                goblin->direcao=0;
            }
        }
        else if(goblin->estrategia==2)
        {
            if(tempo_jogo>=goblin->tempo_estrategia+3*60 && !guerreiro->pulando && !guerreiro->caindo && !guerreiro->paralisado)
            {
                play_sample(som_paralisia,255,128,1000,FALSE);
                guerreiro->paralisado=1;
                guerreiro->tempo_paralisado=tempo_jogo;
            }
        }

    }
}

void desenhar_goblin_chefe(BITMAP *buffer,Tcriatura *goblin1,int ajuste_x,int tempo_jogo)
{
    rectfill(goblin1->sprite,0,0,64,64,makecol(255,0,255));
    if(goblin1->direcao==1)
    {
        draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
    }
    else if(goblin1->direcao==2)
    {
        draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
    }
    else if(goblin1->direcao==0)
    {
        if(goblin1->direcao_anterior==1)
        {
            draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_H_FLIP);
        }
        else
        {
            draw_sprite_ex(goblin1->sprite,goblin1->vetor_sprite[goblin1->estado_sprite],0,0,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
        }
    }
    if(goblin1->levando_dano || goblin1->invencivel)
    {
        set_trans_blender(255,255,255,50);
        if((tempo_jogo/10)%2==0)
            draw_sprite_ex(buffer, goblin1->sprite, ajuste_x + goblin1->x-(64-goblin1->largura)/2,
                goblin1->y-(64-goblin1->altura)/2,DRAW_SPRITE_TRANS,DRAW_SPRITE_NO_FLIP);
        else
            draw_sprite_ex(buffer, goblin1->sprite, ajuste_x + goblin1->x-(64-goblin1->largura)/2,
                goblin1->y-(64-goblin1->altura)/2,DRAW_SPRITE_NORMAL,DRAW_SPRITE_NO_FLIP);
    }
    else
        draw_sprite(buffer, goblin1->sprite, ajuste_x + goblin1->x-(64-goblin1->largura)/2,
                goblin1->y-(64-goblin1->altura)/2);
}

void acoes_goblins(Toponentes *inimigos, Tcriatura *guerreiro, int tempo_jogo, int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],
                   Titens *itens,SAMPLE* espada, SAMPLE* besta,SAMPLE* som_dano_guerreiro,SAMPLE* som_paralisia)
{
    int i;

    for(i=0;i<inimigos->goblins_guerreiros.n_goblins;i++)
    {
        movimento_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[i],guerreiro,tempo_jogo,matriz_tela,bloqueios);
        ataque_goblin_guerreiro(&inimigos->goblins_guerreiros.goblins[i],guerreiro,tempo_jogo,espada,som_dano_guerreiro);
    }
    for(i=0;i<inimigos->goblins_arqueiros.n_goblins;i++)
    {
        movimento_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[i],guerreiro,tempo_jogo,matriz_tela,bloqueios);
        ataque_goblin_arqueiro(&inimigos->goblins_arqueiros.goblins[i],guerreiro,tempo_jogo,itens,besta);
    }
    if(inimigos->chefes.chefe_atual!=0)
    {
        movimento_goblin_chefe(&inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1],guerreiro,tempo_jogo,matriz_tela,bloqueios);
        ataque_goblin_chefe(&inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1],guerreiro,tempo_jogo,espada,som_dano_guerreiro,
                            som_paralisia);
    }
}

void desenhar_todos_goblins(Toponentes *inimigos,BITMAP *buffer, int ajuste_mapa,int tempo_jogo)
{
    int i;
    for(i=0;i<inimigos->goblins_guerreiros.n_goblins;i++)
    {
        if(inimigos->goblins_guerreiros.goblins[i].caracteristicas.hp<=0
                && !inimigos->goblins_guerreiros.goblins[i].levando_dano)
            inimigos->goblins_guerreiros.goblins[i].estado_sprite=0;
        desenhar_goblin_guerreiro(buffer,&inimigos->goblins_guerreiros.goblins[i],ajuste_mapa,tempo_jogo); // desenha goblin tipo 1 e manda para o buffer
    }
    for(i=0;i<inimigos->goblins_arqueiros.n_goblins;i++)
    {
        if(inimigos->goblins_arqueiros.goblins[i].caracteristicas.hp<=0
                && !inimigos->goblins_arqueiros.goblins[i].levando_dano)
            inimigos->goblins_arqueiros.goblins[i].estado_sprite=0;
        desenhar_goblin_arqueiro(buffer,&inimigos->goblins_arqueiros.goblins[i],ajuste_mapa,tempo_jogo);
    }
    if(inimigos->chefes.chefe_atual!=0)
    {
        if(inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].caracteristicas.hp<=0
                && !inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].levando_dano)
            inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1].estado_sprite=0;
        desenhar_goblin_chefe(buffer,&inimigos->chefes.chefe[inimigos->chefes.chefe_atual-1],ajuste_mapa,tempo_jogo);
    }
}

int dano_ataque(Tcriatura* atacante, int tipo_ataque)
{
    int ataque;
    if(tipo_ataque==0)
    {
        ataque=atacante->caracteristicas.forca;
    }
    else
    {
        ataque=atacante->caracteristicas.poder_de_fogo;
    }
    ataque+=atacante->caracteristicas.habilidade;
    float a = (rand()%100)/100.0;
    ataque= ataque*(0.8 + a*0.4);
    return ataque;
}


void calcular_dano(Tcriatura* atacante, Tcriatura* alvo,int tipo_ataque)
{
    int ataque = dano_ataque(atacante,tipo_ataque);
    //int forca=atacante->caracteristicas.forca;
    int defesa= alvo->caracteristicas.resistencia + alvo->caracteristicas.armadura;
    //int hp= alvo->caracteristicas.hp;
    float chance_critico=0.1;
    float a = (rand()%100)/100.0;
    int critico = chance_critico > a? 2 : 1;
    int dano;

    dano= (ataque-defesa)*critico;
    dano= dano > 0? dano : 1;

    alvo->caracteristicas.hp-=dano;
}

void movimento_direita(Tcriatura *ser,int deslocamento,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],
                       int mudar_direcao,int mudar_sprite,int sprite_inf,int sprite_sup)
{
    int i;
    for(i=0; i < deslocamento && (ser->x + ser->largura) < LARGURA_MAPA &&
        !colisao_direita_mapa(ser->x, ser->y, ser->altura, ser->largura, matriz_tela, bloqueios);i++)
    {
        ser->x+=1;
    }
    if(mudar_direcao)
        ser->direcao=1;
    if(mudar_sprite)
    {
        if (!ser->pulando)
            mudanca_sprite(sprite_inf,sprite_sup,&ser->estado_sprite,12,0,0);
    }
}

void movimento_esquerda(Tcriatura *ser,int deslocamento,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],
                       int mudar_direcao,int mudar_sprite,int sprite_inf,int sprite_sup)
{
    int i;
    for(i=0; i < deslocamento && ser->x > 0 &&
        !colisao_esquerda_mapa(ser->x, ser->y, ser->altura, ser->largura, matriz_tela, bloqueios);i++)
    {
        ser->x-=1;
    }
    if(mudar_direcao)
        ser->direcao=2;
    if(mudar_sprite)
    {
        if (!ser->pulando)
            mudanca_sprite(sprite_inf,sprite_sup,&ser->estado_sprite,12,0,0);
    }
}

void pulo(Tcriatura *ser,int deslocamentoy,int deslocamentox,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{
    int i;
    for(i=0; i < deslocamentoy && ser->y > ser->nivel_plataforma - ALTURA_PULO && !ser->caindo && ser->permitir_pulo &&
        !colisao_cima_mapa(ser->x, ser->y, ser->altura, ser->largura, matriz_tela, bloqueios);i++)
    {
        ser->pulando=1;
        ser->y-=1;
        if(deslocamentox!=0)
        {
            if(deslocamentox<0)
                movimento_esquerda(ser,(-1)*deslocamentox,matriz_tela,bloqueios,0,0,0,0);
            if(deslocamentox>0)
                movimento_direita(ser,deslocamentox,matriz_tela,bloqueios,0,0,0,0);
        }
    }
}

void recuo(Tcriatura *ser,int deslocamento,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{
    if(ser->tempo_recuo + 20 >= timer && !ser->pulando && !ser->caindo)
    {
        if(ser->direcao==2)
            movimento_direita(ser,deslocamento*3,matriz_tela,bloqueios,0,0,0,0);
        else
            movimento_esquerda(ser,deslocamento*3,matriz_tela,bloqueios,0,0,0,0);
    }
}

void recuo_por_dano(Tcriatura *ser,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{
    ser->estado_sprite=0;
    ser->caindo=1;
    ser->pulando=0;

    // ele recua dependendo da direção que estiver
    if (ser->direcao==2)
        movimento_direita(ser,6,matriz_tela,bloqueios,0,0,0,0);
    else
        movimento_esquerda(ser,6,matriz_tela,bloqueios,0,0,0,0);
}

void mudanca_sprite(int limite_inferior,int limite_superior,int *estado_sprite,int intervalo,int tempo_inicio,int tempo_jogo)
{
    if(*estado_sprite < limite_inferior || *estado_sprite > limite_superior)
        *estado_sprite = limite_inferior;
    if (timer%intervalo==0 && tempo_inicio<=0)
    {
        *estado_sprite = *estado_sprite - limite_inferior;
        *estado_sprite = (*estado_sprite + 1) % (limite_superior-limite_inferior+1);
        *estado_sprite = *estado_sprite + limite_inferior;
    }
    if (tempo_inicio>0)
    {
        *estado_sprite = ((tempo_jogo-tempo_inicio)/intervalo)%(limite_superior-limite_inferior+1);
        *estado_sprite = *estado_sprite + (limite_superior-limite_inferior + 1);
    }
}

void colide_chao(Tcriatura *ser,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3],int eh_heroi)
{
    if(colisao_abaixo_mapa(ser->x, ser->y, ser->altura, ser->largura, matriz_tela, bloqueios))
    {
        ser->caindo=0;
        if(eh_heroi==1)
        {
            if(!key[KEY_UP])ser->nivel_plataforma = ser->y+ser->altura - 1;
        }
        else
            ser->nivel_plataforma = ser->y+ser->altura - 1;
        ser->pulando=0;
        if(eh_heroi==1)
        {
            if(!key[KEY_UP])ser->permitir_pulo=1;
        }
        else
            ser->permitir_pulo=1;
    }
}

void verificar_queda(Tcriatura *ser,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32], int bloqueios[3])
{
    int i;

    if(ser->y <= ser->nivel_plataforma - ALTURA_PULO ||
       (!colisao_abaixo_mapa(ser->x, ser->y, ser->altura, ser->largura, matriz_tela, bloqueios)
        && !ser->pulando)||
       colisao_cima_mapa(ser->x,ser->y,ser->altura,ser->largura,matriz_tela,bloqueios))
    {
        ser->caindo=1;
        ser->permitir_pulo = 0;
    }

    for(i=0;i<5 && ser->caindo && !colisao_abaixo_mapa(ser->x, ser->y, ser->altura, ser->largura, matriz_tela, bloqueios);i++)
    {
        ser->y+=1;
    }
}

void ataque_ajustes(Tcriatura *atacante,int tempo_jogo,int confirmacao,int sprite_lim_inf,int sprite_lim_sup,SAMPLE* som_ataque)
{
    if(!atacante->atacando && confirmacao)
    {
        atacante->atacando = 1;
        atacante->tempo_ataque = tempo_jogo;
        play_sample(som_ataque,255,128,1000,FALSE);
    }
    if(atacante->tempo_ataque + (sprite_lim_sup-sprite_lim_inf+1)*5 <= tempo_jogo)
        atacante->atacando = 0;

    if(atacante->atacando)
        mudanca_sprite(sprite_lim_inf,sprite_lim_sup,&atacante->estado_sprite,5,atacante->tempo_ataque,tempo_jogo);
}

void ataque(Tcriatura *atacante,Tcriatura *alvo,int tempo_jogo,int tipo_at,SAMPLE *som_dano)
{
    if(atacante->atacando && !alvo->levando_dano && !alvo->invencivel)
    {
        if(atacante->direcao==2)//esquerda
        {
            if(colisao(atacante->x + atacante->at_ajusteX,atacante->y + atacante->at_ajusteY,atacante->at_altura,atacante->at_largura,
                       alvo->x,alvo->y,alvo->altura,alvo->largura)&& tempo_jogo-atacante->tempo_ataque > (atacante->quadro_at-1)*5)
               {
                   play_sample(som_dano,255,128,1000,FALSE);
                   alvo->levando_dano=1;
                   alvo->tempo_dano=tempo_jogo;
                   alvo->direcao = 1;
                   calcular_dano(atacante,alvo,tipo_at);
               }
        }
        else //direita
        {
            if(colisao(atacante->x + atacante->largura + ((-1)*(atacante->at_largura+atacante->at_ajusteX)),atacante->y+atacante->at_ajusteY,atacante->at_altura,atacante->at_largura,
                       alvo->x,alvo->y,alvo->altura,alvo->largura )&& tempo_jogo-atacante->tempo_ataque > (atacante->quadro_at-1)*5)
               {
                   play_sample(som_dano,255,128,1000,FALSE);
                   alvo->levando_dano=1;
                   alvo->tempo_dano=tempo_jogo;
                   alvo->direcao = 2;
                   calcular_dano(atacante,alvo,tipo_at);
               }
        }
    }
}

void deixa_item(Titens *itens,Tcriatura *goblin)
{
    int i,chance;
    for(i=0;i<itens->n_itens;i++)
    {
        if(!itens->todosItens[i].ativo && itens->todosItens[i].tipo==1)
        {
            chance=rand()%100;
            if(chance<=15)
            {
                itens->todosItens[i].x=goblin->x;
                itens->todosItens[i].y=goblin->y+goblin->altura-itens->todosItens[i].altura;
                itens->todosItens[i].ativo=1;
            }
        }
    }
}
