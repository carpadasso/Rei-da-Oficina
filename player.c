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
   /* Teste de Sanidade... */
   if (character != RYU && character != KEN) return NULL;

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
   /* Teste de Sanidade... */
   if (player == NULL) return;

   /* Destrói as estruturas Joystick e Sprites */
   destroy_joystick(player->joystick);
   destroy_sprites(player->sprites);

   free(player); player = NULL;
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
   /* Teste de Sanidade... */
   if (p1 == NULL || p2 == NULL) return;

   /* Movimentos realizados PULANDO */
   if      ((p1->joystick->up || p1->isJumping) && p1->joystick->button_1)
      p1->move = ATTACK_HP;
   else if ((p1->joystick->up || p1->isJumping) && p1->joystick->button_2)
      p1->move = ATTACK_HK;
   else if ((p1->joystick->up || p1->isJumping))
      p1->move = JUMP;

   /* Movimentos realizados AGACHADO */
   else if (p1->joystick->down && p1->joystick->button_1)
      p1->move = ATTACK_LP;
   else if (p1->joystick->down && p1->joystick->button_2)
      p1->move = ATTACK_LK;
   else if (p1->joystick->down)
      p1->move = CROUCH;

   /* Ataques Padrão */
   else if (p1->joystick->button_1)
      p1->move = ATTACK_MP;
   else if (p1->joystick->button_2)
      p1->move = ATTACK_MK;

   /* Andando Esquerda ou Direita */
   else if (p1->joystick->left)
      p1->move = WALK_NEGATIVE;
   else if (p1->joystick->right)
      p1->move = WALK_POSITIVE;

   /* Zero Movimentos */
   else
      p1->move = IDLE;
}

void update_player_coordinates(Player* p1, Player* p2)
{  
   /* Teste de Sanidade... */
   if (p1 == NULL || p2 == NULL) return;

   /* Possibilidade 02: Jogador ANDANDO POSITIVO
    * Incrementa-se a coordenada x do jogador baseado na direção
    * que seu personagem está olhando. */
   if (p1->move == WALK_POSITIVE){
      if (p1->pos_flag == 0) p1->x += STEP_FRONT;
      else                   p1->x += STEP_BACK;

      /* Se ocorreu COLISÃO, desfazemos o movimento! */
      if (is_area_colliding(p1->x_hit, p1->y_hit, ceil(p1->w_hit*2.5), ceil(p1->h_hit*2.5), 
                            p2->x_hit, p2->y_hit, ceil(p2->w_hit*2.5), ceil(p2->h_hit*2.5))){
         if (p1->pos_flag == 0) p1->x -= STEP_FRONT;
         else                   p1->x -= STEP_BACK;
      }
   }

   /* Possibilidade 03: Jogador ANDANDO NEGATIVO
    * Decrementa-se a coordenada x do jogador baseado na direção
    * que seu personagem está olhando. */
   else if (p1->move == WALK_NEGATIVE){
      if (p1->pos_flag == 0) p1->x -= STEP_BACK;
      else                   p1->x -= STEP_FRONT;

      /* Se ocorreu COLISÃO... desfazemos o movimento! */
      if (is_area_colliding(p1->x_hit, p1->y_hit, ceil(p1->w_hit*2.5), ceil(p1->h_hit*2.5), 
                            p2->x_hit, p2->y_hit, ceil(p2->w_hit*2.5), ceil(p2->h_hit*2.5))){
         if (p1->pos_flag == 0) p1->x += STEP_BACK;
         else                   p1->x += STEP_FRONT;
      }
   }

   /* Possibilidade 04: Jogador PULANDO 
    * Aqui as coisas complicam um pouco mais:
    * 1) Primeiro, verificamos se o jogador quer movimentar durante o pulo
    * 2) Segundo, verificamos se o jogador atingiu a altura máxima de um pulo
    * (não queremos o jogador entrando em órbita), se ele já atingiu a altura
    * máxima, fazemos y = Y_MAX
    * 3) Ajustamos a coordenada y do jogador */
   else if (p1->move == JUMP || p1->move == ATTACK_HP || p1->move == ATTACK_HK){
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

void update_player_boxes(Player* p)
{
   /* Distância que corrige o problema do soco/chute
    * do jogador da direita, que vai para trás ao invés
    * de socar/chutar para frente */
   int gap;

   if (p->pos_flag == 0)
      gap = 0;
   else if (p->pos_flag == 1){
      /* Jogador realiza um Soco */
      if (p->move == ATTACK_MP){
         if (p->selected_char == RYU)
            gap = 280;
         else if (p->selected_char == KEN)
            gap = 290;
      }
      else if (p->move == ATTACK_HP){
         if (p->selected_char == RYU){
            gap = 280;
         }
         else if (p->selected_char == KEN){
            gap = 280;
         }      
      }
      else if (p->move == ATTACK_LP){
         if (p->selected_char == RYU){
            gap = 290;
         }
         else if (p->selected_char == KEN){
            gap = 290;
         }
      }

      /* Jogador realiza um Chute */
      else if (p->move == ATTACK_MK){
         if (p->selected_char == RYU)
            gap = 250;
         else if (p->selected_char == KEN)
            gap = 240;
      }
      else if (p->move == ATTACK_HK){
         if (p->selected_char == RYU)
            gap = 240;
         else if (p->selected_char == KEN)
            gap = 240;
      }
      else if (p->move == ATTACK_LK){
         if (p->selected_char == RYU)
            gap = 300;
         else if (p->selected_char == KEN)
            gap = 300;
      }
   }


   /* ----------------------
    * Atualização da Hurtbox
    * ---------------------- */
   /* Jogador realiza um Soco */
   if (p->move == ATTACK_MP){
      if (p->selected_char == RYU){
         p->x_hurt = p->x - gap + 230;
         p->y_hurt = p->y + 45;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }
      else if (p->selected_char == KEN){
         p->x_hurt = p->x - gap + 235;
         p->y_hurt = p->y + 45;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }
   }
   else if (p->move == ATTACK_HP){
      if (p->selected_char == RYU){
         p->x_hurt = p->x - gap + 215;
         p->y_hurt = p->y + 220;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }
      else if (p->selected_char == KEN){
         p->x_hurt = p->x - gap + 215;
         p->y_hurt = p->y + 190;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }      
   }
   else if (p->move == ATTACK_LP){
      if (p->selected_char == RYU){
         p->x_hurt = p->x - gap + 230;
         p->y_hurt = p->y + 115;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }
      else if (p->selected_char == KEN){
         p->x_hurt = p->x - gap + 230;
         p->y_hurt = p->y + 115;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }
   }

   /* Jogador realiza um Chute */
   else if (p->move == ATTACK_MK){
      if (p->selected_char == RYU){
         p->x_hurt = p->x - gap + 190;
         p->y_hurt = p->y + 145;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }
      if (p->selected_char == KEN){
         p->x_hurt = p->x - gap + 185;
         p->y_hurt = p->y + 150;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }
   }
   else if (p->move == ATTACK_HK){
      if (p->selected_char == RYU){
         p->x_hurt = p->x - gap + 180;
         p->y_hurt = p->y + 75;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }
      if (p->selected_char == KEN){
         p->x_hurt = p->x - gap + 180;
         p->y_hurt = p->y + 35;
         p->w_hurt = 10;
         p->h_hurt = 10;
      }
   }
   else if (p->move == ATTACK_LK){
      if (p->selected_char == RYU){
         p->x_hurt = p->x - gap + 240;
         p->y_hurt = p->y + 205;
         p->w_hurt = 50;
         p->h_hurt = 10;
      }
      if (p->selected_char == KEN){
         p->x_hurt = p->x - gap + 240;
         p->y_hurt = p->y + 205;
         p->w_hurt = 50;
         p->h_hurt = 10;
      }   
   }

   /* Hurtbox Padrão */
   else {
      p->x_hurt = p->x + 15;
      p->y_hurt = p->y + 15;
      p->w_hurt = 70 - 15;
      p->h_hurt = 95 - 15;
   }

   /* ---------------------
    * Atualização da Hitbox
    * --------------------- */
   /* Jogador está AGACHADO ou SOCO ABAIXO */
   if (p->move == CROUCH || p->move == ATTACK_LP){
      p->x_hit = p->x + 25;
      p->y_hit = p->y + 70 + 25;
      p->w_hit = 70 - 25;
      p->h_hit = 65 - 15;
   }
   /* Jogador faz CHUTE ABAIXO */
   else if (p->move == ATTACK_LK){
      p->x_hit = p->x + 70;
      p->y_hit = p->y + 70 + 25;
      p->w_hit = 70 - 25;
      p->h_hit = 65 - 15;
   }
   /* Hitbox Padrão */
   else {
      p->x_hit = p->x + 25;
      p->y_hit = p->y + 25;
      p->w_hit = 70 - 25;
      p->h_hit = 95 - 15;
   }
}

void execute_attack(Player* p1, Player* p2)
{
   if (p1->move == ATTACK_MP || p1->move == ATTACK_HP || p1->move == ATTACK_LP){
      if (is_area_colliding(p1->x_hurt, p1->y_hurt, ceil(p1->w_hurt*2.5), ceil(p1->h_hurt*2.5), 
                            p2->x_hit,  p2->y_hit,  ceil(p2->w_hit*2.5),  ceil(p2->h_hit*2.5)))
         p2->hit_points -= 10;
   }
   if (p1->move == ATTACK_MK || p1->move == ATTACK_HK || p1->move == ATTACK_LK){
      if (is_area_colliding(p1->x_hurt, p1->y_hurt, ceil(p1->w_hurt*2.5), ceil(p1->h_hurt*2.5), 
                            p2->x_hit,  p2->y_hit,  ceil(p2->w_hit*2.5),  ceil(p2->h_hit*2.5)))
         p2->hit_points -= 5;
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

   if   (p->pos_flag == 0) gap = 0;
   else gap = 70;

   /* -------------------
    * Exibição de Sprites
    * Personagem: RYU 
    * ------------------- */
   if (p->selected_char == RYU){
      /* ------------------------
       * Exibição de Movimentação
       * ------------------------ */
      if (p->move == JUMP && p->isJumping){
         if (*frame <= 6) *frame += 0.15;
         else reset_frame = true;
         
         if (*frame <= 3)
            al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 115, p->x, p->y - 50*(int)(*frame), 70*2.5, 115*2.5, flag);
         else            
            al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 115, p->x, p->y, 70*2.5, 115*2.5, flag);
      }
      else if ((p->move == WALK_NEGATIVE && p->pos_flag == 0) || (p->move == WALK_POSITIVE && p->pos_flag == 1)){
         if (*frame <= 5) *frame += 0.15;
         else *frame = 0;

         al_draw_scaled_bitmap(p->sprites->walk_negative, 70*(int)(*frame), 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
      else if ((p->move == WALK_NEGATIVE && p->pos_flag == 1) || (p->move == WALK_POSITIVE && p->pos_flag == 0)){
         if (*frame <= 5) *frame += 0.15;
         else *frame = 0;
         
         al_draw_scaled_bitmap(p->sprites->walk_positive, 70*(int)(*frame), 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
      else if (p->move == CROUCH){
         al_draw_scaled_bitmap(p->sprites->crouch, 140, 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }

      /* ---------------------------
       * Exibição de Ataques de SOCO
       * --------------------------- */
      else if ((p->move == ATTACK_HP || p->enableAtkSup == 1) && p->isJumping){
         al_draw_scaled_bitmap(p->sprites->attack_hp, 150, 0, 100, 115, p->x - gap, p->y, 100*2.5, 115*2.5, flag);
      }
      else if ((p->move == ATTACK_MP || p->enableAtkSup == 1)){
         al_draw_scaled_bitmap(p->sprites->attack_mp, 70, 0, 110, 95, p->x - gap, p->y, 110*2.5, 95*2.5, flag);
      }
      else if ((p->move == ATTACK_LP || p->enableAtkSup == 1)){
         al_draw_scaled_bitmap(p->sprites->attack_lp, 80, 0, 105, 95, p->x - gap, p->y, 105*2.5, 95*2.5, flag);
      }

      /* ----------------------------
       * Exibição de Ataques de CHUTE
       * ---------------------------- */
      else if ((p->move == ATTACK_HK || p->enableAtkInf == 1) && p->isJumping){
         al_draw_scaled_bitmap(p->sprites->attack_hk, 70, 0, 85, 115, p->x - gap, p->y, 85*2.5, 115*2.5, flag);
      }
      else if ((p->move == ATTACK_MK || p->enableAtkInf == 1)){
         al_draw_scaled_bitmap(p->sprites->attack_mk, 140, 0, 90, 95, p->x - gap, p->y, 90*2.5, 95*2.5, flag);
      }
      else if ((p->move == ATTACK_LK || p->enableAtkInf == 1)){
         al_draw_scaled_bitmap(p->sprites->attack_lk, 190, 0, 150, 95, p->x - gap, p->y, 150*2.5, 95*2.5, flag);
      }

      /* ---------------
       * Exibição PARADO
       * --------------- */
      else {
         if (*frame <= 4) *frame += 0.15;
         else *frame = 0;

         al_draw_scaled_bitmap(p->sprites->idle, 70*(int)(*frame), 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
   }
   
   /* -------------------
    * Exibição de Sprites
    * Personagem: KEN 
    * ------------------- */
   else if (p->selected_char == KEN){
      /* ------------------------
       * Exibição de Movimentação
       * ------------------------ */
      if (p->move == JUMP && p->isJumping){
         if (*frame <= 7) *frame += 0.15;
         else reset_frame = true;

         if (*frame <= 3)
            al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 105, p->x, p->y - 50*(int)(*frame), 70*2.5, 95*2.5, flag);
         else
            al_draw_scaled_bitmap(p->sprites->jump, 70*(int)(*frame), 0, 70, 105, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
      else if ((p->move == WALK_NEGATIVE && p->pos_flag == 0) || (p->move == WALK_POSITIVE && p->pos_flag == 1)){
         if (*frame <= 5) *frame += 0.15;
         else *frame = 0;

         al_draw_scaled_bitmap(p->sprites->walk_negative, 70*(int)(*frame), 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
      else if ((p->move == WALK_NEGATIVE && p->pos_flag == 1) || (p->move == WALK_POSITIVE && p->pos_flag == 0)){
         if (*frame <= 5) *frame += 0.15;
         else *frame = 0;

         al_draw_scaled_bitmap(p->sprites->walk_positive, 70*(int)(*frame), 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
      else if (p->move == CROUCH){
         al_draw_scaled_bitmap(p->sprites->crouch, 140, 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }

      /* ---------------------------
       * Exibição de Ataques de SOCO
       * --------------------------- */
      else if ((p->move == ATTACK_HP || p->enableAtkSup == 1) && p->isJumping){
         al_draw_scaled_bitmap(p->sprites->attack_hp, 150, 0, 100, 115, p->x - gap, p->y, 100*2.5, 115*2.5, flag);
      }
      else if ((p->move == ATTACK_MP || p->enableAtkSup == 1)){
         al_draw_scaled_bitmap(p->sprites->attack_mp, 70, 0, 105, 95, p->x - gap, p->y, 110*2.5, 95*2.5, flag);
      }
      else if ((p->move == ATTACK_LP || p->enableAtkSup == 1)){
         al_draw_scaled_bitmap(p->sprites->attack_lp, 80, 0, 105, 95, p->x - gap, p->y, 105*2.5, 95*2.5, flag);
      }

      /* ----------------------------
       * Exibição de Ataques de CHUTE
       * ---------------------------- */
      else if ((p->move == ATTACK_HK || p->enableAtkInf == 1) && p->isJumping){
         al_draw_scaled_bitmap(p->sprites->attack_hk, 70, 0, 85, 115, p->x - gap, p->y, 85*2.5, 115*2.5, flag);
      }
      else if ((p->move == ATTACK_MK || p->enableAtkInf == 1)){
         al_draw_scaled_bitmap(p->sprites->attack_mk, 130, 0, 90, 95, p->x - gap, p->y, 90*2.5, 95*2.5, flag);
      }
      else if ((p->move == ATTACK_LK || p->enableAtkInf == 1)){
         al_draw_scaled_bitmap(p->sprites->attack_lk, 190, 0, 150, 95, p->x - gap, p->y, 150*2.5, 95*2.5, flag);
      }

      /* ---------------
       * Exibição PARADO
       * --------------- */
      else {
         if (*frame <= 5) *frame += 0.15;
         else *frame = 0;

         al_draw_scaled_bitmap(p->sprites->idle, 70*(int)(*frame), 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
      }
   }

   return reset_frame;
}