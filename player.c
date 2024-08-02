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

Player* create_player(Character character, int x0, int y0, int w0, int h0)
{
   /* Alocação de Memória */
   Player* new_player = (Player*) malloc(sizeof(Player));
   if (!new_player) return NULL;

   /* Personagem */
   new_player->selected_char = character;

   /* Coordenadas e Medidas */
   new_player->x = x0;
   new_player->y = y0;
   new_player->w = w0;
   new_player->h = h0;

   new_player->x_hit = x0;
   new_player->y_hit = y0;
   new_player->w_hit = w0;
   new_player->h_hit = h0;

   new_player->x_hurt = x0;
   new_player->y_hurt = y0;
   new_player->w_hurt = w0;
   new_player->h_hurt = h0;

   /* Pontos de Vida */
   new_player->hit_points = DEFAULT_HIT_POINTS;

   /* Flags, Gotta capture'em all! */
   new_player->pos_flag = 0;
   new_player->isJumping = new_player->isFalling = false;
   new_player->isAtkSup  = new_player->isAtkInf  = false;
   new_player->isCrouching = false;

   new_player->enableJump = 0;
   new_player->enableAtkSup = new_player->enableAtkInf = 0;

   /* Movimento inicial: Parado */
   new_player->move = IDLE;

   /* Estruturas */
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
   if (p1->joystick->up || p1->isJumping){
      p1->move = JUMPING;
   }

   /* Possibilidade 02: Andar Positivo */
   else if (p1->joystick->right)
      p1->move = WALKING_POSITIVE;

   /* Possibilidade 03: Andar Negativo */
   else if (p1->joystick->left)
      p1->move = WALKING_NEGATIVE;

   /* Possibilidade 04: Agachar */
   else if (p1->joystick->down){
      p1->move = CROUCHING;
   }
   /* Possibilidade 05: Ataque Superior */
   else if (p1->joystick->button_1){
      p1->move = ATTACKING_SUP;
   }
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
   if (p1->move == IDLE || p1->move == CROUCHING)
      return;

   /* Possibilidade 02: Jogador ANDANDO POSITIVO
    * Incrementa-se a coordenada x do jogador baseado na direção
    * que seu personagem está olhando. */
   else if (p1->move == WALKING_POSITIVE){
      if (p1->pos_flag == 0) p1->x += STEP_FRONT;
      else                   p1->x += STEP_BACK;
      if (is_area_colliding(p1->x_hit, p1->y_hit, floor(p1->w_hit*2.5), floor(p1->h_hit*2.5), 
                            p2->x_hit, p2->y_hit, floor(p2->w_hit*2.5), floor(p2->h_hit*2.5))){
         if (p1->pos_flag == 0) p1->x -= ceil(STEP_FRONT*2.5);
         else                   p1->x -= ceil(STEP_BACK*2.5);
      }
   }

   /* Possibilidade 03: Jogador ANDANDO NEGATIVO
    * Decrementa-se a coordenada x do jogador baseado na direção
    * que seu personagem está olhando. */
   else if (p1->move == WALKING_NEGATIVE){
      if (p1->pos_flag == 0) p1->x -= STEP_BACK;
      else                   p1->x -= STEP_FRONT;
      if (is_area_colliding(p1->x_hit, p1->y_hit, floor(p1->w_hit*2.5), floor(p1->h_hit*2.5), 
                            p2->x_hit, p2->y_hit, floor(p2->w_hit*2.5), floor(p2->h_hit*2.5))){
         if (p1->pos_flag == 0) p1->x += ceil(STEP_BACK*2.5);
         else                   p1->x += ceil(STEP_FRONT*2.5);
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
         else                   p1->x -= STEP_BACK;
      }
      if (p1->joystick->right){
         if (p1->pos_flag == 0) p1->x += STEP_BACK;
         else                   p1->x += STEP_FRONT;
      }

      /* Parte 02: Fazendo o jogador pular */
      if (p1->isJumping){
         if (p1->y > Y_MAX){
            p1->y = Y_MAX;
            p1->move = IDLE;
            p1->isJumping = false;
            p1->isFalling = false;
         }

         if (p1->y < Y_MIN){
            p1->y = Y_MIN;
            p1->isFalling = true;
         }

         if (p1->isFalling) p1->y += GRAVITY;
         else               p1->y -= GRAVITY;
      }
   }

   /* Limites da Tela: Corrige o problema do jogador
    * ultrapassar os limites da tela, se escondendo no infinito */
   if (p1->x < 0) p1->x = 0;
   if (p1->x > 625) p1->x = 625;
}

/* LEMBRETE PARA AMANHÃ (30/07)
 * As coordenadas de Hitbox e Hurtbox TEM
 * que ser atualizadas a cada iteração!! */
void update_player_boxes(Player* p, float frame)
{
   /* Distância que corrige o problema do soco/chute
    * do jogador da direita, que vai para trás ao invés
    * de socar/chutar para frente */
   int gap_punch_RYU = (p->pos_flag == 0) ? 0 : 280;
   int gap_punch_KEN = (p->pos_flag == 0) ? 0 : 290;

   int gap_kick_RYU = (p->pos_flag == 0) ? 0 : 250;
   int gap_kick_KEN = (p->pos_flag == 0) ? 0 : 240;

   /* ----------------------
    * Atualização da Hurtbox
    * ---------------------- */
   /* Jogador realiza um Soco */
   if (p->move == ATTACKING_SUP){
      if (p->selected_char == RYU){
         p->x_hurt = p->x - gap_punch_RYU + 230;
         p->y_hurt = p->y + 45;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }
      else if (p->selected_char == KEN){
         p->x_hurt = p->x - gap_punch_KEN + 235;
         p->y_hurt = p->y + 45;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }
   }
   /* Jogador realiza um Chute */
   else if (p->move == ATTACKING_INF){
      if (p->selected_char == RYU){
         p->x_hurt = p->x - gap_kick_RYU + 190;
         p->y_hurt = p->y + 145;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }
      if (p->selected_char == KEN){
         p->x_hurt = p->x - gap_kick_KEN + 185;
         p->y_hurt = p->y + 150;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }
   }
   else {
      p->x_hurt = p->x + 15;
      p->y_hurt = p->y + 15;
      p->w_hurt = 70 - 15;
      p->h_hurt = 95 - 15;
   }

   /* ---------------------
    * Atualização da Hitbox
    * --------------------- */
   /* Jogador está Agachado */
   if (p->move == CROUCHING){
      p->x_hit = p->x + 25;
      p->y_hit = p->y + 70 + 25;
      p->w_hit = 70 - 25;
      p->h_hit = 65 - 25;
   }
   else {
      p->x_hit = p->x + 25;
      p->y_hit = p->y + 25;
      p->w_hit = 70 - 25;
      p->h_hit = 95 - 25;
   }
}

void execute_attack(Player* p1, Player* p2)
{
   if (p1->move == ATTACKING_SUP){
      if (is_area_colliding(p1->x_hurt, p1->y_hurt, ceil(p1->w_hurt*2.5), ceil(p1->h_hurt*2.5), 
                            p2->x_hit,  p2->y_hit,  ceil(p2->w_hit*2.5),  ceil(p2->h_hit*2.5)))
         p2->hit_points -= 5;
   }
   if (p1->move == ATTACKING_INF){
      if (is_area_colliding(p1->x_hurt, p1->y_hurt, ceil(p1->w_hurt*2.5), ceil(p1->h_hurt*2.5), 
                            p2->x_hit,  p2->y_hit,  ceil(p2->w_hit*2.5),  ceil(p2->h_hit*2.5)))
         p2->hit_points -= 2;
   }
}

/* ------------------
 *   Player Sprite
 * ------------------ */
void update_player_pos_flags(Player* p1, Player* p2)
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
      if (p->isJumping){
         if (*frame <= 6) *frame += 0.15;
         else reset_frame = true;
         
         if (*frame <= 3) al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 115, p->x, p->y - 50*(int)(*frame), 70*2.5, 115*2.5, flag);
         else             al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 115, p->x, p->y, 70*2.5, 115*2.5, flag);
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
      else if (p->joystick->button_1 || p->enableAtkSup == 1){
         al_draw_scaled_bitmap(p->sprites->attack_sup, 70, 0, 110, 95, p->x - gap, p->y, 110*2.5, 95*2.5, flag);
      }
      else if (p->joystick->button_2 || p->enableAtkSup == 1){
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
      if (p->isJumping){
         if (*frame <= 7) *frame += 0.15;
         else reset_frame = true;

         if (*frame <= 4) al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 105, p->x, p->y - 50*(int)(*frame), 70*2.5, 95*2.5, flag);
         else             al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 105, p->x, p->y, 70*2.5, 95*2.5, flag);
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
      else if (p->enableAtkSup >= 1 && p->enableAtkSup <= 10){
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