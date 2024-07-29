#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <allegro5/allegro5.h>

#include "enums.h"
#include "joystick.h"
#include "sprite.h"

/* Define constantes de inicialização do jogo */
#define DISPLAY_WIDTH  800
#define DISPLAY_HEIGHT 600
#define REFRESH_RATE   60.0

/* Define constantes de Jogador */
#define DEFAULT_HIT_POINTS 100
#define STEP_FRONT         10
#define STEP_BACK          5
#define JUMP_VEL           5
#define Y_MAX              DISPLAY_HEIGHT - 300
#define Y_MIN              Y_MAX - 160

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
   int x, y;
   int w, h;
   int hit_points;
   int pos_flag;
   bool isJumping, isFalling;
   Character selected_char;
   Movement move;
   Joystick *joystick;
   Sprite *sprites;
} Player;

/* -----------------------------
 * Funções da Interface "player" 
 * ----------------------------- */

/* create_player:
 * Aloca memória para um jogador recém criado.
 * Recebe e atribui os parâmetros do jogador fornecidos.
 * Retorna um ponteiro para a estrutura alocada. */
Player* create_player(Character char_selected, int x, int y, int w, int h);

/* destroy_player:
 * Libera memória de todos os atributos do Jogador. */
void destroy_player(Player* player);

/* update_player_movement:
 * Atualiza o movimento vigente do jogador baseado nos inputs
 * do joystick. */
void update_player_movement(Player* p1, Player* p2);

/* update_player_coordinates: 
 * Atualiza as coordenadas do jogador baseado no movimento vigente. */
void update_player_coordinates(Player* p1, Player* p2);

void update_player_flags(Player* p1, Player* p2);

/* draw_sprite_player:
 * Seleciona o frame desejado baseado no estado atual do
 * jogador.
 * Retorna true caso seja necessário resetar o frame de animação, e 
 * false caso contrário. */
bool draw_sprite_player(Player *player, float* frame);

bool is_area_colliding(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

#endif