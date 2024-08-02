#ifndef __STAGE_H__
#define __STAGE_H__

#include <allegro5/allegro5.h>

/* --------------------------
 * Constantes de cada Cenário 
 * -------------------------- */
/* Floresta Amazônica */
#define STAGE_W_AMAZON       690
#define STAGE_H_AMAZON       212
#define STAGE_N_FRAME_AMAZON 16

/* Construção em Obras */
#define STAGE_W_CONSTRUCTION       680
#define STAGE_H_CONSTRUCTION       221
#define STAGE_N_FRAME_CONSTRUCTION 7

/* Usina Quente */
#define STAGE_W_FACTORY       688
#define STAGE_H_FACTORY       221
#define STAGE_N_FRAME_FACTORY 8

/* Mercado */
#define STAGE_W_MARKET       690
#define STAGE_H_MARKET       209
#define STAGE_N_FRAME_MARKET 12

typedef enum {
   AMAZON,
   CONSTRUCTION,
   FACTORY,
   MARKET
} Stage_Name;

typedef struct Stage {
   ALLEGRO_BITMAP* stage_sprite;
   int stage_x, stage_w, stage_h;
   int stage_num_frame;
   float stage_frame;
} Stage;

/* Cria uma estrutura Cenário */
Stage* create_stage(Stage_Name stage_name);

/* Destrói uma estrutura Cenário */
void destroy_stage(Stage* stage);

/* Exibe na tela um Cenário dado */
void draw_stage(Stage* stage, float x, float y, float w, float h);

#endif