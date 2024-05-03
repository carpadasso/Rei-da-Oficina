#include <allegro5/allegro5.h>

typedef struct {
   unsigned short characterSelected;
   unsigned short x;
   unsigned short y;
} Player;

Player* createPlayer(unsigned short charSelect, unsigned short xInit, unsigned short yInit, unsigned short xMax, unsigned short yMax);
