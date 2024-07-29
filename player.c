#include <allegro5/allegro5.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "joystick.h"
#include "sprite.h"
#include "player.h"

/* ------------------
 *   Create and Destroy
 * ------------------ */

Player* create_player(Character character, int x, int y, int w, int h)
{
   /* Alocação de Memória */
   Player* new_player = (Player*) malloc(sizeof(Player));
   if (!new_player) return NULL;

   /* Atribuição de valores */
   new_player->selected_char = character;
   new_player->x = x;
   new_player->y = y;
   new_player->w = w;
   new_player->h = h;
   new_player->hit_points = DEFAULT_HIT_POINTS;
   new_player->pos_flag = 0;
   new_player->isJumping = false;

   new_player->joystick = create_joystick();
   new_player->sprites = create_sprites(character);

   return new_player;
}

void destroy_player(Player *player)
{
   /* Destrói as estruturas Joystick e Sprites */
   destroy_joystick(player->joystick);
   destroy_sprites(player->sprites);

   free(player);
   player = NULL;
}

/* -------------------
 *   Player Collision
 * ------------------- */
bool is_area_colliding(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
   /* Colisão de Área 01 -> Área 02 */
   /* Canto Superior Esquerdo de Área 01 */
   if ((x2 <= x1) && (x1 <= (x2 + w2)) &&
       (y2 <= y1) && (y1 <= (y2 + h2)))
      return true;
   /* Canto Superior Direito de Área 01 */
   if ((x2 <= (x1 + w1)) && ((x1 + w1) <= (x2 + w2)) &&
       (y2 <= y1) && (y1 <= (y2 + h1)))
      return true;
   /* Canto Inferior Esquerdo de Área 01 */
   if ((x2 <= x1) && (x1 <= (x2 + w2)) &&
       (y2 <= (y1 + h1)) && ((y1 + h1) <= (y2 + h2)))
      return true;
   /* Canto Inferior Direito de Player 01 */
   if ((x2 <= (x1 + w1)) && ((x1 + w1) <= (x2 + w2)) &&
       (y2 <= (y1 + h1)) && ((y1 + h1) <= (y2 + h2)))
      return true;

   /* Colisão de Área 02 -> Área 01 */
   /* Canto Superior Esquerdo de Player 02 */
   if ((x1 <= x2) && (x2 <= (x1 + w1)) &&
       (y1 <= y2) && (y2 <= (y1 + h1)))
      return true;
   /* Canto Superior Direito de Player 02 */
   if ((x1 <= (x2 + w2)) && ((x2 + w2) <= (x1 + w1)) &&
       (y1 <= y2) && (y2 <= (y1 + h1)))
      return true;
   /* Canto Inferior Esquerdo de Player 02 */
   if ((x1 <= x2) && (x2 <= (x1 + w1)) &&
       (y1 <= (y2 + h2)) && ((y2 + h2) <= (y1 + h1)))
      return true;
   /* Canto Inferior Direito de Player 02 */
   if ((x1 <= (x2 + w2)) && ((x2 + w2) <= (x1 + w1)) &&
       (y1 <= (y2 + h2)) && ((y2 + h2) <= (y1 + h1)))
      return true;

   return false;
}

/* ------------------
 *   Player Movement
 * ------------------ */

void update_player_movement(Player* p1, Player* p2)
{

   /* Possibilidade 01: Pular */
   if (p1->joystick->up){
      p1->move = JUMPING;
   }

   /* Possibilidade 02: Andar Positivo */
   else if (p1->joystick->right)
      p1->move = WALKING_POSITIVE;

   /* Possibilidade 03: Andar Negativo */
   else if (p1->joystick->left)
      p1->move = WALKING_NEGATIVE;

   /* Possibilidade 04: Agachar */
   else if (p1->joystick->down)
      p1->move = CROUCHING;

   /* Possibilidade 05: Ataque Superior */
   else if (p1->joystick->button_1)
      p1->move = ATTACKING_SUP;

   /* Possibilidade 06: Ataque Inferior */
   else if (p1->joystick->button_2)
      p1->move = ATTACKING_INF;

   /* Possibilidade 07: Parado */
   else {
      p1->move = IDLE;
   }
}

void update_player_coordinates(Player* p1, Player* p2)
{
   /* Possibilidade 01: Jogador PARADO 
    * Se o jogador está parado, mantém as coordenadas */
   if (p1->move == IDLE)
      return;

   /* Possibilidade 02: Jogador ANDANDO POSITIVO
    * Incrementa-se a coordenada x do jogador baseado na direção
    * que seu personagem está olhando. */
   else if (p1->move == WALKING_POSITIVE){
      if (p1->pos_flag == 0) p1->x += STEP_FRONT;
      else p1->x += STEP_BACK;
      if (is_area_colliding(p1->x, p1->y, p1->w, p1->h, p2->x, p2->y, p2->w, p2->h)){
         if (p1->pos_flag == 0) p1->x -= STEP_FRONT;
         else p1->x -= STEP_BACK;
      }
   }

   /* Possibilidade 03: Jogador ANDANDO NEGATIVO
    * Decrementa-se a coordenada x do jogador baseado na direção
    * que seu personagem está olhando. */
   else if (p1->move == WALKING_NEGATIVE){
      if (p1->pos_flag == 0) p1->x -= STEP_BACK;
      else p1->x -= STEP_FRONT;
      if (is_area_colliding(p1->x, p1->y, p1->w, p1->h, p2->x, p2->y, p2->w, p2->h)){
         if (p1->pos_flag == 0) p1->x += STEP_BACK;
         else p1->x += STEP_FRONT;
      }
   }

   /* Possibilidade 04: Jogador PULANDO 
    * Aqui as coisas complicam um pouco mais:
    * 1) Primeiro, verificamos se o jogador quer movimentar durante o pulo
    * 2) Segundo, verificamos se o jogador atingiu a altura máxima de um pulo
    * (não queremos o jogador entrando em órbita), se ele já atingiu a altura
    * máxima, fazemos y = Y_MAX
    * 3) Ajustamos a coordenada y do jogador */
   else if (p1->move == JUMPING){
      /* Parte 01: Movento Esquerda-Direita no meio do ar */
      if (p1->joystick->left){
         if (p1->pos_flag == 0) p1->x -= STEP_FRONT;
         else p1->x -= STEP_BACK;
      }
      if (p1->joystick->right){
         if (p1->pos_flag == 0) p1->x += STEP_BACK;
         else p1->x += STEP_FRONT;
      }

      /* Parte 02: Fazendo o jogador pular */
      if (p1->y >= Y_MAX){
         p1->y = Y_MAX;
         p1->move = IDLE;
         p1->isFalling = false;
      }
      else {
         if (p1->y <= Y_MIN) p1->isFalling = true;

         if (p1->isFalling) p1->y += JUMP_VEL;
         else p1->y -= JUMP_VEL;
      }
   }

   /* Limites da Tela: Corrige o problema do jogador
    * ultrapassar os limites da tela, se escondendo no infinito */
   if (p1->x < 0) p1->x = 0;
   if (p1->x > 625) p1->x = 625;
}

void execute_attack(Player* p1, Player* p2)
{
   int gap;

   /* Verifica se o golpe é "válido", isto é,
    * se o J1 der um soco e o J2 estiver agachado, em teoria
    * o soco não deveria contar. O mesmo vale para o chute + pulo. */
   if (!(p1->move == ATTACKING_SUP && p2->move != CROUCHING) && !(p1->move == ATTACKING_INF && p2->move != JUMPING))
      return;
   
   if (p1->pos_flag == 0)      gap = 0;
   else if (p1->pos_flag == 1) gap = 70;

   if (p1->selected_char == RYU){
      if (p1->move == ATTACKING_SUP){
         if (is_area_colliding(p1->x - gap, p1->y, p1->w, p1->h, p2->x, p2->y, p2->w, p2->h)) p2->hit_points -= 5;
      }
      if (p1->move == ATTACKING_INF){
         if (is_area_colliding(p1->x - gap, p1->y, p1->w, p1->h, p2->x, p2->y, p2->w, p2->h)) p2->hit_points -= 2;
      }
   }
   else if (p1->selected_char == KEN){
      if (p1->move == ATTACKING_SUP){
         if (is_area_colliding(p1->x - gap, p1->y, p1->w, p1->h, p2->x, p2->y, p2->w, p2->h)) p2->hit_points -= 5;
      }
      if (p1->move == ATTACKING_INF){
         if (is_area_colliding(p1->x - gap, p1->y, p1->w, p1->h, p2->x, p2->y, p2->w, p2->h)) p2->hit_points -= 2;
      }
   }
}

/* ------------------
 *   Player Sprite
 * ------------------ */
void update_player_flags(Player* p1, Player* p2)
{
   if (p1->x < p2->x){
      p1->pos_flag = 0;
      p2->pos_flag = ALLEGRO_FLIP_HORIZONTAL;
   }
   else {
      p1->pos_flag = ALLEGRO_FLIP_HORIZONTAL;
      p2->pos_flag = 0;
   }
}

bool draw_sprite_player(Player* p, float* frame)
{
   unsigned short gap;
   int flag = p->pos_flag;
   bool reset_frame = false;

   if (p->pos_flag == 0) gap = 0;
   else gap = 70;

   /* Exibição de Sprites - RYU */
   if (p->selected_char == RYU){
      /*
      if (p->joystick->up){
         if (*frame <= 6) *frame += 0.15;
         else reset_frame = true;
         al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 115, p->x, p->y - 50*(int)(*frame), 70*2.5, 95*2.5, flag);
      }
      */
      if ((p->joystick->left && p->pos_flag == 0) || (p->joystick->right && p->pos_flag == 1)){
         if (*frame <= 5) *frame += 0.15;
         else *frame = 0;
         al_draw_scaled_bitmap(p->sprites->walking_neg, 70*(int)(*frame), 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
      else if ((p->joystick->right && p->pos_flag == 0) || (p->joystick->left && p->pos_flag == 1)){
         if (*frame <= 5) *frame += 0.15;
         else *frame = 0;
         al_draw_scaled_bitmap(p->sprites->walking_pos, 70*(int)(*frame), 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
      else if (p->joystick->down){
         al_draw_scaled_bitmap(p->sprites->crouch, 140, 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
      else if (p->joystick->button_1){
         al_draw_scaled_bitmap(p->sprites->attack_sup, 70, 0, 110, 95, p->x - gap, p->y, 110*2.5, 95*2.5, flag);
      }
      else if (p->joystick->button_2){
         al_draw_scaled_bitmap(p->sprites->attack_inf, 140, 0, 90, 95, p->x - gap, p->y, 90*2.5, 95*2.5, flag);
      }
      else {
         if (*frame <= 4) *frame += 0.15;
         else *frame = 0;         
         al_draw_scaled_bitmap(p->sprites->idle, 70*(int)(*frame), 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
   }
   
   /* Exibição de Sprites - KEN */
   else if (p->selected_char == KEN){
      /*
      if (p->joystick->up){
         if (*frame <= 7) *frame += 0.15;
         else reset_frame = true;

         if (*frame <=4) al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 105, p->x, p->y - 50*(int)(*frame), 70*2.5, 95*2.5, flag);
         else            al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 105, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
      */
      if ((p->joystick->left && p->pos_flag == 0) || (p->joystick->right && p->pos_flag == 1)){
         if (*frame <= 5) *frame += 0.15;
         else *frame = 0;
         al_draw_scaled_bitmap(p->sprites->walking_neg, 70*(int)(*frame), 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
      else if ((p->joystick->right && p->pos_flag == 0) || (p->joystick->left && p->pos_flag == 1)){
         if (*frame <= 5) *frame += 0.15;
         else *frame = 0;
         al_draw_scaled_bitmap(p->sprites->walking_pos, 70*(int)(*frame), 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
      else if (p->joystick->down){
         al_draw_scaled_bitmap(p->sprites->crouch, 140, 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
      else if (p->joystick->button_1){
         al_draw_scaled_bitmap(p->sprites->attack_sup, 70, 0, 105, 95, p->x - gap, p->y, 110*2.5, 95*2.5, flag);
      }
      else if (p->joystick->button_2){
         al_draw_scaled_bitmap(p->sprites->attack_inf, 130, 0, 90, 95, p->x - gap, p->y, 90*2.5, 95*2.5, flag);
      }
      else {
         if (*frame <= 5) *frame += 0.15;
         else *frame = 0;
         al_draw_scaled_bitmap(p->sprites->idle, 70*(int)(*frame), 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
   }

   return reset_frame;
}