#include <string.h>
#include "player.h"
#include "sprite.h"

Sprite* create_sprites(Character fighter_selected)
{
   char* pathfile;

   Sprite* sprites = (Sprite*) malloc(sizeof(Sprite));
   if (sprites == NULL) return NULL;

   if (fighter_selected == RYU)
      pathfile = strdup("./assets/characters/ryu");
   else if (fighter_selected == CHUNLI)
      pathfile = strdup("./assets/characters/chun_li");
   else {
      free(sprites); return NULL;
   }
   sprites->idle = al_load_bitmap(pathfile);

   free(pathfile);
}