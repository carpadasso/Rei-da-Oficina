#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <allegro5/allegro5.h>

#include "enums.h"
#include "shot.h"
#include "joystick.h"
#include "sprite.h"

/* -------------------
 * Defines Necessários
 * ------------------- */

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

/* ------------------
 * Tipos e Estruturas
 * ------------------ */
/* Define a estrutura PLAYER
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

/* --------------------
 * Funções da Interface 
 * -------------------- */

/* ----------------
 * Criar e Destruir
 * ---------------- */
/* Aloca memória para uma estrutura PLAYER.
 * Recebe e atribui os parâmetros do jogador fornecidos.
 * Retorna um ponteiro para a estrutura alocada. */
Player* create_player(Character char_selected, int x0, int y0, int w0, int h0);

/* Libera memória de uma estrutura PLAYER e seus atributos. */
void destroy_player(Player* player);

/* -------------------------------------------------------------------------- */

/* ------------------
 * Funções de Colisão
 * ------------------ */
/* Verifica se uma área colidiu com outra área, se baseando no fato de que
 * uma colisão entre áreas acontece quando um vértice de uma delas adentra
 * ná área correspondente da outra. 
 * 
 * Uma área é delimitada a partir das coordenadas de um dos seus vértices (x, y)
 * e o tamanho de seus lados (w, h). Sendo assim, a partir dos pontos (x, y) e
 * (x + w, y + h), é possível estabeler uma área delimitada em formato retangular.
 * 
 * Dessa forma, verificando se os pontos (x, y), (x + w, y), (x, y + h) e (x + w, y + h)
 * estão contidos em outra área com coordenadas diferentes mas estrutura igual, podemos
 * verificar se uma área colidiu com a outra.
 * 
 * DETALHE: esta função NÃO verifica se duas áreas invocadas simultaneamente estão
 * colidindo, o que significa que ela considera colisão apenas de áreas que foram
 * criadas inicialmente disjuntas/separadas. */
bool is_area_colliding(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

/* -------------------------------------------------------------------------- */

/* --------------------
 * Funções de Movimento 
 * -------------------- */
/* Atualiza o movimento vigente do JOGADOR baseado nos inputs do JOYSTICK. */
void update_player_movement(Player* p1, Player* p2);

/* Atualiza as coordenadas (x, y) do JOGADOR baseado no movimento vigente. */
void update_player_coordinates(Player* p1, Player* p2);

/* Atualiza as coordenadas e dimensões da Hitbox e Hurtbox de um JOGADOR
 * baseado no movimento vigente. */
void update_player_boxes(Player* p);

/* Calcula se um ataque de um JOGADOR p1 acertou outro JOGADOR p2, e se sim
 * diminui a vida do segundo baseado no ataque realizado. */
void execute_attack(Player* p1, Player* p2);

/* -------------------------------------------------------------------------- */

/* -------------------
 * Funções de Exibição 
 * ------------------- */
/* Atualiza a Flag de posição dos JOGADORES comparando a coordenada x de ambos. */
void update_player_pos_flags(Player* p1, Player* p2);

/* Exibe na tela o sprite de um JOGADOR baseado no movimento vigente. */
void draw_sprite_player(Player *player);

/* -------------------------------------------------------------------------- */

/* ------------------
 * Funções de Disparo 
 * ------------------ */
/* Aloca memória para uma estrutura DISPARO e faz o apontador do JOGADOR p
 * apontar para a estrutura. */
void spawn_player_shot(Player* p);

/* Atualiza a coordenada x de um DISPARO feito por um JOGADOR baseado na
 * direção em que está se movendo. */
void move_shot(Player* p);

/* Verifica se um DISPARO realizado por um JOGADOR acertou outro JOGADOR p,
 * e em caso afirmativo, desconta pontos de vida do acertado. */
bool verify_damage_shot(Shot* shot, Player* p);

/* Libera memória alocada por uma estrutura DISPARO
 * realizado por um JOGADOR p. */
void despawn_player_shot(Player* p);

/* Mostra uma estrutura DISPARO na tela feita por um JOGADOR p. */
void show_shot(Player* p);

#endif