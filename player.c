#include <allegro5/allegro5.h>
#include <stdlib.h>
#include <stdbool.h>

#include "joystick.h"
#include "player.h"

Player* create_player(Character character, unsigned short x, unsigned short y,
                      unsigned short w, unsigned short h)
{
   Player* new_player = (Player*) malloc(sizeof(Player));
   if (!new_player) return NULL;
   new_player->selected_char = character;
   new_player->joystick = joystick_create();
   new_player->current_x = x;
   new_player->current_y = y;
   new_player->current_lenght = w;
   new_player->current_height = h;
   new_player->hit_points = 100;
}

void destroy_player(Player* player)
{
   // Elementos internos de Player
   joystick_destroy(player->joystick);

   free(player);
   player = NULL;
}

/* ------------------
 *   Player Movement
 * ------------------ */
bool isUpMovementValid(Player* player, unsigned short steps, unsigned short velocity)
{
   return ((player->current_y - steps * velocity) - player->current_height / 2 >= 0);
}

bool isDownMovementValid(Player* player, unsigned short steps, unsigned short max_y,
                         unsigned short velocity)
{
   return ((player->current_y + steps * velocity) + player->current_height / 2 <= max_y);
}

bool isLeftMovementValid(Player* player, unsigned short steps, unsigned short velocity)
{
   return ((player->current_x - steps * velocity) - player->current_lenght / 2 >= 0);
}

bool isRightMovementValid(Player* player, unsigned short steps, unsigned short velocity,
                          unsigned short max_x)
{
   return ((player->current_x + steps * velocity) + player->current_lenght / 2 <= max_x);
}

int move_player(Player* player, unsigned short direction, unsigned short steps,
                unsigned short velocity, unsigned short max_y, unsigned short max_x)
{
   if (direction == UP){ 
      if (isUpMovementValid(player, steps, velocity))
         player->current_y += steps * velocity;
   }
   else if (direction == DOWN){
      if (isDownMovementValid(player, steps, velocity, max_y))
         player->current_y -= steps * velocity;
   }
   else if (direction == LEFT){
      if (isLeftMovementValid(player, steps, velocity))
         player->current_x -= steps * velocity;
   }
   else if (direction == RIGHT){
      if (isRightMovementValid(player, steps, velocity, max_x))
         player->current_x += steps * velocity;
   }
}