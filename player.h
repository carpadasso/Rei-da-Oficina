#include <allegro5/allegro5.h>

#define DISPLAY_WIDTH  800
#define DISPLAY_HEIGHT 600

typedef struct {
   unsigned short selected_char;
   unsigned short current_position_x;
   unsigned short current_position_y;
} Player;

Player* createPlayer(unsigned short charSelect, unsigned short xInit, unsigned short yInit, unsigned short xMax, unsigned short yMax);
