#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "enums.h"
#include "sprite.h"

/* ----------------
 * Criar e Destruir
 * ---------------- */
Sprite* create_sprites(Character fighter_selected)
{
   Sprite* sprites = (Sprite*) malloc(sizeof(Sprite));
   if (sprites == NULL) return NULL;

   if (fighter_selected == RYU){
      sprites->attack_sp     = al_load_bitmap("./assets/characters/ryu/attack_sp.png");

      sprites->attack_hp     = al_load_bitmap("./assets/characters/ryu/attack_hp.png");
      sprites->attack_mp     = al_load_bitmap("./assets/characters/ryu/attack_mp.png");
      sprites->attack_lp     = al_load_bitmap("./assets/characters/ryu/attack_lp.png");

      sprites->attack_hk     = al_load_bitmap("./assets/characters/ryu/attack_hk.png");
      sprites->attack_mk     = al_load_bitmap("./assets/characters/ryu/attack_mk.png");
      sprites->attack_lk     = al_load_bitmap("./assets/characters/ryu/attack_lk.png");

      sprites->jump          = al_load_bitmap("./assets/characters/ryu/jump.png");
      sprites->crouch        = al_load_bitmap("./assets/characters/ryu/crouch.png");
      
      sprites->idle          = al_load_bitmap("./assets/characters/ryu/idle.png");
      sprites->walk_positive = al_load_bitmap("./assets/characters/ryu/walk_positive.png");
      sprites->walk_negative = al_load_bitmap("./assets/characters/ryu/walk_negative.png");

      sprites->victory       = al_load_bitmap("./assets/characters/ryu/victory.png");
      sprites->defeat        = al_load_bitmap("./assets/characters/ryu/defeat.png");
   }

   else if (fighter_selected == KEN){
      sprites->attack_sp     = al_load_bitmap("./assets/characters/ken/attack_sp.png");

      sprites->attack_hp     = al_load_bitmap("./assets/characters/ken/attack_hp.png");
      sprites->attack_mp     = al_load_bitmap("./assets/characters/ken/attack_mp.png");
      sprites->attack_lp     = al_load_bitmap("./assets/characters/ken/attack_lp.png");

      sprites->attack_hk     = al_load_bitmap("./assets/characters/ken/attack_hk.png");
      sprites->attack_mk     = al_load_bitmap("./assets/characters/ken/attack_mk.png");
      sprites->attack_lk     = al_load_bitmap("./assets/characters/ken/attack_lk.png");

      sprites->jump          = al_load_bitmap("./assets/characters/ken/jump.png");
      sprites->crouch        = al_load_bitmap("./assets/characters/ken/crouch.png");

      sprites->idle          = al_load_bitmap("./assets/characters/ken/idle.png");
      sprites->walk_positive = al_load_bitmap("./assets/characters/ken/walk_positive.png");
      sprites->walk_negative = al_load_bitmap("./assets/characters/ken/walk_negative.png");

      sprites->victory       = al_load_bitmap("./assets/characters/ken/victory.png");
      sprites->defeat        = al_load_bitmap("./assets/characters/ken/defeat.png");
   }

   else {
      free(sprites); sprites = NULL;
   }
   
   sprites->frame = 0.0;
   
   return sprites;
}

void destroy_sprites(Sprite *sprites)
{
   /* Teste de Sanidade... */
   if (sprites == NULL) return;

   if (sprites->attack_hp) al_destroy_bitmap(sprites->attack_hp);
   if (sprites->attack_mp) al_destroy_bitmap(sprites->attack_mp);
   if (sprites->attack_lp) al_destroy_bitmap(sprites->attack_lp);

   if (sprites->attack_hk) al_destroy_bitmap(sprites->attack_hk);
   if (sprites->attack_mk) al_destroy_bitmap(sprites->attack_mk);
   if (sprites->attack_lk) al_destroy_bitmap(sprites->attack_lk);

   if (sprites->jump)   al_destroy_bitmap(sprites->jump);
   if (sprites->crouch) al_destroy_bitmap(sprites->crouch);

   if (sprites->idle)          al_destroy_bitmap(sprites->idle);
   if (sprites->walk_positive) al_destroy_bitmap(sprites->walk_positive);
   if (sprites->walk_negative) al_destroy_bitmap(sprites->walk_negative);

   if (sprites->victory) al_destroy_bitmap(sprites->victory);
   if (sprites->defeat)  al_destroy_bitmap(sprites->defeat);

   free(sprites); sprites = NULL;
}