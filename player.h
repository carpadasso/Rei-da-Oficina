#include <allegro5/allegro5.h>

#define DISPLAY_WIDTH  800
#define DISPLAY_HEIGHT 600
#define REFRESH_RATE 60.0

typedef enum { RYU, KEN } Character;

typedef struct {
   Character selected_char;
   unsigned short current_position_x;
   unsigned short current_position_y;
   unsigned short current_lenght;
   unsigned short current_height;
} Player;

Player create_player(Character char_selected, unsigned short x, unsigned short y, unsigned short w, unsigned short h);