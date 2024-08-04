#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "shot.h"

/* ----------------
 * Criar e Destruir
 * ---------------- */
Shot* create_shot(Character character, int x0, int y0, int w0, int h0, int flag)
{
   Shot* s = (Shot*) malloc(sizeof(Shot));
   if (s == NULL) return NULL;

   s->x = x0;
   s->y = y0;
   s->w = w0;
   s->h = h0;

   if (character == RYU || character == KEN){
      s->shot_type   = HADOUKEN;
      s->shot_sprite = al_load_bitmap("./assets/characters/shots/hadouken.png");
      s->x_hurt = s->x + 120;
      s->y_hurt = s->y + 80;
      s->w_hurt = SHOT_W_HURT_HADOUKEN;
      s->h_hurt = SHOT_H_HURT_HADOUKEN;
   }

   s->dir_flag = flag;
   s->frame    = 0.0;

   return s;
}

void destroy_shot(Shot* s)
{ 
   /* Teste de Sanidade... */
   if (s == NULL) return;

   al_destroy_bitmap(s->shot_sprite);
   free(s);
}

/* --------------------
 * Movimento do DISPARO
 * -------------------- */
void update_shot_movement(Shot* s)
{
   /* Teste de Sanidade... */
   if (s == NULL) return;

   if (s->dir_flag == 0){
      if (s->shot_type == HADOUKEN){
         s->x += SHOT_SPEED_HADOUKEN;
         s->x_hurt = s->x + 120;
      }
   }
   else if (s->dir_flag == 1){
      if (s->shot_type == HADOUKEN){
         s->x -= SHOT_SPEED_HADOUKEN;
         s->x_hurt = s->x + 120;
      }
   }
}

/* -------------------
 * Exibição do DISPARO
 * ------------------- */
void draw_shot(Shot* s)
{
   /* Teste de Sanidade... */
   if (s == NULL) return;

   if (s->shot_type == HADOUKEN)
      al_draw_scaled_bitmap(s->shot_sprite, 0, 0, 55, 35, s->x + 50, s->y + 50, 2.5*s->w, 2.5*s->h, s->dir_flag);
}