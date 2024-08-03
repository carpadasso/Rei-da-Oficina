#include <allegro5/allegro5.h>
#include "stage.h"

/* Cria uma estrutura Cenário */
Stage* create_stage(Stage_Name stage_name)
{
   Stage* s = (Stage*) malloc(sizeof(Stage));
   if (!s) return NULL;

   if (stage_name == AMAZON){
      s->stage_sprite = al_load_bitmap("./assets/background/stage_amazon.png");
      s->stage_w = STAGE_W_AMAZON;
      s->stage_h = STAGE_H_AMAZON;
      s->stage_num_frame = STAGE_N_FRAME_AMAZON;
   }
   else if (stage_name == CONSTRUCTION){
      s->stage_sprite = al_load_bitmap("./assets/background/stage_construction.png");
      s->stage_w = STAGE_W_CONSTRUCTION;
      s->stage_h = STAGE_H_CONSTRUCTION;
      s->stage_num_frame = STAGE_N_FRAME_CONSTRUCTION;
   }
   else if (stage_name == FACTORY){
      s->stage_sprite = al_load_bitmap("./assets/background/stage_factory.png");
      s->stage_w = STAGE_W_FACTORY;
      s->stage_h = STAGE_H_FACTORY;
      s->stage_num_frame = STAGE_N_FRAME_FACTORY;
   }
   else if (stage_name == MARKET){
      s->stage_sprite = al_load_bitmap("./assets/background/stage_market.png");
      s->stage_w = STAGE_W_MARKET;
      s->stage_h = STAGE_H_MARKET;
      s->stage_num_frame = STAGE_N_FRAME_MARKET;
   }
   else {
      free(s); s = NULL;
   }

   s->stage_x = al_get_bitmap_width(s->stage_sprite) / 2;
   s->stage_frame = 0.0;

   return s;
}

/* Destrói uma estrutura Cenário */
void destroy_stage(Stage* s)
{
   al_destroy_bitmap(s->stage_sprite);
   free(s); s = NULL;
}

/* Exibe na tela um Cenário dado */
void draw_stage(Stage* s, float x, float y, float w, float h)
{
   if (s->stage_frame <= (s->stage_num_frame - 1)) s->stage_frame += 0.1;
   else s->stage_frame = 0.0;

   al_draw_scaled_bitmap(s->stage_sprite, s->stage_x, s->stage_h*(int)(s->stage_frame),
                           (4.0/3)*s->stage_h, s->stage_h, x, y, w, h, 0);
}