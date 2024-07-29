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

void update_player_movement(Player* p)
{
   /* Possibilidade 01: Parado 
   * 1) Todos os inputs estão 0 
   * 2) CIMA e BAIXO ativos
   * 3) ESQUERDA e DIREITA ativos */
   if (( !(p->joystick->up) && !(p->joystick->down) 
         && !(p->joystick->left) && !(p->joystick->right) 
         && !(p->joystick->button_1) && !(p->joystick->button_2) )
         || (p->joystick->up && p->joystick->down)
         || (p->joystick->left && p->joystick->right))
      p->move = IDLE;

   /* Possibilidade 02: Andar Positivo */
   else if (p->joystick->right)
      p->move = WALKING_POSITIVE;

   /* Possibilidade 03: Andar Negativo */
   else if (p->joystick->left)
      p->move = WALKING_NEGATIVE;
   
   /* Possibilidade 04: Pular */
   if (p->joystick->up && p->isJumping == false){
      p->move = JUMPING;
      p->isJumping = true;
   }

   /* Possibilidade 05: Agachar */
   if (p->joystick->down)
      p->move = CROUCHING;

   /* Possibilidade 06: Ataque Superior */
   if (p->joystick->button_1)
      p->move = ATTACKING_SUP;

   /* Possibilidade 07: Ataque Inferior */
   if (p->joystick->button_2)
      p->move = ATTACKING_INF;
}

void update_player_coordinates(Player* p)
{
   /* Possibilidade 01: Jogador PARADO 
    * Se o jogador está parado, mantém as coordenadas */
   if (p->move == IDLE)
      return;

   /* Possibilidade 02: Jogador ANDANDO POSITIVO
    * Incrementa-se a coordenada x do jogador baseado na direção
    * que seu personagem está olhando. */
   else if (p->move == WALKING_POSITIVE){
      if (p->pos_flag == 0) p->x += STEP_FRONT;
      else p->x += STEP_BACK;
   }

   /* Possibilidade 03: Jogador ANDANDO NEGATIVO
    * Decrementa-se a coordenada x do jogador baseado na direção
    * que seu personagem está olhando. */
   else if (p->move == WALKING_NEGATIVE){
      if (p->pos_flag == 0) p->x -= STEP_BACK;
      else p->x -= STEP_FRONT;
   }

   /* Possibilidade 04: Jogador PULANDO 
    * Aqui as coisas complicam um pouco mais:
    * 1) Primeiro, verificamos se o jogador quer movimentar durante o pulo
    * 2) Segundo, verificamos se o jogador atingiu a altura máxima de um pulo
    * (não queremos o jogador entrando em órbita), se ele já atingiu a altura
    * máxima, fazemos y = Y_MAX
    * 3) Ajustamos a coordenada y do jogador */
   else if (p->move == JUMPING){
      /* Parte 01: Movento Esquerda-Direita no meio do ar */
      if (p->joystick->left){
         if (p->pos_flag == 0) p->x -= STEP_FRONT;
         else p->x -= STEP_BACK;
      }
      if (p->joystick->right){
         if (p->pos_flag == 0) p->x += STEP_BACK;
         else p->x += STEP_FRONT;
      }

      /* Parte 02: Fazendo o jogador pular */
      if (p->y >= Y_MAX){
         p->y = Y_MAX;
         p->move = IDLE;
      }
      else {
         if (p->y <= Y_MIN) p->isFalling = true;

         if (p->isFalling) p->y += JUMP_VEL;
         else p->y -= JUMP_VEL;
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
      if (p->joystick->up){
         if (*frame <= 6) *frame += 0.15;
         else reset_frame = true;
         al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 115, p->x, p->y - 50*(int)(*frame), 70*2.5, 95*2.5, flag);
      }
      else if ((p->joystick->left && p->pos_flag == 0) || (p->joystick->right && p->pos_flag == 1)){
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
      if (p->joystick->up){
         if (*frame <= 7) *frame += 0.15;
         else reset_frame = true;

         if (*frame <=4) al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 105, p->x, p->y - 50*(int)(*frame), 70*2.5, 95*2.5, flag);
         else            al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 105, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
      else if ((p->joystick->left && p->pos_flag == 0) || (p->joystick->right && p->pos_flag == 1)){
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