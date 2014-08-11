/*
 * graphics.c
 * Módulo que gerencia o atlas e a criação de bitmaps e spritesheets
 *
 */

#include "graphics.h"


/*************************************************************
 * ***********************************************************
 * Estruturas
 * ***********************************************************
 *************************************************************/

/* ------------------------------------------------------
 * Estrutura de informações do bitmap retirado do atlas
 * ------------------------------------------------------
 * Membros:
 * float position_x : coordenada x do bitmap no atlas.
 * float position_y : coordenada y do bitmap no atlas.
 * float width : largura do bitmap no atlas.
 * float height : altura do bitmap no atlas.
 --------------------------------------------------------*/
typedef struct bitmap_info{
    float position_x;
    float position_y;
    float width;
    float height;
}Bitmap_info;

/* -------------------------------------------------
 * Estrutura relativa ao atlas do jogo
 * -------------------------------------------------
 * Membros:
 * ALLEGRO_BITMAP* atlas : imagem do atlas
 ---------------------------------------------------*/
typedef struct atlas_game{
    // objetos allegro
    ALLEGRO_BITMAP* atlas;
}Atlas_game;

typedef struct graphics_game{
    Atlas_game* atlas;
}Graphics_game;

/***************************************************************
 * *************************************************************
 * Construtores
 * *************************************************************
 ***************************************************************/

/* -------------------------------------------------------------
 * Carrega e aloca o atlas do jogo
 * -------------------------------------------------------------
 * Retorna false se não conseguir
 * -------------------------------------------------------------
 * Parâmetros:
 * Atlas_game** atlas : ponteiro*** para Atlas_game
 ---------------------------------------------------------------*/
int create_atlas(Graphics_game** graphics){
    // tenta alocar objeto atlas
    (*graphics)->atlas = malloc(sizeof(Atlas_game));
    // se não conseguir, informa e retorna false
    if(!(*graphics)->atlas){
        fprintf(stderr, "erro ao alocar o atlas\n");
        return false;
    }

    // tenta alocar objeto ALLEGRO_BITMAP do allegro
    (*graphics)->atlas->atlas = al_load_bitmap(ATLAS_NAME);
    // se não conseguir, informa, desaloca atlas e retorna false
    if((*graphics)->atlas->atlas == NULL){
        fprintf(stderr, "erro ao alocar a imagem do atlas\n");
        free((*graphics)->atlas);
        ((*graphics)->atlas) = NULL;
        return false;
    }

    // operações bem sucedidas
    return true;

}

int create_graphics(Graphics_game** graphics){

    (*graphics) = malloc(sizeof(Graphics_game));
    if(!(*graphics)){
        puts("falha ao tentar alocar objeto graphics");
        return false;
    }

    if(!create_atlas(graphics)){
        free(*graphics);
        (*graphics) = NULL;
        return false;
    }

    return true;
}


/************************************************************
 * **********************************************************
 * Destrutores
 * **********************************************************
 ************************************************************/

/* ----------------------------------------------------------
 * Desaloca objeto atlas
 * ----------------------------------------------------------
 * Parâmetros:
 * Atlas_game** atlas : ponteiro** para Atlas_game
 ------------------------------------------------------------*/
void dealloc_atlas(Graphics_game** graphics){
    // se o objeto atlas estiver alocado...
    if((*graphics)->atlas){
        // destroi o objeto ALLEGRO_BITMAP
        al_destroy_bitmap((*graphics)->atlas->atlas);
        // desaloca objeto atlas
        free((*graphics)->atlas);
        ((*graphics)->atlas) = NULL;
        // informa
        puts("desalocacao do atlas bem sucedida");
    }
}

void dealloc_graphics(Graphics_game** graphics){

    if(!(*graphics)) return;

    dealloc_atlas(graphics);

    free(*graphics);

    (*graphics) = NULL;
}


/**********************************************************************************
 * ********************************************************************************
 * Funções de graphics
 * ********************************************************************************
 **********************************************************************************/

/* ---------------------------------------------------------------------------------
 * Obtém informação de um bitmap do atlas
 * ---------------------------------------------------------------------------------
 * Retorna false se o bitmap não existir ou ocorrer algum erro (esse erro é reportado)
 * ---------------------------------------------------------------------------------
 * Parâmetros:
 * const char* bitmap_name : nome do bitmap
 * Bitmap_info* info : estrutura a ser preenchida com informações do bitmap
 -----------------------------------------------------------------------------------*/
int load_information_bitmap_from_atlas(const char* bitmap_name, Bitmap_info* info){

    /*
     * Declaração de variáveis locais
     */
    // arquivo
    FILE * file;
    // guardará informações do arquivo xml do atlas
    mxml_node_t *tree;

    // tenta abrir o arquivo xml
    file = fopen(ATLAS_XML,"r");
    // se não conseguir, informa e sai com false
    if(!file){
        puts("falha ao tentar abrir o arquivo");
        return false;
    }

    // a variável tree é preechida com informações desse arquivo
    tree = mxmlLoadFile(NULL,file,MXML_TEXT_CALLBACK);
    // se não conseguir preencher tree, informa, fecha arquivo e sai com false
    if(!tree){
        puts("falha ao tentar criar arvore");
        fclose(file);
        return false;
    }

    // fecha arquivo
    fclose(file);

    // tenta encontrar o bitmap em tree
    mxml_node_t* node = mxmlFindElement(tree,tree,"SubTexture","name",bitmap_name,MXML_DESCEND);
    // se não encontrar, informa, desaloca tree e sai com false
    if(!node){
        puts("bitmap nao encontrado");
        mxmlDelete(tree);
        return false;
    }

    // preenche informações da estrutura info
    info->position_x = atof(mxmlElementGetAttr(node,"x"));
    info->position_y = atof(mxmlElementGetAttr(node,"y"));
    info->width = atof(mxmlElementGetAttr(node,"width"));
    info->height = atof(mxmlElementGetAttr(node,"height"));

    // desaloca tree
    mxmlDelete(tree);

    // operações bem sucedidas
    return true;
}

/* -----------------------------------------------------------------------------------------
 * Cria e retorna uma imagem de acordo com a string literal fornecida
 * -----------------------------------------------------------------------------------------
 * Essa função retornará NULL se o bitmap não existir no atlas
 * -----------------------------------------------------------------------------------------
 * Parâmetros:
 * const char* bitmap_name : nome do bitmap a ser retornado
 * Window_game** window : ponteiro** para Window_game
 * Atlas_game** atlas : ponteiro** para Atlas_game
 -------------------------------------------------------------------------------------------*/
ALLEGRO_BITMAP* create_bitmap_from_atlas(const char* bitmap_name,Window_game** window,
        Graphics_game** graphics){

    // estrutura que conterá informações do bitmap
    Bitmap_info info;

    // procura informações do bitmap, e se não conseguir retorna NULL
    if(!load_information_bitmap_from_atlas(bitmap_name, &info)){
        return NULL;
    }

    // apenas imprime informações localizadas
    printf("image info: x %.0f, y %.0f, w %.0f, h %.0f\n",info.position_x,info.position_y,info.width,info.height);

    // cria um objeto ALLEGRO_BITMAP
    ALLEGRO_BITMAP* bitmap = al_create_bitmap(info.width,info.height);

    // coloca o bitmap criado como local de desenho
    al_set_target_bitmap(bitmap);

    // desenha parte do atlas no bitmap criado de acordo com as informações pesquisadas
    al_draw_bitmap_region((*graphics)->atlas->atlas,info.position_x,info.position_y,info.width,info.height,0,0,0);

    // retorna a janela de jogo como local de desenho
    set_draw_current_window_game(window);

    // retorna o bitmap criado
    return bitmap;

}

