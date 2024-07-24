#include <allegro5/allegro5.h>
#include <stdlib.h>
#include <stdbool.h>

#include "joystick.h"
#include "sprite.h"
#include "player.h"

/* ------------------
 *   Create and Destroy
 * ------------------ */

Player* create_player(Character character, unsigned short x, unsigned short y,
                      unsigned short w, unsigned short h)
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
bool is_player_colliding(Player *p1, Player *p2)
{
   /* Colisão de Player01 -> Player02 */
   /* Canto Superior Esquerdo de Player 01 */
   if ((p2->x <= p1->x) && (p1->x <= (p2->x + p2->w)) &&
       (p2->y <= p1->y) && (p1->y <= (p2->y + p2->h)))
      return true;
   /* Canto Superior Direito de Player 01 */
   if ((p2->x <= (p1->x + p1->w)) && ((p1->x + p1->w) <= (p2->x + p2->w)) &&
       (p2->y <= p1->y) && (p1->y <= (p2->y + p2->h)))
      return true;
   /* Canto Inferior Esquerdo de Player 01 */
   if ((p2->x <= p1->x) && (p1->x <= (p2->x + p2->w)) &&
       (p2->y <= (p1->y + p1->h)) && ((p1->y + p1->h) <= (p2->y + p2->h)))
      return true;
   /* Canto Inferior Direito de Player 01 */
   if ((p2->x <= (p1->x + p1->w)) && ((p1->x + p1->w) <= (p2->x + p2->w)) &&
       (p2->y <= (p1->y + p1->h)) && ((p1->y + p1->h) <= (p2->y + p2->h)))
      return true;

   /* Colisão de Player02 -> Player01 */
   /* Canto Superior Esquerdo de Player 02 */
   if ((p1->x <= p2->x) && (p2->x <= (p1->x + p1->w)) &&
       (p1->y <= p2->y) && (p2->y <= (p1->y + p1->h)))
      return true;
   /* Canto Superior Direito de Player 02 */
   if ((p1->x <= (p2->x + p2->w)) && ((p2->x + p2->w) <= (p1->x + p1->w)) &&
       (p1->y <= p2->y) && (p2->y <= (p1->y + p1->h)))
      return true;
   /* Canto Inferior Esquerdo de Player 02 */
   if ((p1->x <= p2->x) && (p2->x <= (p1->x + p1->w)) &&
       (p1->y <= (p2->y + p2->h)) && ((p2->y + p2->h) <= (p1->y + p1->h)))
      return true;
   /* Canto Inferior Direito de Player 02 */
   if ((p1->x <= (p2->x + p2->w)) && ((p2->x + p2->w) <= (p1->x + p1->w)) &&
       (p1->y <= (p2->y + p2->h)) && ((p2->y + p2->h) <= (p1->y + p1->h)))
      return true;

   return false;
}

/* ------------------
 *   Player Movement
 * ------------------ */

bool isUpMovementValid(Player* player, unsigned short steps, unsigned short velocity)
{
   return ((player->y - steps * velocity) - player->h / 2 >= 0);
}

bool isDownMovementValid(Player* player, unsigned short steps, unsigned short max_y,
                         unsigned short velocity)
{
   return ((player->y + steps * velocity) + player->h / 2 <= max_y);
}

bool isLeftMovementValid(Player* player, unsigned short steps, unsigned short velocity)
{
   return ((player->x - steps * velocity) - player->w / 2 >= 0);
}

bool isRightMovementValid(Player* player, unsigned short steps, unsigned short velocity,
                          unsigned short max_x)
{
   return ((player->x + steps * velocity) + player->w / 2 <= max_x);
}

int move_player(Player* player, Direction direction, unsigned short steps,
                unsigned short velocity, unsigned short max_y, unsigned short max_x)
{
   if (direction == UP){ 
      if (isUpMovementValid(player, steps, velocity))
         player->y -= steps * velocity;
   }
   else if (direction == DOWN){
      if (isDownMovementValid(player, steps, velocity, max_y))
         player->y += steps * velocity;
   }
   else if (direction == LEFT){
      if (isLeftMovementValid(player, steps, velocity))
         player->x -= steps * velocity;
   }
   else if (direction == RIGHT){
      if (isRightMovementValid(player, steps, velocity, max_x))
         player->x += steps * velocity;
   }
}