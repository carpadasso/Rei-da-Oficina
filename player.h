#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <allegro5/allegro5.h>

#include "enums.h"
#include "shot.h"
#include "joystick.h"
#include "sprite.h"

/* Define constantes de inicialização do jogo */
#define DISPLAY_WIDTH  800
#define DISPLAY_HEIGHT 600
#define REFRESH_RATE   60.0

/* Define constantes de Jogador */
#define DEFAULT_HIT_POINTS 100
#define DEFAULT_POW_POINTS 3
#define STEP_FRONT         10
#define STEP_BACK          5
#define GRAVITY            10
#define X_MIN              0
#define X_MAX              DISPLAY_WIDTH - 175
#define Y_MAX              DISPLAY_HEIGHT - 300
#define Y_MIN              Y_MAX - 350

/* Define a estrutura "Player"
 * selected_char: Personagem escolhido pelo jogador
 * joystick: Registro de movimento do jogador
 * sprites: Coleção dos sprites de movimento
 * x: Coordenada x atual do jogador
 * y: Coordenada y atual do jogador
 * w: Comprimento atual do sprite do jogador
 * h: Altura atual do sprite do jogador
 * hit_points: Pontos de vida atual do jogador */
typedef struct Player {

   /* Coordenadas e Dimensões */
   int x, y, w, h;
   int x_hit, y_hit, w_hit, h_hit;
   int x_hurt, y_hurt, w_hurt, h_hurt;
   int hit_points, power_points;

   /* Flags */
   int pos_flag;
   int enable_jump, enable_atk_p, enable_atk_k, enable_atk_sp;
   bool is_jumping, is_falling;

   /* Variáveis Enumeráveis */
   Movement move;
   Character selected_char;

   /* Estruturas */
   Sprite *sprites;
   Joystick *joystick;
   Shot* shot;

} Player;

/* -----------------------------
 * Funções da Interface "player" 
 * ----------------------------- */

/* create_player:
 * Aloca memória para um jogador recém criado.
 * Recebe e atribui os parâmetros do jogador fornecidos.
 * Retorna um ponteiro para a estrutura alocada. */
Player* create_player(Character char_selected, int x0, int y0, int w0, int h0);

/* destroy_player:
 * Libera memória de todos os atributos do Jogador. */
void destroy_player(Player* player);

bool is_area_colliding(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

/* -------------------------------------------------------------------------- */

/* update_player_movement:
 * Atualiza o movimento vigente do jogador baseado nos inputs
 * do joystick. */
void update_player_movement(Player* p1, Player* p2);

/* update_player_coordinates: 
 * Atualiza as coordenadas do jogador baseado no movimento vigente. */
void update_player_coordinates(Player* p1, Player* p2);

void update_player_boxes(Player* p);

void execute_attack(Player* p1, Player* p2);

/* -------------------------------------------------------------------------- */

void update_player_pos_flags(Player* p1, Player* p2);

/* draw_sprite_player:
 * Seleciona o frame desejado baseado no estado atual do
 * jogador.
 * Retorna true caso seja necessário resetar o frame de animação, e 
 * false caso contrário. */
void draw_sprite_player(Player *player);

/* -------------------------------------------------------------------------- */

void spawn_player_shot(Player* p);

void show_shot(Player* p);

void move_shot(Player* p);

bool verify_damage_shot(Shot* shot, Player* p);

void despawn_player_shot(Player* p);

#endif