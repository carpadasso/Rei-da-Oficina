/* King of Workshop (pt-br: O Rei da Oficina) */
/* Arquivo principal */
/* Executará os eventos e as inicializações necessárias */

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <stdbool.h>
#include <stdio.h>

#include "player.h"
#include "joystick.h"

void draw_sprite_player(Player* p)
{
   int flag = p->pos_flag;

   if (p->joystick->up)
      al_draw_scaled_bitmap(p->sprites->jump, 0, 20, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
   else if (p->joystick->left)
      al_draw_scaled_bitmap(p->sprites->walking_neg, 0, 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
   else if (p->joystick->right)
      al_draw_scaled_bitmap(p->sprites->walking_pos, 0, 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
   else if (p->joystick->down)
      al_draw_scaled_bitmap(p->sprites->crouch, 0, 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
   else if (p->joystick->button_1){
      p->w = 110;
      p->h = 95;
      al_draw_scaled_bitmap(p->sprites->attack_sup, 70, 0, 110, 95, p->x, p->y, 110*2.5, 95*2.5, flag);
   }
   else if (p->joystick->button_2)
      al_draw_scaled_bitmap(p->sprites->attack_inf, 140, 0, 90, 95, p->x, p->y, 90*2.5, 95*2.5, flag);
   else
      al_draw_scaled_bitmap(p->sprites->idle, 0, 0, 70, 95, p->x, p->y, 70*2.5, 95*2.5, flag);
}

void update_players_positions(Player *p1, Player *p2)
{
   if (p1->joystick->left)
      move_player(p1, LEFT, 1, STEP_FRONT, DISPLAY_WIDTH - p1->w, 0);
   if (p1->joystick->right)
      move_player(p1, RIGHT, 1, STEP_FRONT, DISPLAY_WIDTH - p1->w, 0);
   if (p1->joystick->up)
      move_player(p1, UP, 1, STEP_FRONT, DISPLAY_WIDTH - p1->w, 0);
   if (p1->joystick->down)
      move_player(p1, DOWN, 1, STEP_FRONT, DISPLAY_WIDTH - p1->w, 0);
   if (p2->joystick->left)
      move_player(p2, LEFT, 1, STEP_FRONT, DISPLAY_WIDTH - p1->w, 0);
   if (p2->joystick->right)
      move_player(p2, RIGHT, 1, STEP_FRONT, DISPLAY_WIDTH - p1->w, 0);
   if (p2->joystick->up)
      move_player(p2, UP, 1, STEP_FRONT, DISPLAY_WIDTH - p1->w, 0);
   if (p2->joystick->down)
      move_player(p2, DOWN, 1, STEP_FRONT, DISPLAY_WIDTH - p1->w, 0);
}

int main(int argc, char* argv[])
{
   /* Variáveis de uso geral */
   ALLEGRO_EVENT event;
   bool closeGame = false;

   /* Inits necessários */
   al_init();                  // Carregamento da biblioteca padrão Allegro
   al_init_font_addon();       // Carregamento da biblioteca de fontes
   al_init_ttf_addon();        // Carregamento da biblioteca de fontes .ttf
   al_init_image_addon();      // Carregamento da biblioteca de imagens
   al_install_keyboard();      // Carregamento da biblioteca do teclado
   al_install_audio();         // Carregamento da biblioteca de áudio
   al_init_acodec_addon();     // Carregamento da biblioteca de formatos de áudio
   al_init_primitives_addon(); // Carregamento da biblioteca de primitivos

   /* Criação dos elementos do jogo */
   ALLEGRO_TIMER* timer = al_create_timer(1.0 / REFRESH_RATE);
   ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
   ALLEGRO_DISPLAY* display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

   /* Registro de fontes de eventos para a Event Queue */
   al_register_event_source(event_queue, al_get_keyboard_event_source());
   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   
   /* Começa o Timer */
   al_start_timer(timer);
   
   /* Altera o título e ícone do display aberto */
   al_set_window_title(display, "Street Fighter II: Champion Edition");
   ALLEGRO_BITMAP* icon_display = al_load_bitmap("./assets/icon.png");
   al_set_display_icon(display, icon_display);

   /* --------------------------------------------------------------------------------------------------------------- */
   /* --------------------------------------------------------------------------
   *  MENU INICIAL
   ---------------------------------------------------------------------------*/
   /* Carrega as imagens do Menu */
   ALLEGRO_BITMAP* logo_menu = al_load_bitmap("./assets/logo.png");
   ALLEGRO_BITMAP* arrow_select = al_load_bitmap("./assets/arrow_select_menu.png");
   /* Fonte usada no Menu */
   ALLEGRO_FONT* font_menu = al_load_ttf_font("./assets/font/CooperBits.ttf", 32, 0);
   /* Música do Menu */
   al_reserve_samples(1);
   ALLEGRO_SAMPLE* menu_music = al_load_sample("./assets/music/menu.mp3");
   ALLEGRO_SAMPLE_ID menu_music_id;
   al_play_sample(menu_music, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &menu_music_id);

   unsigned short transp = 0;
   unsigned short selected_option = 0;
   while (true)
   {
      al_wait_for_event(event_queue, &event);

      /* Evento 01:
       * Fechar o Jogo */
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
         closeGame = true;
         break;
      }
      /* Evento 02:
       * Seleção da Opção do Menu */
      else if (event.type == ALLEGRO_EVENT_KEY_DOWN){
         // UP ou DOWN -> Muda de Opção
         if (event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_UP){
            if (selected_option == 0) selected_option = 1;
            else if (selected_option == 1) selected_option = 0;
         }
         // ENTER -> Seleciona a Opção
         else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) break;
      }
      /* Evento 03:
       * Atualização do Temporizador (Forma a estrutura gráfica do Menu Inicial) */
      else if (event.type == ALLEGRO_EVENT_TIMER){
         al_clear_to_color(al_map_rgb(0, 0, 0));                       
         al_draw_tinted_scaled_bitmap(logo_menu, al_map_rgb(255, 255, 255), 0, 0, 577, 253, 400 - (577 / 2), 50, 577, 253, 0);                                    
         al_draw_bitmap(arrow_select, 280, 360 + 50 * selected_option, 0);   
         al_draw_text(font_menu, al_map_rgb(255, 255, 255), 400, 360, ALLEGRO_ALIGN_CENTER, "Novo Jogo");
         al_draw_text(font_menu, al_map_rgb(255, 255, 255), 400, 410, ALLEGRO_ALIGN_CENTER, "Sair");
         al_flip_display();
      }
   }
   /* Destrói os elementos do Menu */
   al_stop_sample(&menu_music_id);  // Interrompe a música do Menu
   al_destroy_font(font_menu);
   al_destroy_bitmap(logo_menu);
   al_destroy_bitmap(arrow_select);
   al_destroy_sample(menu_music);

   /* Encerramento do Jogo */
   if (closeGame || selected_option == 1){
      al_destroy_timer(timer);
      al_destroy_event_queue(event_queue);
      al_destroy_display(display);
      al_destroy_bitmap(icon_display);
      return 0;
   }
   /* --------------------------------------------------------------------------------------------------------------- */

   /* --------------------------------------------------------------------------------------------------------------- */
   /* --------------------------------------------------------------------------
   *  TELA DE SELEÇÃO
   ---------------------------------------------------------------------------*/
   /* Fonte utilizada na Seleção de Personagem*/
   ALLEGRO_FONT* font_select = al_load_ttf_font("./assets/font/Fatal Fighter.ttf", 32, 0);
   /* Transformação usada para Rotacionar o Fundo */
   ALLEGRO_TRANSFORM transf;

   unsigned short character_select = 0;
   unsigned short w, h;
   float movement = 0.0;
   /* w_string: Armazena o comprimento da string Street Fighter 
    * h_string: Armazena a altura em pixels da string na fonte font_select */
   unsigned short w_string = al_get_text_width(font_select, "STREET FIGHTER");
   unsigned short h_string = al_get_font_line_height(font_select);

   while (true)
   {
      al_wait_for_event(event_queue, &event);

      /* Evento 01:
       * Encerramento do Jogo */
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
         closeGame = true;
         break;
      }
      else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_C)
         break;
      /* Evento 02:
       * Atualização do Temporizador*/
      else if (event.type == ALLEGRO_EVENT_TIMER){
         movement += 0.5;
         if (movement >= h_string) movement = 0.0;

         al_clear_to_color(al_map_rgb(34, 35, 36));

         /* --- --- Início da Rotação --- --- */
         al_identity_transform(&transf);
         al_rotate_transform(&transf, 0.5f);
         al_use_transform(&transf);

         /* Elementos Rotacionados:
          * > String "STREET FIGHTER" */
         for (h = 0; h < 2 * DISPLAY_HEIGHT; h += h_string)
            for (w = 0; w < 2 * DISPLAY_WIDTH; w += w_string + 10)
               al_draw_text(font_select, al_map_rgb(77, 78, 79), w, -(h_string + 10 + 400) + (h + 10) + (int) movement, 0, "STREET FIGHTER");
         
         al_identity_transform(&transf);
         al_use_transform(&transf);
         /* --- --- Fim da Rotação --- --- */

         al_draw_text(font_select, al_map_rgb(255, 255, 255), 400, 100, 
                        ALLEGRO_ALIGN_CENTER, "ESCOLHA SEU PERSONAGEM");
         
         al_flip_display();
      }
   }
   al_destroy_font(font_select);

   /* Encerramento do Jogo */
   if (closeGame){
      al_destroy_timer(timer);
      al_destroy_event_queue(event_queue);
      al_destroy_display(display);
      al_destroy_bitmap(icon_display);
      return 0;
   }
   /* --------------------------------------------------------------------------------------------------------------- */

   /* --------------------------------------------------------------------------------------------------------------- */
   /* --------------------------------------------------------------------------
   *  CONFIGURAÇÃO DA LUTA
   ---------------------------------------------------------------------------*/
   Player* player_01 = create_player(RYU, 20, DISPLAY_HEIGHT - 300, 70, 95);
   Player* player_02 = create_player(KEN, DISPLAY_WIDTH - 200, DISPLAY_HEIGHT - 300, 70, 95);
   ALLEGRO_BITMAP* fundo = al_load_bitmap("./assets/background/ryu_stage.png");

   unsigned short damage = 0;

   while (true)
   {
      al_wait_for_event(event_queue, &event);

      /* Atualização dos atributos dos Jogadores */
      update_players_positions(player_01, player_02);

      if (player_01->x < player_02->x){
         player_01->pos_flag = 0;
         player_02->pos_flag = ALLEGRO_FLIP_HORIZONTAL;
      }
      else {
         player_01->pos_flag = ALLEGRO_FLIP_HORIZONTAL;
         player_02->pos_flag = 0;
      }


      /* Evento 01:
       * Encerrar o Jogo */
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
      /* Evento 02:
       * Pressionar e Soltar de uma Tecla */
      else if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP){
         /* Teclas do Jogador 01:
          * W, A, S, D -> Movimentação do Jogador 01
          * Z -> Ataque com Membro Superior 
          * X -> Ataque com Membro Inferior */
         if (event.keyboard.keycode == ALLEGRO_KEY_W) joystick_up(player_01->joystick);
         else if (event.keyboard.keycode == ALLEGRO_KEY_A) joystick_left(player_01->joystick);
         else if (event.keyboard.keycode == ALLEGRO_KEY_S) joystick_down(player_01->joystick);
         else if (event.keyboard.keycode == ALLEGRO_KEY_D) joystick_right(player_01->joystick);
         else if (event.keyboard.keycode == ALLEGRO_KEY_Z) joystick_button_1(player_01->joystick);
         else if (event.keyboard.keycode == ALLEGRO_KEY_X) joystick_button_2(player_01->joystick);

         /* Teclas do Jogador 02:
          * CIMA, ESQUERDA, BAIXO, DIREITA -> Movimentação do Jogador 02
          * K -> Ataque com Membro Superior 
          * L -> Ataque com Membro Inferior */
         else if (event.keyboard.keycode == ALLEGRO_KEY_UP) joystick_up(player_02->joystick);
         else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) joystick_left(player_02->joystick);
         else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) joystick_down(player_02->joystick);
         else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) joystick_right(player_02->joystick);
         else if (event.keyboard.keycode == ALLEGRO_KEY_K) joystick_button_1(player_02->joystick);
         else if (event.keyboard.keycode == ALLEGRO_KEY_L) joystick_button_2(player_02->joystick);
      }
      /* Evento 03:
       * Atualização do Temporizador */
      else if (event.type == ALLEGRO_EVENT_TIMER){
         /* Exibição do Cenário de Fundo */
         al_draw_scaled_bitmap(fundo, 300, 0, 320, 240, 0, 0, 800, 600, 0);

         /* Exibição de informações importantes:
          * Pontuação, Contador da Luta, Barras de Vida */
         
         /* Barra de Vida - Jogador 01*/
         al_draw_filled_rectangle(30, 50, 330, 70, al_color_name("red"));
         if (player_01->hit_points >= 0) 
            al_draw_filled_rectangle(30, 50, 330 * (player_01->hit_points / DEFAULT_HIT_POINTS), 70, al_color_name("yellow"));
         al_draw_rectangle(29.5, 49.5, 330.5, 70.5, al_color_name("white"), 3);

         /* Barra de Vida - Jogador 02*/
         al_draw_filled_rectangle(470, 50, 770, 70, al_color_name("red"));
         if (player_02->hit_points >= 0)
            al_draw_filled_rectangle(470, 50, 770 * (player_02->hit_points / DEFAULT_HIT_POINTS), 70, al_color_name("yellow"));
         al_draw_rectangle(469.5, 49.5, 770.5, 70.5, al_color_name("white"), 3);

         /* Sprites dos Jogadores */
         draw_sprite_player(player_01);
         draw_sprite_player(player_02);

         al_flip_display();
      }
   }
   /* --------------------------------------------------------------------------------------------------------------- */

   /* Destruição dos Jogadores */
   destroy_player(player_01);
   destroy_player(player_02);

   /* Destruição dos elementos do jogo */
   al_destroy_display(display);
   al_destroy_timer(timer);
   al_destroy_event_queue(event_queue);
   al_destroy_bitmap(icon_display);
   al_destroy_bitmap(fundo);

   return 0;
}