#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <allegro5/allegro5.h>
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

/* Define constantes para cada direção de movimento do jogador */
typedef enum { 
   UP, 
   DOWN, 
   LEFT, 
   RIGHT 
} Direction;

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
   Character selected_char;
   Joystick *joystick;
   Sprite *sprites;
   unsigned short x;
   unsigned short y;
   unsigned short w;
   unsigned short h;
   unsigned short hit_points;
   int pos_flag;
} Player;

/* Funções da Interface "player" */

/* creater_player:
 * Aloca memória para um jogador recém criado.
 * Recebe e atribui os parâmetros do jogador fornecidos.
 * Retorna um ponteiro para a estrutura alocada. */
Player* create_player(Character char_selected, unsigned short x, unsigned short y,
                      unsigned short w, unsigned short h);

/* destroy_player:
 * Libera memória de todos os atributos do Jogador. */
void destroy_player(Player* player);

/* move_player:
 * */
int move_player(Player* player, Direction direction, unsigned short step,
                unsigned short velocity, unsigned short max_x, unsigned short max_y);

#endif