#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "enums.h"

/* Struct Sprite */
struct Sprite_st {
   /* Sprite Ataque ESPECIAL*/
   ALLEGRO_BITMAP* attack_sp;
   
   /* Sprite de SOCO*/
   ALLEGRO_BITMAP* attack_hp;
   ALLEGRO_BITMAP* attack_mp;
   ALLEGRO_BITMAP* attack_lp;

   /* Sprite de CHUTE */
   ALLEGRO_BITMAP* attack_hk;
   ALLEGRO_BITMAP* attack_mk;
   ALLEGRO_BITMAP* attack_lk;

   /* PULO e AGACHAMENTO */
   ALLEGRO_BITMAP* jump;
   ALLEGRO_BITMAP* crouch;

   /* Andar */
   ALLEGRO_BITMAP* idle;
   ALLEGRO_BITMAP* walk_positive;
   ALLEGRO_BITMAP* walk_negative;

   float frame;
};
typedef struct Sprite_st Sprite;

/* Funções da Biblioteca "sprite" */

/* create_sprites:
 * Aloca estruturas bitmap para guardar cada sprite,
 * dado um personagem fornecido como parâmetro. 
 * Retorna o ponteiro para estrutura. */
Sprite* create_sprites(Character fighter_selected);

/* destroy_sprites:
 * Destrói os bitmaps reservados para os sprites do
 * jogador. */
void destroy_sprites(Sprite* sprites);

#endif