/* King of Workshop (pt-br: O Rei da Oficina) */
/* Arquivo principal */
/* Executará os eventos e as inicializações necessárias */

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define TRUE 1
#define FALSE 0
#define DISPLAY_WIDTH  800
#define DISPLAY_HEIGHT 600

int main(int argc, char* argv[])
{
   al_init();
   al_init_font_addon();
   al_init_ttf_addon();

   ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
   ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
   ALLEGRO_FONT* font = al_load_ttf_font("./assets/font/Fatal Fighter.ttf", 32, 0);
   ALLEGRO_DISPLAY* display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
   
   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   
   al_start_timer(timer);
   ALLEGRO_EVENT event;
   ALLEGRO_TRANSFORM transf;

   unsigned short largura_string = al_get_text_width(font, "STREET FIGHTER");
   unsigned short altura_string = al_get_font_line_height(font);
   unsigned short largura, altura;
   unsigned short print_mov = 0;
   float movement = 0.0;
   while (TRUE)
   {
      al_wait_for_event(event_queue, &event);

      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
      else if (event.type == ALLEGRO_EVENT_TIMER){
         movement += 0.5;
         if (movement >= altura_string) movement = 0.0;
         //if (print_mov > 1) print_mov = 0;
         //else print_mov++;

         al_clear_to_color(al_map_rgb(34, 35, 36));

         al_identity_transform(&transf);
         al_rotate_transform(&transf, 0.5f);
         al_use_transform(&transf);
         for (altura = 0; altura < 2 * DISPLAY_HEIGHT; altura += altura_string)
            for (largura = 0; largura < 2 * DISPLAY_WIDTH; largura += largura_string + 10)
               al_draw_text(font, al_map_rgb(77, 78, 79), largura, -(altura_string + 10 + 400) + (altura + 10) + (int) movement, 0, "STREET FIGHTER");

         al_identity_transform(&transf);
         al_use_transform(&transf);
         al_draw_text(font, al_map_rgb(255, 255, 255), 400, 100, ALLEGRO_ALIGN_CENTER, "ESCOLHA SEU PERSONAGEM");
         
         al_flip_display();
      }
      //if (print_mov == 1){
         //if (movement < altura_string) movement += 1;
         //else movement = 0;
      //}
   }

   al_destroy_font(font);
   al_destroy_display(display);
   al_destroy_timer(timer);
   al_destroy_event_queue(event_queue);
}