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