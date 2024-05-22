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

// DEFINES
#define TRUE 1
#define FALSE 0

int main(int argc, char* argv[])
{
   // Variáveis de uso geral
   ALLEGRO_EVENT event;

   // Inits necessários
   al_init();                 // Carregamento da biblioteca padrão Allegro
   al_init_font_addon();      // Carregamento da biblioteca de fontes
   al_init_ttf_addon();       // Carregamento da biblioteca de fontes .ttf
   al_init_image_addon();     // Carregamento da biblioteca de imagens
   al_install_keyboard();     // Carregamento da biblioteca do teclado
   al_install_audio();        // Carregamento da biblioteca de áudio
   al_init_acodec_addon();    // Carregamento da biblioteca de formatos de áudio

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
   ALLEGRO_BITMAP* arrow_select = al_load_bitmap("./assets/arrow_select_menu.png");
   // Fonte usada no Menu
   ALLEGRO_FONT* font = al_load_ttf_font("./assets/font/CooperBits.ttf", 24, 0);
   // Música do Menu
   al_reserve_samples(1);
   ALLEGRO_SAMPLE* menu_music = al_load_sample("./assets/music/menu.mp3");
   ALLEGRO_SAMPLE_ID menu_music_id;
   al_play_sample(menu_music, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &menu_music_id);

   unsigned short transp = 0;
   unsigned short selected_option = 0;
   while (TRUE)
   {
      al_wait_for_event(event_queue, &event);

      // Clicar no X -> Fecha o jogo
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
         // Elementos Gerais
         al_destroy_timer(timer);
         al_destroy_event_queue(event_queue);
         al_destroy_display(display);
         // Elementos Menu
         al_destroy_font(font);
         al_destroy_bitmap(icon_display);
         al_destroy_sample(menu_music);
         // Imagens Menu
         al_destroy_bitmap(logo_menu);
         al_destroy_bitmap(arrow_select);
         return 0;
      }
      // Seleção da Opção
      if (event.type == ALLEGRO_EVENT_KEY_DOWN){
         // UP ou DOWN -> Muda de Opção
         if (event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_UP){
            if (selected_option == 0) selected_option = 1;
            else if (selected_option == 1) selected_option = 0;
         }
         // ENTER -> Seleciona a Opção
         else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER){
            if (selected_option == 0) break; // Avança para a Tela de Seleção
            else if (selected_option == 1){  // Fecha o jogo
               // Elementos Gerais
               al_destroy_timer(timer);
               al_destroy_event_queue(event_queue);
               al_destroy_display(display);
               al_destroy_font(font);
               // Elementos Menu
               al_destroy_bitmap(icon_display);
               al_destroy_sample(menu_music);
               // Imagens Menu
               al_destroy_bitmap(logo_menu);
               al_destroy_bitmap(arrow_select);
               return 0;
            }
         }
      }
      // CLOCK TIMER -> Exibição de Elementos Gráficos do Menu
      else if (event.type == ALLEGRO_EVENT_TIMER){
         al_clear_to_color(al_map_rgb(0, 0, 0));                                  // Fundo da Tela
         al_draw_tinted_scaled_bitmap(logo_menu, al_map_rgb(255, 255, 255), 0, 
                                      0, 1000, 560, 400 - (1000 / 2) / 2,50, 
                                      (1000 / 2), (560 / 2), 0);                  // Logo do Street Fighter
         al_draw_bitmap(arrow_select, 280, 360 + 50 * selected_option, 0);        // Seta de Escolha de Opção
         al_draw_text(font, al_map_rgb(255, 255, 255), 330, 360, 0, "Novo Jogo"); // Opção 1
         al_draw_text(font, al_map_rgb(255, 255, 255), 330, 410, 0, "Sair");      // Opção 2
         al_flip_display();
      }
   }
   al_stop_sample(&menu_music_id); // Interrompe a música do Menu
   al_destroy_bitmap(logo_menu);
   al_destroy_bitmap(arrow_select);
   al_destroy_bitmap(icon_display);
   al_destroy_sample(menu_music);

   /* --------------------------------------------------------------------------
   *  TELA DE SELEÇÃO
   ---------------------------------------------------------------------------*/
   unsigned short character_select_opt = 0;
   while (TRUE)
   {
      al_wait_for_event(event_queue, &event);

      if (event.type == ALLEGRO_EVENT_TIMER){
         al_clear_to_color(al_map_rgb(34, 35, 36));
         al_draw_text(font, al_map_rgb(255, 255, 255), 150, 100, 0, "Modo de Luta: ");
         if (character_select_opt == 0)
            al_draw_text(font, al_map_rgb(255, 255, 255), 370, 100, 0, "Ryu vs Ken");
         else if (character_select_opt == 1)
            al_draw_text(font, al_map_rgb(255, 255, 255), 370, 100, 0, "Ken vs Ryu");
         al_flip_display();
      }
      else if (event.type == ALLEGRO_EVENT_KEY_DOWN){
         if (event.keyboard.keycode == ALLEGRO_KEY_LEFT || event.keyboard.keycode == ALLEGRO_KEY_RIGHT){
            if (character_select_opt == 0) character_select_opt = 1;
            else if (character_select_opt == 1) character_select_opt = 0;
         }
         else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) break;
      }
      else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
         // Elementos Gerais
         al_destroy_timer(timer);
         al_destroy_event_queue(event_queue);
         al_destroy_display(display);
         al_destroy_font(font);
         return 0;
      }
   }

   /* --------------------------------------------------------------------------
   *  CONFIGURAÇÃO DA LUTA
   ---------------------------------------------------------------------------*/
   ALLEGRO_BITMAP* p1_idle = al_load_bitmap("./assets/characters/ryu/idle.png");
   ALLEGRO_BITMAP* p1_walk_pos = al_load_bitmap("./assets/characters/ryu/walk_pos.png");
   ALLEGRO_BITMAP* p1_walk_neg = al_load_bitmap("./assets/characters/ryu/walk_neg.png");
   ALLEGRO_BITMAP* fundo = al_load_bitmap("./assets/background/one_piece.jpg");
   unsigned short pos_x = 100, pos_y = 200;
   unsigned short idle_frame = 0, walk_frame = 0;
   unsigned short position = 0, flag_position = 0;
   int tecla;

   while (TRUE)
   {
      al_wait_for_event(event_queue, &event);

      // Clicar no X -> Fecha o jogo
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;

      // Exibe o Cenário de Fundo
      al_draw_bitmap_region(fundo, 960, 400, 800, 600, 0, 0, 0);

      // Tecla ESC -> Pausa o jogo
      if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
         do
            al_wait_for_event(event_queue, &event);
         while (event.keyboard.keycode != ALLEGRO_KEY_ESCAPE && event.type != ALLEGRO_EVENT_DISPLAY_CLOSE);
         if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
         continue;
      }

      // Calcula a posição do jogador com relação ao meio da tela
      if (pos_x - 70 < DISPLAY_WIDTH / 2) position = 0;
      else position = 1;
      // Calcula se deve exibir o sprite invertido ou não
      if (position == 0) flag_position = 0;
      else if (position == 1) flag_position = ALLEGRO_FLIP_HORIZONTAL;
      
      /* Aperto de Tecla */
      // D -> MOVIMENTA para a DIREITA
      // A -> MOVIMENTA para a ESQUERDA
      // W -> PULA
      // S -> AGACHA
      // Z -> ATAQUE com MEMBROS SUPERIORES
      // X -> ATAQUE com MEMBROS INFERIORES
      if (event.type == ALLEGRO_EVENT_KEY_DOWN){
         tecla = event.keyboard.keycode;
         if (tecla == ALLEGRO_KEY_D){
            pos_x += 5;
            //walk_frame += 1;
            //if (walk_frame > 24) walk_frame = 1;
            //al_draw_scaled_bitmap(p1_walk_pos, 70 * (walk_frame / 6), 0, 70, 95, pos_x, pos_y, 140, 190, flag_position);
         }
         if (tecla == ALLEGRO_KEY_A){
            pos_x -= 3;
            //walk_frame += 1;
            //if (walk_frame > 24) walk_frame = 1;
            //al_draw_scaled_bitmap(p1_walk_neg, 70 * (walk_frame / 6), 0, 70, 95, pos_x, pos_y, 140, 190, flag_position);
         }
         if (tecla == ALLEGRO_KEY_W) pos_y -= 5;
         if (tecla == ALLEGRO_KEY_S) pos_y += 5;
      }
      /* Soltura de Tecla */
      // A ou D -> CANCELA a animação de movimento
      if (event.type == ALLEGRO_EVENT_KEY_UP){
         tecla = event.keyboard.keycode;
         if (tecla == ALLEGRO_KEY_D) { walk_frame = 0; idle_frame = 0; }
         else if (tecla == ALLEGRO_KEY_A) { walk_frame = 0; idle_frame = 0; }
      }
      /* Batida de Relógio */
      // EXECUTA a animação de PARADO do personagem
      if (event.type == ALLEGRO_EVENT_TIMER){
         idle_frame += 1;
         if (idle_frame > 20) idle_frame = 1;
         al_draw_scaled_bitmap(p1_idle, 70 * (idle_frame / 5), 0, 70, 95, pos_x, pos_y, 140, 190, flag_position);
      }
      al_flip_display();
   }

   // Destruição de todos os bitmaps
   al_destroy_bitmap(icon_display);
   al_destroy_bitmap(p1_idle);
   al_destroy_bitmap(p1_walk_pos);
   al_destroy_bitmap(p1_walk_neg);
   al_destroy_bitmap(fundo);

   // Destruição dos elementos do jogo
   al_destroy_font(font);
   al_destroy_display(display);
   al_destroy_timer(timer);
   al_destroy_event_queue(event_queue);
}