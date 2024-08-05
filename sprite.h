#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "enums.h"

/* ------------------
 * Tipos e Estruturas
 * ------------------ */
/* Define a estrutura SPRITE
 * > Bitmaps de ataque: attack_sp,
 * attack_mp, attack_lp, attack_lp,
 * attack_hk, attack_mk, attack_lk,
 * > Bitmaps de movimento: jump, crouch,
 * idle, walk_positive, walk_negative
 * > Bitmaps de Fim de Jogo: victory, defeat */
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

   /* Vitória e Derrota */
   ALLEGRO_BITMAP* victory;
   ALLEGRO_BITMAP* defeat;

   float frame;
   int loop_count;
};
typedef struct Sprite_st Sprite;

/* ---------------------
 * Funções da Biblioteca
 * --------------------- */
/* Aloca estruturas bitmap para guardar cada sprite,
 * dado um personagem fornecido como parâmetro. 
 * Retorna o ponteiro para estrutura. */
Sprite* create_sprites(Character fighter_selected);

/* Libera memória alocada por uma estrutura SPRITE. */
void destroy_sprites(Sprite* sprites);

#endif