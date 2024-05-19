/* King of Workshop (pt-br: O Rei da Oficina) */
/* Arquivo principal */
/* Executará os eventos e as inicializações necessárias */

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#define TRUE 1
#define FALSE 0

int main(int argc, char* argv[])
{
   al_init();
   al_init_font_addon();

   ALLEGRO_TIMER* timer = al_create_timer(1.0 / 2000.0);
   ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
   ALLEGRO_FONT* font = al_create_builtin_font();
   ALLEGRO_DISPLAY* display = al_create_display(800, 600);
   
   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   
   al_start_timer(timer);
   ALLEGRO_EVENT event;

   unsigned short ida = TRUE;
   unsigned short cor = 0;

   while (TRUE){
      al_wait_for_event(event_queue, &event);
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;

      if (event.type == ALLEGRO_EVENT_TIMER){
         if (ida) cor++;
         else cor--;

         if (cor == 0) ida = TRUE;
         if (cor == 255) ida = FALSE;

         al_clear_to_color(al_map_rgb(cor, cor, cor));
         al_flip_display();
      }
   }

   al_destroy_font(font);
   al_destroy_display(display);
   al_destroy_timer(timer);
   al_destroy_event_queue(event_queue);
}