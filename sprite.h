#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "player.h"

/* Estrutura "Sprite"
 * > idle: Animação quando não há comandos
 * > walking_pos: Animação de andar em direção ao jogador adversário
 * > walking_neg: Animação de andar contra o jogador adversário
 * > jump: Animação de pulo
 * > crouch: Animação de abaixar-se
 * > attack_sup: Animação de ataque com membros superiores
 * > attack_inf: Animação de ataque com membros inferiores */
typedef struct Sprite {
   ALLEGRO_BITMAP* idle;
   ALLEGRO_BITMAP* walking_pos;
   ALLEGRO_BITMAP* walking_neg;
   ALLEGRO_BITMAP* jump;
   ALLEGRO_BITMAP* crouch;
   ALLEGRO_BITMAP* attack_sup;
   ALLEGRO_BITMAP* attack_inf;
} Sprite;

/* Tipo "Character" 
 * Tipo enumerável que define constantes para cada
 * personagem do jogo. */
typedef enum { 
   RYU,
   KEN
} Character;

/* Tipo "Movement" 
 * Tipo enumerável que define o movimento que está
 * sendo realizado pelo jogador no instante. */
typedef enum {
   IDLE,
   WALKING_POSITIVE,
   WALKING_NEGATIVE,
   JUMPING,
   CROUCHING,
   ATTACKING_SUP,
   ATTACKING_INF,
} Movement;

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

/* draw_sprite_player:
 * Seleciona o frame desejado baseado no estado atual do
 * jogador. */
void draw_sprite_player(Player *player, unsigned short *frame);

#endif