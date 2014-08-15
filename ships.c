/*
 * ships.c
 *
 *  Módulo que trata do player e das naves inimigas.
 */

#include "ships.h"

/****************************************************
 * **************************************************
 * Estruturas
 * **************************************************
 ****************************************************/

/* --------------------------------------------
 * Parte da nave que descreve seus atributos
 * --------------------------------------------
 * Membros:
 * int power : poder (especial para nave, poder de tiro
 *              para tiro)
 * float speed : velocidade
 ----------------------------------------------*/
typedef struct attributes{
    int power;
    float speed;
}Attributes;

/* ---------------------------------------------------
 * Parte da nave que determina a sua localização
 * ---------------------------------------------------
 * Membros:
 * float position_x : coordenada x
 * float position_y : coordenada y
 -----------------------------------------------------*/
typedef struct location{
    float position_x;
    float position_y;
}Location;

/* --------------------------------------------------
 * Parte da nave que determina a sua dimensão
 * --------------------------------------------------
 * Membros:
 * float width : largura
 * float height : altura
 ----------------------------------------------------*/
typedef struct dimension{
    float width;
    float height;
}Dimension;

/* ----------------------------------------------------
 * Descreve um único tiro
 * ----------------------------------------------------
 * Membros:
 * char type[20] : tipo do tiro (player ou enemy)
 * char subtype[20] : subtipo do tiro (variável)
 *
 * Attributes attr : atributos do tiro
 * Location location : posição x e y do tiro
 * Dimension dimension : dimensões do tiro
 *
 * ALLEGRO_BITMAP *image : imagem do tiro
 *
 * Bullet* nextBullet : ponteiro para o próximo tiro
 * Bullet* previousBullet : ponteiro para o tiro anterior
 ------------------------------------------------------*/
typedef struct bullet{
    // tipo e subtipo
    char type[20];
    char subtype[20];

    // caractesrísticas
    Attributes attr;
    Location location;
    Dimension dimension;

    // imagem
    ALLEGRO_BITMAP *image;

    // ponteiros para outros tiros
    Bullet* nextBullet;
    Bullet* previousBullet;

}Bullet;

/* ----------------------------------------------------
 * Lista de tiros
 * ----------------------------------------------------
 * Membros:
 * Bullet* firstBullet : ponteiro para o primeiro tiro
 ------------------------------------------------------*/
typedef struct bullets{
    Bullet* firstBullet;
}Bullets;

/* ----------------------------------------------------
 * Descreve a base comum a todas as naves
 * ----------------------------------------------------
 * Membros:
 * char type[20] : tipo da nave (player ou enemy)
 * char subtype[20] : subtipo da nave (variável)
 * char bullet_subtype[20] : subtipo do tiro atual (variável)
 *
 * Attributes attr : atributos da nave (power, speed)
 * Location location : coordenadas x e y
 * Dimension dimension : dimensões da nave
 *
 * ALLEGRO_BITMAP* image : imagem da nave
 ------------------------------------------------------*/
typedef struct ship{

    // definições de tipos
    char type[20];
    char subtype[20];
    char bullet_subtype[20];

    // características
    Attributes attr;
    Location location;
    Dimension dimension;

    // imagens
    ALLEGRO_BITMAP *image;
}Ship;

/* ----------------------------------------------------
 * estrutura usada pela nave do player
 * ----------------------------------------------------
 * Base:
 * Ship* base : estrutura básica da nave
 ------------------------------------------------------*/
typedef struct player_ship{
    Ship* base;
}Player_ship;


/*******************************************
 * *****************************************
 * Construtores - alocação de memória
 * *****************************************
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
Ship* new_ship(const char *type, const char *subtype,Window_game** window,Graphics_game** graphics){


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
    ship->image = create_bitmap_from_atlas(mxmlElementGetAttr(node,"name"),window,graphics);

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
int new_player_ship(Player_ship** player,const char *subtype,Window_game** window,Graphics_game** graphics){
    // aloca nave com malloc
    (*player) = malloc (sizeof(Player_ship));

    if((*player) == NULL){
        fprintf(stderr, "falha ao alocar objeto Player_ship\n");
        return false;
    }

    // constrói a base
    (*player)->base = new_ship("player",subtype,window,graphics);

    if((*player)->base == NULL){
        fprintf(stderr, "falha ao alocar base de Player_ship\n");
        free(*player);
        return false;
    }

    // retorna player
    return true;
}

int make_bullet(Bullets** bullets,const char* type, const char* subtype, float position_x, float position_y,
        Window_game** window, Graphics_game** graphics){



    FILE * file;
    mxml_node_t *tree;

    Bullet* bullet = malloc(sizeof(Bullet));
    if(!bullet){
        puts("falha ao alocar tiro");
        return false;
    }

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
    bullet->attr.power = atoi(mxmlElementGetAttr(node,"power"));
    bullet->attr.speed = atof(mxmlElementGetAttr(node,"speed"));
    printf("power %d, speed %.0f \n",bullet->attr.power,bullet->attr.speed);

    node = mxmlWalkNext(node, tree, MXML_DESCEND);
    bullet->image = create_bitmap_from_atlas(mxmlElementGetAttr(node,"name"),window,graphics);

    mxmlDelete(tree);

    bullet->location.position_x = position_x - al_get_bitmap_width(bullet->image)/2;
    bullet->location.position_y = position_y - al_get_bitmap_height(bullet->image);


    bullet->nextBullet = (*bullets)->firstBullet;
    bullet->previousBullet = NULL;

    if((*bullets)->firstBullet != NULL)
        (*bullets)->firstBullet->previousBullet = bullet;

    (*bullets)->firstBullet = bullet;

    return true;

}

int makeListofBullets(Bullets** bullets){
    (*bullets) = malloc(sizeof(Bullets));
    if(!(*bullets)){
        puts("falha ao tentar alocar bullets");
        return false;
    }

    (*bullets)->firstBullet = NULL;


    return true;
}

/*************************************************
 * Destrutores - desalocação de memória
 *************************************************/

void dealloc_bullet(Bullet** bullet){
    al_destroy_bitmap((*bullet)->image);
    free(*bullet);
    (*bullet) = NULL;
    puts("bullet desalocado");
}

void dealloc_bullets(Bullets** bullets){

    if(*bullets){

        Bullet* bullet = (*bullets)->firstBullet;
        Bullet* nextBullet = NULL;

        while(bullet){

            nextBullet = bullet->nextBullet;

            /*al_destroy_bitmap(bullet->image);
            free(bullet);
            bullet = NULL;
            puts("bullet desalocado");*/
            dealloc_bullet(&bullet);

            bullet = nextBullet;

        }

        free(*bullets);
        puts("lista de bullets desalocada");

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
void dealloc_player_ship(Player_ship** player){
    if(*player){
        // desaloca base
        dealloc_ship((*player)->base);
        // desaloca com free
        free(*player);
        (*player) = NULL;
        puts("desalocado ship_player");
    }
}

void dealloc_ships_objects(Player_ship** player,Bullets** bullets){
    dealloc_player_ship(player);
    dealloc_bullets(bullets);
}

/*************************************************
 * Funções comuns das naves
 *************************************************/

void draw_ship(Ship* ship){
    al_draw_bitmap(ship->image, ship->location.position_x, ship->location.position_y, 0);
}

void draw_bullet(Bullets** bullets){

    Bullet* bullet = (*bullets)->firstBullet;

    while(bullet){
        al_draw_bitmap(bullet->image,bullet->location.position_x,bullet->location.position_y,0);
        bullet = bullet->nextBullet;
    }

}

void draw_ships_objects(Player_ship** player,Bullets** bullets){
    draw_ship((*player)->base);
    draw_bullet(bullets);
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

void update_bullets(Bullets** bullets){

    Bullet* bullet = (*bullets)->firstBullet;
    Bullet* nextBullet = NULL;

    while(bullet){

        nextBullet = bullet->nextBullet;

        bullet->location.position_y -= 6;

        if(bullet->location.position_y < 5){

            if(bullet->previousBullet == NULL){

                if(bullet->nextBullet == NULL){
                    (*bullets)->firstBullet = NULL;
                    dealloc_bullet(&bullet);
                    //free(bullet);
                }else{
                    (*bullets)->firstBullet = bullet->nextBullet;
                    (*bullets)->firstBullet->previousBullet = NULL;
                    //free(bullet);
                    dealloc_bullet(&bullet);
                    bullet = NULL;
                }
            }

            else if(bullet->nextBullet == NULL){
                bullet->previousBullet->nextBullet = NULL;
                //free(bullet);
                dealloc_bullet(&bullet);
                bullet = NULL;
            }

            else{

                bullet->previousBullet->nextBullet = bullet->nextBullet;
                bullet->nextBullet->previousBullet = bullet->previousBullet;
                //free(bullet);
                dealloc_bullet(&bullet);
                bullet=NULL;
            }

        }

        bullet = nextBullet;
    }
}

void update_player(Player_ship** player, Bullets** bullets, Window_game** window,
        Graphics_game** graphics, Controls_game** controls){

    if(get_mouse_move_state(controls)){
        (*player)->base->location.position_x = get_mouse_x(controls)-(al_get_bitmap_width((*player)->base->image)/2);
        (*player)->base->location.position_y = get_mouse_y(controls)-(al_get_bitmap_height((*player)->base->image)/2);
    }



    if(mouseIsClicked(controls)){
        int bullet_position_x = (*player)->base->location.position_x +
                    (al_get_bitmap_width((*player)->base->image)/2);
        int bullet_position_y = (*player)->base->location.position_y;
        make_bullet(bullets,(*player)->base->type,(*player)->base->bullet_subtype,bullet_position_x,
                        bullet_position_y,window,graphics);
    }

}

void update_ships_objects(Player_ship** player,Bullets** bullets,Window_game** window,Graphics_game** graphics,Controls_game** controls){
    update_player(player,bullets,window,graphics,controls);
    update_bullets(bullets);
}
