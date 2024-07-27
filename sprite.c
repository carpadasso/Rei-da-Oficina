#include <string.h>

#include "enums.h"
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