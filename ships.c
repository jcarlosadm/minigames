/*
 * ships.c
 *
 *  Módulo que trata do player e das naves inimigas.
 */

#include "ships.h"

/*******************************************
 * Tipos
 *******************************************/


// Parte da nave que descreve seus atributos
typedef struct attributes{
    int power; // poder de tiro
    float speed; // velocidade
}Attributes;

// Parte da nave que determina a sua localização
typedef struct location{
    float position_x; // coordenada x
    float position_y; // coordenada y
}Location;

// Parte da nave que determina a sua dimensão
typedef struct dimension{
    float width; // largura
    float height; // altura
}Dimension;

typedef struct fire{
    char type[20]; // player ou enemy
    char subtype[20]; // subtipo de tiro

    int power;
    int speed;

    int active;
    float x;
    float y;
    ALLEGRO_BITMAP *image; // imagem do tiro
}Fire;

// Descreve a nave, usada pelo inimigo e como base do player
typedef struct ship{
    char type[20]; // player ou enemy
    char subtype[20]; // subtipo de player ou enemy

    Attributes attr; // atributos da nave
    Location location; // coordenadas x e y da nave
    Dimension dimension; // largura e altura da nave

    Fire fire;

    ALLEGRO_BITMAP *image; // imagem da nave
}Ship;

// tipo Ship usado pelo player
typedef struct player_ship{
    Ship* base;
}Player_ship;

/*******************************************
 * Construtores - alocação de memória
 *******************************************/

// define atributos básicos (usado nos construtores)
void build_Attr(Attributes* attr, int power, int speed){
    attr->power = power;
    attr->speed = speed;
}

// inicializa localização e dimensões
void initialize_loc_dim(Location* loc, Dimension* dim, float position_x,
        float position_y, float width, float height){
    loc->position_x = position_x;
    loc->position_y = position_y;
    dim->height = height;
    dim->width = width;
}

// aloca nave inimiga ou base para o player
Ship* new_ship(const char *type, const char *subtype){


    FILE * file;
    mxml_node_t *tree;

    // aloca nave com malloc
    Ship *ship_ = (Ship*) malloc (sizeof(Ship));

    if(ship_ == NULL){
        fprintf(stderr, "falha ao alocar objeto Ship\n");
        return NULL;
    }

    if(strcmp(type,"player")==0){
        file = fopen(XML_PLAYER,"r");
    }
    else if(strcmp(type,"enemy")==0){
        file = fopen(XML_PLAYER,"r");
    }else{
        puts("type deve ser player ou enemy");
        free(ship_);
        return NULL;
    }

    if(!file){
        puts("falha ao tentar abrir o arquivo");
        free(ship_);
        return NULL;
    }

    tree = mxmlLoadFile(NULL,file,MXML_TEXT_CALLBACK);
    if(!tree){
        puts("falha ao tentar criar arvore");
        free(ship_);
        fclose(file);
        return NULL;
    }

    fclose(file);

    mxml_node_t* node = mxmlFindElement(tree,tree,subtype,NULL,NULL,MXML_DESCEND);
    if(!node){
        puts("bitmap nao encontrado");
        mxmlDelete(tree);
        free(ship_);
        return NULL;
    }

    node = mxmlFindElement(node,node,"Attributes",NULL,NULL,MXML_DESCEND);
    build_Attr(&(ship_->attr),atof(mxmlElementGetAttr(node,"power")),
            atof(mxmlElementGetAttr(node,"speed")));
    printf("power %d, speed %.0f \n",ship_->attr.power,ship_->attr.speed);

    node = mxmlWalkNext(node, tree, MXML_DESCEND);
    ship_->image = create_bitmap_from_atlas(mxmlElementGetAttr(node,"name"));

    node = mxmlWalkNext(node, tree, MXML_DESCEND);
    initialize_loc_dim(&(ship_->location),&(ship_->dimension),atof(mxmlElementGetAttr(node,"x")),
                atof(mxmlElementGetAttr(node,"y")), al_get_bitmap_width(ship_->image),
                al_get_bitmap_height(ship_->image));
    printf("x %.0f, y %.0f, w %.0f, h %.0f \n",ship_->location.position_x,ship_->location.position_y,
            ship_->dimension.width,ship_->dimension.height);

    node = mxmlWalkNext(node, tree, MXML_DESCEND);
    ship_->fire.image = create_bitmap_from_atlas(mxmlElementGetAttr(node,"name"));

    mxmlDelete(tree);

    // define os atributos
    //build_Attr(&(ship_->attr),power,speed);
    // define localização e dimensões
    //initialize_loc_dim(&(ship_->location),&(ship_->dimension),position_x,
      //      position_y,al_get_bitmap_width(ship_->image), al_get_bitmap_height(ship_->image));
    // cria uma imagem bitmap

    //ship_->image = create_bitmap_from_atlas("playerShip1_blue.png");

    ship_->fire.active = false;
    //ship_->fire.image = NULL;
    ship_->fire.x = 0;
    ship_->fire.y = 0;

    //al_set_target_bitmap(ship_->image);
    //al_clear_to_color(al_map_rgb(255, 100, 155));
    //set_draw_current_window_game();

    // retorna ponteiro para a nave
    return ship_;
}

// aloca nave do player
Player_ship* new_player_ship(const char *type, const char *subtype){
    // aloca nave com malloc
    Player_ship *ship = (Player_ship*) malloc (sizeof(Player_ship));

    if(ship == NULL){
        fprintf(stderr, "falha ao alocar objeto Player_ship\n");
        return NULL;
    }

    // constrói a base
    ship->base = new_ship(type,subtype);

    if(ship->base == NULL){
        fprintf(stderr, "falha ao alocar base de Player_ship\n");
        free(ship);
        return NULL;
    }

    // retorna player
    return ship;
}

/*************************************************
 * Destrutores - desalocação de memória
 *************************************************/

// desaloca nave inimiga
void dealloc_ship(Ship* ship){

    if(ship){
        // desaloca imagens
        al_destroy_bitmap(ship->image);
        al_destroy_bitmap(ship->fire.image);
        // desaloca com free
        free(ship);
        puts("desalocado ship");
    }

}

// desaloca nave player
void dealloc_player_ship(Player_ship* ship){
    if(ship){
        // desaloca base
        dealloc_ship(ship->base);
        // desaloca com free
        free(ship);
        puts("desalocado ship_player");
    }
}

/*************************************************
 * Funções comuns das naves
 *************************************************/

// imprime poder de tiro
void printPower(Ship* ship){
    // imprime atributo power
    printf("power: %d\n",ship->attr.power);
}

void draw_ship(Ship* ship){
    al_draw_bitmap(ship->image, ship->location.position_x, ship->location.position_y, 0);
    if(ship->fire.active){
        al_draw_bitmap(ship->fire.image, ship->fire.x, ship->fire.y, 0);
    }
}

/**************************************************
 * Funções somente para o player
 *************************************************/

//Pega a nave do jogador e retorna um ponteiro para a sua base
Ship* getBase(Player_ship* ship){
    return ship->base;
}

void player_fire(Player_ship* ship){
    if(!ship->base->fire.active){
        ship->base->fire.active = true;
        //ship->base->fire.image = create_bitmap_from_atlas("laserBlue03.png");
        ship->base->fire.x = ship->base->location.position_x + (al_get_bitmap_width(ship->base->image)/2)
                - al_get_bitmap_width(ship->base->fire.image)/2;
        ship->base->fire.y = ship->base->location.position_y -
                (al_get_bitmap_height(ship->base->fire.image));

        //al_set_target_bitmap(ship->base->fire.image);
        //al_clear_to_color(al_map_rgb(55, 100, 175));
        //set_draw_current_window_game();
    }
}

void update_player_fire(Player_ship* ship){
    if(ship->base->fire.active){
        ship->base->fire.y -= 6;
        if(ship->base->fire.y < 5){
            ship->base->fire.active = false;
            //al_destroy_bitmap(ship->base->fire.image);
            //ship->base->fire.image = NULL;
        }
    }
}

void update_player(Player_ship* ship){

    if(get_mouse_move_state()){
        ship->base->location.position_x = get_mouse_x()-(al_get_bitmap_width(ship->base->image)/2);
        ship->base->location.position_y = get_mouse_y()-(al_get_bitmap_height(ship->base->image)/2);
    }

    if(mouseIsClicked())
        player_fire(ship);

    update_player_fire(ship);
}
