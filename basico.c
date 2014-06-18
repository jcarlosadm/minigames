#include "basico.h"
#include "controle.h"
#include "mapa.h"
#include "eventos.h"

int posicaoX_logo;
int posicaoY_logo;

// função de saída
void fecha_programa(){exit_program = TRUE;}
END_OF_FUNCTION(fecha_programa)

//  função tempo
void incrementa_timer(){timer++;}
END_OF_FUNCTION(incrementa_timer)

void init()
{
    srand(time(NULL));
    int res;
    allegro_init();

    set_color_depth(32);
    res = set_gfx_mode(MODO_SCREEN, LARGURA_SCREEN , ALTURA_SCREEN , 0 , 0 ); // cria tela 640x480

    if (res != 0 ) // verifica se houve erro na criação da tela
    {
        allegro_message(allegro_error);
        exit( -1 );
    }
    // instalação do timer, teclado e mouse
    install_timer();
    install_keyboard();
    install_mouse();
    install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);

    opacidade=0;
}

void deinit()
{
    clear_keybuf(); // libera espaço
}

int colisao(float ax,float ay, float ah, float al, float bx, float by, float bh, float bl)
{
    if((ax+al)<bx || ax>(bx+bl) || ay>(by+bh) || (ay+ah)<by)
    {
       return 0;
    }
    return 1;
}

void desenhar_retangulo(BITMAP *buffer,int pos_x,int pos_y,int largura,int altura,int opacidade)
{
    drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
    set_trans_blender(255,0,0,opacidade);
    rectfill(buffer,pos_x,pos_y,pos_x+largura,pos_y+altura,makecol(0,0,160));
    rectfill(buffer,pos_x+5,pos_y+5,pos_x+largura-5,pos_y+altura-5,makecol(45,145,255));
    solid_mode();
}

void janela_texto(BITMAP *buffer,int pos_x,int pos_y,int largura,int altura,
                  char texto_titulo[256],char texto_corpo[256], FONT* titulo,FONT* corpo,int opacidade,
                  int inicio,int fim,int tempo_jogo,int efeito)
{
    char texto1[31],texto2[31],texto3[31];
    int menor, menor_anterior=0,i;

    // tratamento de texto linha 1
    menor=strlen(texto_corpo);
    if(menor>30)
        menor=30;
    if(texto_corpo[menor]!=0)
    {
        while(texto_corpo[menor]!=' ' && menor>=0)
            menor-=1;
        if(menor==0)
            menor=strlen(texto_corpo);
        if(menor>30)
            menor=30;
    }
    for(i=0;i<menor;i++)
        texto1[i]=texto_corpo[i];
    texto1[i]=0;
    menor_anterior+=menor;

    // tratamento de texto linha 2
    if(strlen(texto_corpo)-menor_anterior+1 > 0)
    {
        if(strlen(texto_corpo)>menor_anterior)
            menor=strlen(texto_corpo)-menor_anterior;
        if(menor>30)
            menor=30;
        if(texto_corpo[menor+menor_anterior]!=0)
        {
            while(texto_corpo[menor+menor_anterior]!=' ' && menor>=0)
                menor-=1;
            if(menor==0)
            {
                if(strlen(texto_corpo)>menor_anterior)
                    menor=strlen(texto_corpo)-menor_anterior;
            }
            if(menor>30)
                menor=30;
        }
        for(i=0;i<menor;i++)
            texto2[i]=texto_corpo[i+menor_anterior];
        texto2[i]=0;
        menor_anterior+=menor;
    }
    else
        texto2[0]=0;


    // tratamento de texto linha 3
    if(strlen(texto_corpo)-menor_anterior > 0 && strlen(texto_corpo)>30)
    {
        if(strlen(texto_corpo)>menor_anterior)
        menor=strlen(texto_corpo)-menor_anterior;
        if(menor>30)
            menor=30;
        for(i=0;i<menor;i++)
            texto3[i]=texto_corpo[i+menor_anterior];
        texto3[i]=0;
    }
    else
        texto3[0]=0;

    if(tempo_jogo>=inicio)
    {
        if (tempo_jogo<inicio+20 && efeito)
            desenhar_retangulo(buffer,pos_x,pos_y,(largura/20)*(tempo_jogo-inicio),(altura/20)*(tempo_jogo-inicio),opacidade);
        else if((tempo_jogo > (fim-60)+40)&&(fim!=-1))
            desenhar_retangulo(buffer,pos_x,pos_y,(largura/20)*(fim-tempo_jogo),(altura/20)*(fim-tempo_jogo),opacidade);
        else
        {
            desenhar_retangulo(buffer,pos_x,pos_y,largura,altura,opacidade);
            textprintf_ex(buffer,titulo,pos_x+10,pos_y+10,makecol(0,0,150),-1,"%s",texto_titulo);
            textprintf_ex(buffer,corpo,pos_x+10,pos_y+30,makecol(250,250,250),-1,"%s",texto1);
            textprintf_ex(buffer,corpo,pos_x+10,pos_y+50,makecol(250,250,250),-1,"%s",texto2);
            textprintf_ex(buffer,corpo,pos_x+10,pos_y+70,makecol(250,250,250),-1,"%s",texto3);
        }
    }
}

void janela_variavel(BITMAP *buffer,int pos_x,int pos_y,int largura,int altura,int variavel, FONT* fonte,int opacidade)
{
    desenhar_retangulo(buffer,pos_x,pos_y,largura,altura,opacidade);
    textprintf_ex(buffer,fonte,pos_x+15,pos_y+10,makecol(0,0,150),-1,"%d",variavel);
}

void janela_dialogo(BITMAP *buffer,Tcriatura *personagem,int pos_x,int pos_y,FONT* titulo,FONT* corpo,
                    int inicio, int fim, int tempo_jogo,char texto_titulo[50],char texto_corpo[256],int efeito)
{
    // gera duas janela de texto, uma sem texto e outra com texto
    // se efeito==1, haverá um efeito de abertura da janela
    janela_texto(buffer,pos_x,pos_y,106,106,"","",titulo,corpo,150,inicio,fim, tempo_jogo,efeito);
    janela_texto(buffer,pos_x+107,pos_y,300,106,texto_titulo,texto_corpo,titulo,corpo,150,inicio,fim, tempo_jogo,efeito);
    // se houver efeito, então espera o efeito de abertura para desenha foto do personagem e botão no canto
    // o tempo final da janela deve estar indefinido (-1)
    if(efeito)
    {
        if(fim==-1 && tempo_jogo>inicio+20)
        {
            // desenha rosto do personagem
            draw_sprite(buffer,personagem->face,pos_x+5,pos_y+5);
            // desenha botão animado no canto
            if((tempo_jogo/16)%2==0)
                rectfill(buffer,pos_x+380,pos_y+80,pos_x+380+10,pos_y+80+10,makecol(200,0,0));
            else
                rectfill(buffer,pos_x+380,pos_y+70,pos_x+380+10,pos_y+70+10,makecol(200,0,0));
        }
    }
    else // se não houver efeito, desenha imediatamente
    {
        if(fim==-1)
        {
            draw_sprite(buffer,personagem->face,pos_x+5,pos_y+5);
            if((tempo_jogo/16)%2==0)
            {
                rectfill(buffer,pos_x+380,pos_y+80,pos_x+380+10,pos_y+80+10,makecol(200,0,0));
            }
            else
            {
                rectfill(buffer,pos_x+380,pos_y+70,pos_x+380+10,pos_y+70+10,makecol(200,0,0));
            }
        }
    }

}

void preencher_janela(Tjanela *janela_atual,float x, float y, int altura, int largura, int controle, int tempo_inicio,
                      int tempo_fim, char titulo[30],char conteudo[256],int ator)
{
    janela_atual->x = x;
    janela_atual->y = y;
    janela_atual->altura = altura;
    janela_atual->largura = largura;
    janela_atual->controle = controle;
    janela_atual->tempo_inicio = tempo_inicio;
    janela_atual->tempo_fim = tempo_fim;
    strcpy(janela_atual->titulo,titulo);
    strcpy(janela_atual->conteudo,conteudo);
    janela_atual->ator = ator;
}

void menu_inicial (BITMAP *buffer, int *selecionar, BITMAP *menu_iniciar, BITMAP *menu_options, BITMAP *menu_exit,BITMAP *menu_creditos,
                    int *loading_time, int *tela,int *estagio_loading,int *tela_destino, int *fase, int *carrega_fase, int *tempo_jogo,
                    SAMPLE* selecao,SAMPLE* confirmar,int *tocando,MIDI* musica)
{
    if(*tocando==0)
    {
        *tocando=1;
        play_midi(musica,TRUE);
    }

    if(apertou(KEY_DOWN))
    {
        *selecionar = (*selecionar+1)%4;
        play_sample(selecao,255,128,1000,FALSE);
    }


    else if(apertou(KEY_UP))
    {
        if (*selecionar != 0)
            *selecionar = (*selecionar-1)%4;
        else if (*selecionar == 0)
            *selecionar = 3;
        play_sample(selecao,255,128,1000,FALSE);
    }

    if (*selecionar == 0)
        draw_sprite(buffer, menu_iniciar, 0, 0);
    else if (*selecionar == 1)
        draw_sprite(buffer, menu_options, 0, 0);
    else if(*selecionar == 2)
        draw_sprite(buffer, menu_creditos,0,0);
    else if (*selecionar == 3)
        draw_sprite(buffer, menu_exit, 0, 0);

    draw_sprite (screen, buffer,0, 0);

    if(apertou(KEY_ENTER) || apertou(KEY_SPACE))
    {
        if(*selecionar==0)
        {
            *tela = 9;
            *loading_time = timer;
            *estagio_loading = 0;
            *tela_destino=1;
            *carrega_fase=1;
            *fase=1;
            posicaoX_logo=10;
            posicaoY_logo=10;
            *tempo_jogo=0;
            play_sample(confirmar,255,128,1000,FALSE);
            stop_midi();
            *tocando=0;
        }
        else if(*selecionar==1)
        {
            *tela=2;
            play_sample(confirmar,255,128,1000,FALSE);
        }
        else if(*selecionar==2)
        {
            *tela=3;
            play_sample(confirmar,255,128,1000,FALSE);
        }
        else if(*selecionar==3)
        {
            play_sample(confirmar,255,128,1000,FALSE);
            rest(400);
            fecha_programa();
        }

    }
}

void tela_carregamento (BITMAP *buffer, BITMAP *tela_loading[4], int *loading_time,int tela_destino, int *tela,BITMAP *logo)
{
    if(timer%16==0)
    {
        posicaoX_logo = rand()%(SCREEN_W-250);
        posicaoY_logo = rand()%(SCREEN_H-55-130);
    }

    if((timer/16)%4 == 0)
        draw_sprite(buffer, tela_loading[0], 0, 0);
    else if((timer/16)%4 == 1)
        draw_sprite(buffer, tela_loading[1], 0, 0);
    else if((timer/16)%4 == 2)
        draw_sprite(buffer, tela_loading[2], 0, 0);
    else if((timer/16)%4 == 3)
        draw_sprite(buffer, tela_loading[3], 0, 0);

    draw_sprite(buffer,logo,posicaoX_logo,posicaoY_logo);

    rectfill(buffer,(SCREEN_W/2)-75,350,(SCREEN_W/2)+75, 350+25, makecol(255,0,0));
    rectfill(buffer,(SCREEN_W/2)-70,355,((SCREEN_W/2)-70)+(((timer-*loading_time)*1.0/(TEMPO_LOADING))*140),355+15, makecol(0,0,160));
    draw_sprite (screen, buffer,0, 0);
    if(*loading_time+TEMPO_LOADING <= timer)
        *tela = tela_destino;
    if(tela_destino==1)
    {
        opacidade=254;
        inicio_fase=1;
        fim_fase=0;
    }

}

void carrega_elementos_fase(int *carrega_fase,int *estagio_loading,int matriz_tela[ALTURA_MAPA/32][LARGURA_MAPA/32],
                            char nome_fase[N_FASES][10],int fase,Titens *itens, Tcriatura *guerreiro,Toponentes *inimigos,
                            Tjanelas *janelas, BITMAP *background,BITMAP *texturas[MAX_TERRENOS],Teventos *eventos,
                            BITMAP *mapa,DATAFILE* graficos)
{
    if(*carrega_fase)
    {
        switch(*estagio_loading)
        {
        case 0:
            prepara_mapa(matriz_tela, nome_fase[fase-1]); // preenche matriz com os tilesets corretos
            *estagio_loading=*estagio_loading+1;
            break;
        case 1:
            carregar_var_fase(fase,itens,guerreiro,inimigos,janelas,background,texturas,eventos,graficos);
            *estagio_loading=*estagio_loading+1;
            break;
        case 2:
            carrega_mapa(mapa,texturas,matriz_tela); // cria mapa com as texturas
            *estagio_loading=*estagio_loading+1;
            *carrega_fase=0;
            break;
        }
    }
}

void pause_menu(int *pause, Teventos *eventos, BITMAP *buffer,int *selecionar,int *tela,int tempo_jogo,int *tela_destino,
                int *loading_time,SAMPLE* selecao,SAMPLE* confirmar,int *tocando)
{
    if(*pause && eventos->evento_atual==0)
    {
        if(opacidade==0)
            opacidade=50;
        if(opacidade<170)
            opacidade++;

        // coloca tela cinza por cima da tela atual
        drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
        set_trans_blender(255,0,0,opacidade);
        rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(160,160,160));

        // desenha tela de opções
        rectfill(buffer,SCREEN_W/2-80,SCREEN_H/2-50,SCREEN_W/2+80,SCREEN_H/2+70,makecol(255,0,0));
        rectfill(buffer,SCREEN_W/2-80+5,SCREEN_H/2-50+5,SCREEN_W/2+80-5,SCREEN_H/2+70-5,makecol(0,0,255));

        solid_mode();

        textout_ex(buffer,font,"Continue",SCREEN_W/2-100+55,SCREEN_H/2-50+30,makecol(255,255,255),-1);
        textout_ex(buffer,font,"Tela inicial",SCREEN_W/2-100+55,SCREEN_H/2-50+50,makecol(255,255,255),-1);
        textout_ex(buffer,font,"Sair do jogo",SCREEN_W/2-100+55,SCREEN_H/2-50+70,makecol(255,255,255),-1);

        if((timer/30)%2==0)
            textprintf_ex(buffer,font,SCREEN_W/2-100+80,SCREEN_H/2-50+100,makecol(255,255,255),-1,"%d:%d:%d",
                          tempo_jogo/(60*60*60),(tempo_jogo/(60*60))%60,(tempo_jogo/60)%60);
        else
            textprintf_ex(buffer,font,SCREEN_W/2-100+80,SCREEN_H/2-50+100,makecol(255,255,255),-1,"%d %d %d",
                          tempo_jogo/(60*60*60),(tempo_jogo/(60*60))%60,(tempo_jogo/60)%60);

        int posicao_y_cursor;

        if(apertou(KEY_DOWN))
        {
            *selecionar=(*selecionar+1)%3;
            play_sample(selecao,255,128,1000,FALSE);
        }

        else if(apertou(KEY_UP))
        {
            if(*selecionar==0)
                *selecionar=2;
            else
                *selecionar=(*selecionar-1)%3;
            play_sample(selecao,255,128,1000,FALSE);
        }

        if(*selecionar==0)
            posicao_y_cursor=SCREEN_H/2-50+30;
        else if(*selecionar==1)
            posicao_y_cursor=SCREEN_H/2-50+50;
        else
            posicao_y_cursor=SCREEN_H/2-50+70;

        if((timer/16)%2==0)
        {
            rectfill(buffer,SCREEN_W/2-100+35,posicao_y_cursor,SCREEN_W/2-100+45,posicao_y_cursor+10,makecol(255,0,0));
            rectfill(buffer,SCREEN_W/2-100+37,posicao_y_cursor-2,SCREEN_W/2-100+47,posicao_y_cursor+10-2,makecol(180,0,0));
        }
        else
            rectfill(buffer,SCREEN_W/2-100+35,posicao_y_cursor,SCREEN_W/2-100+45,posicao_y_cursor+10,makecol(180,0,0));

        if(apertou(KEY_ENTER)|| apertou(KEY_SPACE))
        {
            opacidade=0;
            play_sample(confirmar,255,128,1000,FALSE);
            if(*selecionar==0)
                *pause=FALSE;
            else if(*selecionar==1)
            {
                *pause=FALSE;
                *selecionar=0;
                *tela=9;
                *tela_destino=0;
                *loading_time=timer-60;
                stop_midi();
                *tocando=0;
                set_volume(volume,volume);
            }
            else
            {
                rest(400);
                fecha_programa();
            }
        }
    }
}

void game_over(int *pause, Teventos *eventos, BITMAP *buffer,int *selecionar,int *tela,int tempo_jogo,int *tela_destino,
                int *loading_time,SAMPLE* selecao,SAMPLE* confirmar,int *tocando,MIDI* musica_game_over,Tcriatura *guerreiro,
                int *estagio_loading,int *tocando_game_over,int *carrega_fase,DATAFILE* graficos)
{
    if(guerreiro->caracteristicas.hp<=0)
    {
        if(opacidade==0)
            opacidade=40;
        if(opacidade<255)
            opacidade++;

        // toca a música de game over
        if(*tocando_game_over==0)
        {
            play_midi(musica_game_over,FALSE);
            *tocando_game_over=1;
        }

        // configura para um evento improvável, apenas para pausar sem chamar o menu de pause
        eventos->evento_atual=10;
        // pausa o jogo
        *pause = 1;

        // coloca tela cinza por cima da tela atual
        drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
        set_trans_blender(255,0,0,opacidade);
        rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(0,0,0));

        BITMAP* imagem_gameOver = (BITMAP*) graficos[GAME_OVER].dat;
        draw_sprite_ex(buffer,imagem_gameOver,0,0,DRAW_SPRITE_TRANS,DRAW_SPRITE_NO_FLIP);

        // desenha tela de opções
        rectfill(buffer,SCREEN_W/2-93,SCREEN_H/2-50,SCREEN_W/2+67,SCREEN_H/2+70,makecol(255,0,0));
        rectfill(buffer,SCREEN_W/2-93+5,SCREEN_H/2-50+5,SCREEN_W/2+67-5,SCREEN_H/2+70-5,makecol(0,0,255));

        solid_mode();

        //textout_ex(buffer,font,"Game Over",SCREEN_W/2-113+75,SCREEN_H/2-50+10,makecol(255,255,255),-1);

        textout_ex(buffer,font,"Continue",SCREEN_W/2-113+55,SCREEN_H/2-50+30,makecol(255,255,255),-1);
        textout_ex(buffer,font,"Tela inicial",SCREEN_W/2-113+55,SCREEN_H/2-50+50,makecol(255,255,255),-1);
        textout_ex(buffer,font,"Sair do jogo",SCREEN_W/2-113+55,SCREEN_H/2-50+70,makecol(255,255,255),-1);

        if((timer/30)%2==0)
            textprintf_ex(buffer,font,SCREEN_W/2-113+80,SCREEN_H/2-50+100,makecol(255,255,255),-1,"%d:%d:%d",
                          tempo_jogo/(60*60*60),(tempo_jogo/(60*60))%60,(tempo_jogo/60)%60);
        else
            textprintf_ex(buffer,font,SCREEN_W/2-113+80,SCREEN_H/2-50+100,makecol(255,255,255),-1,"%d %d %d",
                          tempo_jogo/(60*60*60),(tempo_jogo/(60*60))%60,(tempo_jogo/60)%60);

        int posicao_y_cursor;

        if(apertou(KEY_DOWN))
        {
            *selecionar=(*selecionar+1)%3;
            play_sample(selecao,255,128,1000,FALSE);
        }

        else if(apertou(KEY_UP))
        {
            if(*selecionar==0)
                *selecionar=2;
            else
                *selecionar=(*selecionar-1)%3;
            play_sample(selecao,255,128,1000,FALSE);
        }

        if(*selecionar==0)
            posicao_y_cursor=SCREEN_H/2-50+30;
        else if(*selecionar==1)
            posicao_y_cursor=SCREEN_H/2-50+50;
        else
            posicao_y_cursor=SCREEN_H/2-50+70;

        if((timer/16)%2==0)
        {
            rectfill(buffer,SCREEN_W/2-113+35,posicao_y_cursor,SCREEN_W/2-113+45,posicao_y_cursor+10,makecol(255,0,0));
            rectfill(buffer,SCREEN_W/2-113+37,posicao_y_cursor-2,SCREEN_W/2-113+47,posicao_y_cursor+10-2,makecol(180,0,0));
        }
        else
            rectfill(buffer,SCREEN_W/2-113+35,posicao_y_cursor,SCREEN_W/2-113+45,posicao_y_cursor+10,makecol(180,0,0));

        if(apertou(KEY_ENTER)|| apertou(KEY_SPACE))
        {
            opacidade=0;
            play_sample(confirmar,255,128,1000,FALSE);
            if(*selecionar==0)
            {
                *pause=FALSE;
                *tela = 9;
                *loading_time = timer;
                *estagio_loading = 0;
                *tela_destino=1;
                *carrega_fase=1;
                posicaoX_logo=10;
                posicaoY_logo=10;
                play_sample(confirmar,255,128,1000,FALSE);
                stop_midi();
                *tocando=0;
                set_volume(volume,volume);
            }
            else if(*selecionar==1)
            {
                *pause=FALSE;
                *selecionar=0;
                *tela=9;
                *tela_destino=0;
                *loading_time=timer-60;
                stop_midi();
                *tocando=0;
                set_volume(volume,volume);
            }
            else
            {
                rest(400);
                fecha_programa();
            }
        }
    }
    else
    {
        *tocando_game_over=0;
    }
}

void menu_opcoes(BITMAP* buffer, DATAFILE* graficos,SAMPLE* selecao,SAMPLE* confirmacao,int *tela)
{

    if(apertou(KEY_RIGHT) && volume<=225)
    {
        volume+=25;
        play_sample(selecao,volume,128,1000,FALSE);
        set_volume(volume,volume);
    }
    else if(apertou(KEY_LEFT) && volume>=25)
    {
        volume-=25;
        play_sample(selecao,volume,128,1000,FALSE);
        set_volume(volume,volume);
    }
    else if(apertou(KEY_ESC) || apertou(KEY_ENTER) || apertou(KEY_SPACE))
    {
        play_sample(confirmacao,volume,128,1000,FALSE);
        *tela = 0;
    }

    float largura=volume*1.112;

    rectfill(buffer,228,37,506,68,makecol(15,66,2));
    rectfill(buffer,228,37,1.0*228+largura,68,makecol(250-volume/2,0,0));

    draw_sprite(buffer,(BITMAP*)graficos[OPCOES].dat,0,0);
    draw_sprite(screen,buffer,0,0);
}
void creditos(BITMAP* buffer,DATAFILE* graficos,SAMPLE *confirmacao,int *tela)
{
    if(apertou(KEY_ESC) || apertou(KEY_ENTER)|| apertou(KEY_SPACE))
    {
        play_sample(confirmacao,volume,128,1000,FALSE);
        *tela = 0;
    }

    draw_sprite(buffer,(BITMAP*)graficos[CREDITOS].dat,0,0);
    draw_sprite(screen,buffer,0,0);
}
