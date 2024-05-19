/* King of Workshop (pt-br: O Rei da Oficina) */
/* Arquivo principal */
/* Executará os eventos e as inicializações necessárias */

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "player.h"

#define TRUE 1
#define FALSE 0

int main(int argc, char* argv[])
{
   // Variáveis de uso geral
   ALLEGRO_EVENT event;

   // Inits necessários
   al_init();
   al_init_font_addon();
   al_init_image_addon();
   al_install_keyboard();
   al_install_audio();
   al_init_acodec_addon();

   // Criação dos elementos do jogo
   ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
   ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
   ALLEGRO_DISPLAY* display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

   // Registro de fontes de eventos para a Event Queue
   al_register_event_source(event_queue, al_get_keyboard_event_source());
   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   
   // Começa o Timer
   al_start_timer(timer);
   
   // Altera o título e ícone do display aberto
   al_set_window_title(display, "Street Fighter II: Champion Edition");
   ALLEGRO_BITMAP* icon_display = al_load_bitmap("./assets/icon.png");
   al_set_display_icon(display, icon_display);

   /* --------------------------------------------------------------------------
   *  MENU INICIAL
   ---------------------------------------------------------------------------*/
   // Imagens do Menu
   ALLEGRO_BITMAP* logo_menu = al_load_bitmap("./assets/logo_menu.png");
   // Fonte usada no Menu
   ALLEGRO_FONT* font = al_load_ttf_font("./assets/font/upheaval.ttf", 14, 0);
   // Música do Menu
   al_reserve_samples(1);
   ALLEGRO_SAMPLE* menu_music = al_load_sample("./assets/music/menu.mp3");
   ALLEGRO_SAMPLE_ID menu_music_id;
   al_play_sample(menu_music, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &menu_music_id);

   unsigned short transp = 0;
   while (TRUE)
   {
      al_wait_for_event(event_queue, &event);

      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
         al_destroy_bitmap(logo_menu);
         al_destroy_bitmap(icon_display);
         al_destroy_font(font);
         al_destroy_display(display);
         al_destroy_timer(timer);
         al_destroy_event_queue(event_queue);
         return 0;
      }

      if (event.keyboard.keycode == ALLEGRO_KEY_C) break;

      al_clear_to_color(al_map_rgb(0, 0, 0));
      al_draw_scaled_bitmap(logo_menu, 0, 0, 1000, 560, 400 - (1000 / 2) / 2, 50, (1000 / 2), (560 / 2), 0);

      if (transp < 255) transp += 4;
      if (transp > 255) transp = 255;
      al_draw_tinted_scaled_bitmap(logo_menu, al_map_rgb(transp, transp, transp), 0, 0, 1000, 560, 400 - (1000 / 2) / 2, 
                                   50, (1000 / 2), (560 / 2), 0);
      //al_draw_text(font, al_map_rgb(255, 255, 255), 600, 400, 0, "Novo Jogo");
      al_flip_display();
   }

   al_stop_sample(&menu_music_id); // Interrompe a música do Menu

   /* --------------------------------------------------------------------------
   *  CONFIGURAÇÃO DA LUTA
   ---------------------------------------------------------------------------*/
   ALLEGRO_BITMAP* personagem = al_load_bitmap("./assets/characters/ryu/idle.png");
   ALLEGRO_BITMAP* fundo = al_load_bitmap("./assets/background/one_piece.jpg");
   int pos_x = 100, pos_y = 200;
   int frame = 0;
   while (TRUE)
   {
      al_wait_for_event(event_queue, &event);

      // Clicar no X -> Fecha o jogo
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;

      // Tecla ESC -> Pausa o jogo
      if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
         do
            al_wait_for_event(event_queue, &event);
         while (event.keyboard.keycode != ALLEGRO_KEY_ESCAPE && event.type != ALLEGRO_EVENT_DISPLAY_CLOSE);
         if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
         continue;
      }

      if (pos_x >= 0 && pos_x <= DISPLAY_WIDTH && pos_y >= 0 && pos_y <= DISPLAY_HEIGHT){
         if (event.keyboard.keycode == ALLEGRO_KEY_A)
            pos_x -= 5;
         if (event.keyboard.keycode == ALLEGRO_KEY_D)
            pos_x += 5;
         if (event.keyboard.keycode == ALLEGRO_KEY_W)
            pos_y -= 5;
         if (event.keyboard.keycode == ALLEGRO_KEY_S)
            pos_y += 5;
      } else {
         pos_x = 1;
         pos_y = 1;
      }

      frame += 1;
      if (frame > 10) frame = 1;

      al_clear_to_color(al_map_rgb(255, 255, 255));
      al_draw_bitmap_region(fundo, 960, 400, 800, 600, 0, 0, 0);
      if (pos_x <= DISPLAY_WIDTH / 2)
         //al_draw_bitmap_region(personagem, 70 * (frame / 2), 0, 70, 95, pos_x, pos_y, 0);
         al_draw_scaled_bitmap(personagem, 70 * (frame / 2), 0, 70, 95, pos_x, pos_y, 140, 190, 0);
      else
         //al_draw_bitmap_region(personagem, 70 * (frame / 2), 0, 70, 95, pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
         al_draw_scaled_bitmap(personagem, 70 * (frame / 2), 0, 70, 95, pos_x, pos_y, 140, 190, ALLEGRO_FLIP_HORIZONTAL);
      al_flip_display();
   }

   // Destruição de todos os bitmaps
   al_destroy_bitmap(icon_display);
   al_destroy_bitmap(personagem);
   al_destroy_bitmap(fundo);

   // Destruição dos elementos do jogo
   al_destroy_font(font);
   al_destroy_display(display);
   al_destroy_timer(timer);
   al_destroy_event_queue(event_queue);
}