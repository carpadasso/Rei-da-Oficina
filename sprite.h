#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "enums.h"

/* Estrutura "Sprite"
 * > idle: Animação quando não há comandos
 * > walking_pos: Animação de andar em direção ao jogador adversário
 * > walking_neg: Animação de andar contra o jogador adversário
 * > jump: Animação de pulo
 * > crouch: Animação de abaixar-se
 * > attack_sup: Animação de ataque com membros superiores
 * > attack_inf: Animação de ataque com membros inferiores */
struct Sprite_st {
   ALLEGRO_BITMAP* idle;
   ALLEGRO_BITMAP* walking_pos;
   ALLEGRO_BITMAP* walking_neg;
   ALLEGRO_BITMAP* jump;
   ALLEGRO_BITMAP* crouch;
   ALLEGRO_BITMAP* attack_sup;
   ALLEGRO_BITMAP* attack_inf;
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