#include <string.h>
#include "player.h"
#include "sprite.h"

Sprite* create_sprites(Character fighter_selected)
{
   Sprite* sprites = (Sprite*) malloc(sizeof(Sprite));
   if (sprites == NULL) return NULL;

   if (fighter_selected == RYU){
      sprites->idle = al_load_bitmap("./assets/characters/ryu/idle.png");
      sprites->walking_pos = al_load_bitmap("./assets/characters/ryu/walk_pos.png");
      sprites->walking_neg = al_load_bitmap("./assets/characters/ryu/walk_neg.png");
      sprites->jump = al_load_bitmap("./assets/characters/ryu/jump.png");
      sprites->crouch = al_load_bitmap("./assets/characters/ryu/crouch.png");
      sprites->attack_sup = al_load_bitmap("./assets/characters/ryu/attack_sup.png");
      sprites->attack_inf = al_load_bitmap("./assets/characters/ryu/attack_inf.png");
   }
   else if (fighter_selected == KEN){
      sprites->idle = al_load_bitmap("./assets/characters/ken/idle.png");
      sprites->walking_pos = al_load_bitmap("./assets/characters/ken/walk_pos.png");
      sprites->walking_neg = al_load_bitmap("./assets/characters/ken/walk_neg.png");
      sprites->jump = al_load_bitmap("./assets/characters/ken/jump.png");
      sprites->crouch = al_load_bitmap("./assets/characters/ken/crouch.png");
      sprites->attack_sup = al_load_bitmap("./assets/characters/ken/attack_sup.png");
      sprites->attack_inf = al_load_bitmap("./assets/characters/ken/attack_inf.png");
   }
   else {
      free(sprites); return NULL;
   }
   
   return sprites;
}

void destroy_sprites(Sprite *sprites)
{
   al_destroy_bitmap(sprites->idle);
   al_destroy_bitmap(sprites->walking_pos);
   al_destroy_bitmap(sprites->walking_neg);
   al_destroy_bitmap(sprites->jump);
   al_destroy_bitmap(sprites->crouch);
   al_destroy_bitmap(sprites->attack_sup);
   al_destroy_bitmap(sprites->attack_inf);
}

void draw_sprite_player(Player* p, unsigned short *frame)
{
   int flag = p->pos_flag;

   if (p->joystick->up)
      al_draw_scaled_bitmap(p->sprites->jump, 0, 20, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
   else if (p->joystick->left)
      al_draw_scaled_bitmap(p->sprites->walking_neg, 0, 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
   else if (p->joystick->right)
      al_draw_scaled_bitmap(p->sprites->walking_pos, 0, 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
   else if (p->joystick->down)
      al_draw_scaled_bitmap(p->sprites->crouch, 0, 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
   else if (p->joystick->button_1){
      p->w = 110;
      p->h = 95;
      al_draw_scaled_bitmap(p->sprites->attack_sup, 70, 0, 110, 95, p->x, p->y, 110*2.5, 95*2.5, flag);
   }
   else if (p->joystick->button_2)
      al_draw_scaled_bitmap(p->sprites->attack_inf, 140, 0, 90, 95, p->x, p->y, 90*2.5, 95*2.5, flag);
   else
      al_draw_scaled_bitmap(p->sprites->idle, 0, 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
}