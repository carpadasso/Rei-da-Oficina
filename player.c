#include <allegro5/allegro5.h>
#include <stdlib.h>
#include "player.h"

Player create_player(Character char_selected, unsigned short x, unsigned short y, unsigned short w, unsigned short h)
{
   Player jogador;

   jogador.selected_char = char_selected;
   jogador.current_position_x = x;
   jogador.current_position_y = y;
   jogador.current_lenght = w;
   jogador.current_height = h;

   return jogador;
}