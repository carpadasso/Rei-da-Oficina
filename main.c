/* King of Workshop (pt-br: O Rei da Oficina) */
/* Arquivo principal */
/* Executará os eventos e as inicializações necessárias */

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "player.h"

int main(int argc, char* argv[])
{
   al_init();
   al_install_keyboard();

   ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
   ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
   ALLEGRO_FONT* font = al_create_builtin_font();
   ALLEGRO_DISPLAY* display = al_create_display(800, 600);
   
   al_register_event_source(event_queue, al_get_keyboard_event_source());
   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   
   al_start_timer(timer);
   ALLEGRO_EVENT event;
   
   char flag = 1;
   while (1){
      al_wait_for_event(event_queue, &event);

      if (event.type == ALLEGRO_EVENT_TIMER){
         if (flag < 5){
            al_clear_to_color(al_map_rgb(0, 0, 0));
            flag++;
         }
         else {
            al_clear_to_color(al_map_rgb(255, 255, 255));
            flag++;
            if (!(flag % 10)) flag = 0;
         }
         al_flip_display();
      }
      else if (event.type == 42) break;
   }

   al_destroy_font(font);
   al_destroy_display(display);
   al_destroy_timer(timer);
   al_destroy_event_queue(event_queue);
}
