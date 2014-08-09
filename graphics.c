/*
 * graphics.c
 *
 */

#include "graphics.h"

typedef struct bitmap_info{
    float x;
    float y;
    float width;
    float height;
}Bitmap_info;

typedef struct atlas_game{
    ALLEGRO_BITMAP* atlas;
}Atlas_game;

Atlas_game* atlas;

int create_atlas(){
    atlas = (Atlas_game*)malloc(sizeof(Atlas_game));
    if(!atlas){
        fprintf(stderr, "erro ao alocar o atlas\n");
        return false;
    }

    atlas->atlas = al_load_bitmap(ATLAS_NAME);

    if(atlas->atlas == NULL){
        fprintf(stderr, "erro ao alocar a imagem do atlas\n");
        free(atlas);
        return false;
    }

    return true;

}

void desaloc_atlas(){
    if(atlas){
        al_destroy_bitmap(atlas->atlas);
        free(atlas);
        puts("desalocacao do atlas bem sucedida");
    }
}

int load_information_bitmap_from_atlas(const char* bitmap_name, Bitmap_info* info){
    FILE * file;
    mxml_node_t *tree;

    file = fopen(ATLAS_XML,"r");
    if(!file){
        puts("falha ao tentar abrir o arquivo");
        return false;
    }

    tree = mxmlLoadFile(NULL,file,MXML_TEXT_CALLBACK);
    if(!tree){
        puts("falha ao tentar criar arvore");
        fclose(file);
        return false;
    }

    fclose(file);

    mxml_node_t* node = mxmlFindElement(tree,tree,"SubTexture","name",bitmap_name,MXML_DESCEND);
    if(!node){
        puts("bitmap nao encontrado");
        mxmlDelete(tree);
        return false;
    }

    info->x = atof(mxmlElementGetAttr(node,"x"));
    info->y = atof(mxmlElementGetAttr(node,"y"));
    info->width = atof(mxmlElementGetAttr(node,"width"));
    info->height = atof(mxmlElementGetAttr(node,"height"));

    mxmlDelete(tree);

    return true;
}

ALLEGRO_BITMAP* create_bitmap_from_atlas(const char* bitmap_name){
    Bitmap_info info;

    if(!load_information_bitmap_from_atlas(bitmap_name, &info)){
        return NULL;
    }

    printf("%.0f %.0f %.0f %.0f\n",info.x,info.y,info.width,info.height);

    ALLEGRO_BITMAP* bitmap = al_create_bitmap(info.width,info.height);

    al_set_target_bitmap(bitmap);

    al_draw_bitmap_region(atlas->atlas,info.x,info.y,info.width,info.height,0,0,0);

    set_draw_current_window_game();

    return bitmap;

}

