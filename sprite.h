#ifndef __SPRITE__
#define __SPRITE__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "player.h"

/* Define a estrutura "Sprite"
 * idle: Animação quando não há comandos
 * walking_pos: Animação de andar em direção ao jogador adversário
 * walking_neg: Animação de andar contra o jogador adversário
 * attack_sup: Animação de ataque com membros superiores
 * attack_inf: Animação de ataque com membros inferiores
 * jump: Animação de pulo */
typedef struct{
   ALLEGRO_BITMAP* idle;
   ALLEGRO_BITMAP* walking_pos;
   ALLEGRO_BITMAP* walking_neg;
   ALLEGRO_BITMAP* attack_sup;
   ALLEGRO_BITMAP* attack_inf;
   ALLEGRO_BITMAP* jump;
} Sprite;

/* Funções da Biblioteca "sprite" */

/* create_sprites:
 * */
Sprite* create_sprites(Character fighter_selected);
/* destroy_sprites:
 * */
void destroy_sprites(Sprite* sprites);

#endif