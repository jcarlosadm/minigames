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

typedef struct bullet{
    char type[20]; // player ou enemy
    char subtype[20]; // subtipo de tiro

    int power;
    float speed;

    int active;
    float x;
    float y;
    ALLEGRO_BITMAP *image; // imagem do tiro
}Bullet;

// Descreve a nave, usada pelo inimigo e como base do player
typedef struct ship{
    char type[20]; // player ou enemy
    char subtype[20]; // subtipo de player ou enemy
    char bullet_subtype[20]; // subtipo do seu tiro

    Attributes attr; // atributos da nave
    Location location; // coordenadas x e y da nave
    Dimension dimension; // largura e altura da nave

    ALLEGRO_BITMAP *image; // imagem da nave
}Ship;

// tipo Ship usado pelo player
typedef struct player_ship{
    Ship* base;
}Player_ship;

/********************************************
 * Objetos do módulo
 ********************************************/

// player
Player_ship* player;

// lista de bullets ativos
Bullet* bullet;

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
    Ship *ship = (Ship*) malloc (sizeof(Ship));

    if(ship == NULL){
        fprintf(stderr, "falha ao alocar objeto Ship\n");
        return NULL;
    }

    strcpy(ship->type,type);
    strcpy(ship->subtype,subtype);

    if(strcmp(type,"player")==0){
        file = fopen(XML_PLAYER,"r");
    }
    else if(strcmp(type,"enemy")==0){
        file = fopen(XML_ENEMIES,"r");
    }else{
        puts("type deve ser player ou enemy");
        free(ship);
        return NULL;
    }

    if(!file){
        puts("falha ao tentar abrir o arquivo");
        free(ship);
        return NULL;
    }

    tree = mxmlLoadFile(NULL,file,MXML_TEXT_CALLBACK);
    if(!tree){
        puts("falha ao tentar criar arvore");
        free(ship);
        fclose(file);
        return NULL;
    }

    fclose(file);

    mxml_node_t* node = mxmlFindElement(tree,tree,subtype,NULL,NULL,MXML_DESCEND);
    if(!node){
        puts("subtipo nao encontrado");
        mxmlDelete(tree);
        free(ship);
        return NULL;
    }

    node = mxmlFindElement(node,node,"Attributes",NULL,NULL,MXML_DESCEND);
    build_Attr(&(ship->attr),atof(mxmlElementGetAttr(node,"power")),
            atof(mxmlElementGetAttr(node,"speed")));
    printf("power %d, speed %.0f \n",ship->attr.power,ship->attr.speed);

    node = mxmlWalkNext(node, tree, MXML_DESCEND);
    ship->image = create_bitmap_from_atlas(mxmlElementGetAttr(node,"name"));

    node = mxmlWalkNext(node, tree, MXML_DESCEND);
    initialize_loc_dim(&(ship->location),&(ship->dimension),atof(mxmlElementGetAttr(node,"x")),
                atof(mxmlElementGetAttr(node,"y")), al_get_bitmap_width(ship->image),
                al_get_bitmap_height(ship->image));
    printf("x %.0f, y %.0f, w %.0f, h %.0f \n",ship->location.position_x,ship->location.position_y,
            ship->dimension.width,ship->dimension.height);

    node = mxmlWalkNext(node, tree, MXML_DESCEND);
    strcpy(ship->bullet_subtype,mxmlElementGetAttr(node,"name"));

    mxmlDelete(tree);

    // define os atributos
    //build_Attr(&(ship_->attr),power,speed);
    // define localização e dimensões
    //initialize_loc_dim(&(ship_->location),&(ship_->dimension),position_x,
      //      position_y,al_get_bitmap_width(ship_->image), al_get_bitmap_height(ship_->image));
    // cria uma imagem bitmap

    //ship_->image = create_bitmap_from_atlas("playerShip1_blue.png");

    //al_set_target_bitmap(ship_->image);
    //al_clear_to_color(al_map_rgb(255, 100, 155));
    //set_draw_current_window_game();

    // retorna ponteiro para a nave
    return ship;
}

// aloca nave do player
int new_player_ship(const char *subtype){
    // aloca nave com malloc
    player = (Player_ship*) malloc (sizeof(Player_ship));

    if(player == NULL){
        fprintf(stderr, "falha ao alocar objeto Player_ship\n");
        return false;
    }

    // constrói a base
    player->base = new_ship("player",subtype);

    if(player->base == NULL){
        fprintf(stderr, "falha ao alocar base de Player_ship\n");
        free(player);
        return false;
    }

    // retorna player
    return true;
}

int make_bullet(const char* type, const char* subtype, float position_x, float position_y){

    if(!bullet){

        FILE * file;
        mxml_node_t *tree;

        bullet = (Bullet*)malloc(sizeof(Bullet));
        if(!bullet){
            puts("falha ao alocar tiro");
            return false;
        }

        bullet->active = true;
        strcpy(bullet->type,type);
        strcpy(bullet->subtype,subtype);

        file = fopen(XML_BULLETS,"r");

        if(!file){
            puts("falha ao tentar abrir o arquivo");
            free(bullet);
            return false;
        }

        tree = mxmlLoadFile(NULL,file,MXML_TEXT_CALLBACK);
        if(!tree){
            puts("falha ao tentar criar arvore");
            free(bullet);
            fclose(file);
            return false;
        }

        fclose(file);

        mxml_node_t* node = mxmlFindElement(tree,tree,subtype,NULL,NULL,MXML_DESCEND);
        if(!node){
            puts("subtipo nao encontrado");
            mxmlDelete(tree);
            free(bullet);
            return false;
        }

        node = mxmlFindElement(node,node,"Attributes",NULL,NULL,MXML_DESCEND);
        bullet->power = atoi(mxmlElementGetAttr(node,"power"));
        bullet->speed = atof(mxmlElementGetAttr(node,"speed"));
        printf("power %d, speed %.0f \n",bullet->power,bullet->speed);

        node = mxmlWalkNext(node, tree, MXML_DESCEND);
        bullet->image = create_bitmap_from_atlas(mxmlElementGetAttr(node,"name"));

        mxmlDelete(tree);

        bullet->x = position_x - al_get_bitmap_width(bullet->image)/2;
        bullet->y = position_y - al_get_bitmap_height(bullet->image);

        return true;
    }

    else{
        puts("ja existe um tiro ativo");
        return false;
    }
}

/*************************************************
 * Destrutores - desalocação de memória
 *************************************************/

void dealloc_bullet(){
    if(bullet){
        al_destroy_bitmap(bullet->image);
        free(bullet);
        bullet = NULL;
        puts("bullet desalocado");
    }
}

// desaloca nave inimiga
void dealloc_ship(Ship* ship){

    if(ship){
        // desaloca imagens
        al_destroy_bitmap(ship->image);
        // desaloca com free
        free(ship);
        ship = NULL;
        puts("desalocado ship");
    }

}

// desaloca nave player
void dealloc_player_ship(){
    if(player){
        // desaloca base
        dealloc_ship(player->base);
        // desaloca com free
        free(player);
        player = NULL;
        puts("desalocado ship_player");
    }
}

void dealloc_ships_objects(){
    dealloc_player_ship();
    dealloc_bullet();
}

/*************************************************
 * Funções comuns das naves
 *************************************************/

void draw_ship(Ship* ship){
    al_draw_bitmap(ship->image, ship->location.position_x, ship->location.position_y, 0);
}

void draw_bullet(){
    if(bullet)
        al_draw_bitmap(bullet->image,bullet->x,bullet->y,0);
}

void draw_ships_objects(){
    draw_ship(player->base);
    draw_bullet();
}

/**************************************************
 * Funções somente para o player
 *************************************************/

//Pega a nave do jogador e retorna um ponteiro para a sua base
Ship* getBase(Player_ship* ship){
    return ship->base;
}

/*void player_fire(Player_ship* ship){
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
}*/

void update_bullet(){
    if(bullet){
        bullet->y -= 6;
        if(bullet->y < 5){
            dealloc_bullet();
            //al_destroy_bitmap(ship->base->fire.image);
            //ship->base->fire.image = NULL;
        }
    }
}

void update_player(){

    if(get_mouse_move_state()){
        player->base->location.position_x = get_mouse_x()-(al_get_bitmap_width(player->base->image)/2);
        player->base->location.position_y = get_mouse_y()-(al_get_bitmap_height(player->base->image)/2);
    }



    if(mouseIsClicked()){
        int bullet_position_x = player->base->location.position_x +
                    (al_get_bitmap_width(player->base->image)/2);
        int bullet_position_y = player->base->location.position_y;
        make_bullet(player->base->type,player->base->subtype,bullet_position_x,
                        bullet_position_y);
    }

}

void update_ships_objects(){
    update_player();
    update_bullet();
}
