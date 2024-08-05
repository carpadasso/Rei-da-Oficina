#ifndef __SHOT_H__
#define __SHOT_H__

#include <allegro5/allegro5.h>
#include "enums.h"

/* -------------------
 * Defines necessários
 * ------------------- */
/* Tamanhos de cada DISPARO
 * baseado em cada PERSONAGEM */
#define SHOT_W_HURT_HADOUKEN 15
#define SHOT_H_HURT_HADOUKEN 15
#define SHOT_SPEED_HADOUKEN  8

/* ------------------
 * Tipos e Estruturas
 * ------------------ */
/* Tipo Enumerável SHOT_TYPE que define quais tipos
 * de disparo existem. */
typedef enum {
   HADOUKEN,
} Shot_Type;

/* Define a estrutura DISPARO 
 * > Tipo de disparo: shot_type
 * > Coordenadas e dimensões do sprite: x, y, w, h
 * > Coordenadas e dimensões da hurtbox: x_hurt, y_hurt, w_hurt, h_hurt
 * > Variáveis de Exibição: shot_sprite, dir_flag, frame */
typedef struct {
   /* Tipo de Disparo */
   Shot_Type shot_type;

   /* Coordenadas e Dimensões */
   int x, y, w, h;
   int x_hurt, y_hurt, w_hurt, h_hurt;

   /* Variáveis de Exibição*/
   ALLEGRO_BITMAP* shot_sprite;
   int dir_flag;
   float frame;
} Shot;

/* ---------------------
 * Funções da Biblioteca
 * --------------------- */
/* Aloca memória para uma estrutura DISPARO
 * baseado em um personagem dado */
Shot* create_shot(Character character, int x, int y, int w, int h, int flag);

/* Libera memória de uma estrutura DISPARO */
void  destroy_shot(Shot* shot);

/* Atualiza o movimento de uma estrutura DISPARO
 * baseado na direção em que o disparo está movento */
void  update_shot_movement(Shot* shot);

/* Mostra uma estrutura DISPARO na tela */
void  draw_shot(Shot* s);

#endif