#ifndef __SHOT_H__
#define __SHOT_H__

#include <allegro5/allegro5.h>
#include "enums.h"

/* Tamanhos de cada DISPARO
 * baseado em cada PERSONAGEM */
#define SHOT_W_HURT_HADOUKEN 15
#define SHOT_H_HURT_HADOUKEN 15
#define SHOT_SPEED_HADOUKEN  8

typedef enum {
   HADOUKEN,
} Shot_Type;

typedef struct {
   Shot_Type shot_type;

   /* Coordenadas e Dimensões */
   int x, y, w, h;
   int x_hurt, y_hurt, w_hurt, h_hurt;

   /* Variáveis de Exibição*/
   ALLEGRO_BITMAP* shot_sprite;
   int dir_flag;
   float frame;
} Shot;

Shot* create_shot(Character character, int x, int y, int w, int h, int flag);
void  destroy_shot(Shot* shot);

void  update_shot_movement(Shot* shot);
void  draw_shot(Shot* s);


#endif