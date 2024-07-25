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

void destroy_game_elements(ALLEGRO_TIMER *t, ALLEGRO_EVENT_QUEUE *ev_q, 
                              ALLEGRO_DISPLAY *d, ALLEGRO_BITMAP *icon)
{
   al_destroy_timer(t);
   al_destroy_event_queue(ev_q);
   al_destroy_display(d);
   al_destroy_bitmap(icon);
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

/* -------------------------------- */
/* Funções de Execução de Fase 
 * execMainMenu: Executa a fase de Menu Principal
 * execSelecScreen: Executa a fase de Tela de Seleção
 * execFight: Executa a fase de Luta */
int execMainMenu(ALLEGRO_EVENT_QUEUE *event_queue)
{
   ALLEGRO_EVENT event;
   bool closeGame = false;

   /* Carrega as imagens do Menu */
   ALLEGRO_BITMAP* logo_menu = al_load_bitmap("./assets/main_menu/logo.png");
   ALLEGRO_BITMAP* arrow_select = al_load_bitmap("./assets/main_menu/arrow_select_menu.png");
   /* Fonte usada no Menu */
   ALLEGRO_FONT* font_menu = al_load_ttf_font("./assets/font/CooperBits.ttf", 32, 0);
   /* Música do Menu */
   al_reserve_samples(1);
   ALLEGRO_SAMPLE* menu_music = al_load_sample("./assets/music/main_menu.mp3");
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

   if (closeGame == true || selected_option == 1) return 1;
   return 0;
}

int execSelectScreen(ALLEGRO_EVENT_QUEUE *event_queue, Character *char_p1, Character *char_p2)
{
   ALLEGRO_EVENT event;
   bool closeGame = false;

   /* Fonte utilizada na Seleção de Personagem */
   ALLEGRO_FONT* font_select_32 = al_load_ttf_font("./assets/font/Fatal Fighter.ttf", 32, 0);
   ALLEGRO_FONT* font_select_24 = al_load_ttf_font("./assets/font/Fatal Fighter.ttf", 24, 0);

   /* Transformação usada para Rotacionar o Fundo */
   ALLEGRO_TRANSFORM transf;

   ALLEGRO_BITMAP* ryu_icon = al_load_bitmap("./assets/select_menu/ryu_icon.png");
   ALLEGRO_BITMAP* ken_icon = al_load_bitmap("./assets/select_menu/ken_icon.png");

   unsigned short w, h;
   float movement = 0.0;
   /* w_string: Armazena o comprimento da string Street Fighter 
    * h_string: Armazena a altura em pixels da string na fonte font_select */
   unsigned short w_string = al_get_text_width(font_select_32, "STREET FIGHTER");
   unsigned short h_string = al_get_font_line_height(font_select_32);

   /* gamemode: Determina o modo de luta entre os personagens
    * gamemode == 0: Ryu vs. Ken
    * gamemode == 1: Ken vs. Ryu */
   unsigned short gamemode = 0;

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
       * Atualização do Temporizador */
      else if (event.type == ALLEGRO_EVENT_TIMER){
         al_clear_to_color(al_map_rgb(34, 35, 36));

         /* --- --- Início da Rotação --- --- */
         al_identity_transform(&transf);
         al_rotate_transform(&transf, 0.5f);
         al_use_transform(&transf);

         /* Elementos Rotacionados:
          * > String "STREET FIGHTER" */
         movement += 0.5;
         if (movement >= h_string) movement = 0.0;
         for (h = 0; h < 2 * DISPLAY_HEIGHT; h += h_string)
            for (w = 0; w < 2 * DISPLAY_WIDTH; w += w_string + 10)
               al_draw_text(font_select_32, al_map_rgb(77, 78, 79), w, 
                              (-(h_string + 10 + 400) + (h + 10)
                              + (int) movement), 0, "STREET FIGHTER");
         
         al_identity_transform(&transf);
         al_use_transform(&transf);
         /* --- --- Fim da Rotação --- --- */

         /* Impressão de Texto */
         al_draw_text(font_select_32, al_map_rgb(224, 119, 13), 402, 102, 
                        ALLEGRO_ALIGN_CENTER, "ESCOLHA SEU PERSONAGEM");
         al_draw_text(font_select_32, al_map_rgb(255, 255, 255), 400, 100,
                        ALLEGRO_ALIGN_CENTER, "ESCOLHA SEU PERSONAGEM");
         al_draw_text(font_select_32, al_map_rgb(224, 119, 13), 52, 502, 0,
                        "JOGADOR 01");
         al_draw_text(font_select_32, al_map_rgb(255, 255, 255), 50, 500, 0,
                        "JOGADOR 01");
         al_draw_text(font_select_32, al_map_rgb(224, 119, 13), 582, 502, 0,
                        "JOGADOR 02");
         al_draw_text(font_select_32, al_map_rgb(255, 255, 255), 580, 500, 0,
                        "JOGADOR 02");
         al_draw_text(font_select_24, al_map_rgb(224, 119, 13), 582, 502,
                        ALLEGRO_ALIGN_CENTER, "Modo de Jogo");
         al_draw_text(font_select_24, al_map_rgb(255, 255, 255), 580, 500,
                        ALLEGRO_ALIGN_CENTER, "Modo de Jogo");

         /* Impressão dos Ícones */
         if (gamemode == 0){
            al_draw_bitmap(ryu_icon, 50, 300, 0);
            al_draw_bitmap(ken_icon, 600, 300, ALLEGRO_FLIP_HORIZONTAL);
         }
         else if (gamemode == 1){
            al_draw_bitmap(ryu_icon, 600, 300, ALLEGRO_FLIP_HORIZONTAL);
            al_draw_bitmap(ken_icon, 50, 300, 0);
         }
         al_flip_display();
      }
   }
   /* Destruição dos Elementos da Tela de Seleção */
   al_destroy_font(font_select_32);
   al_destroy_font(font_select_24);
   al_destroy_bitmap(ryu_icon);
   al_destroy_bitmap(ken_icon);

   if (closeGame == true) return 1;
   return 0;
}

int execFight(ALLEGRO_EVENT_QUEUE *event_queue, Character char_p1, Character char_p2)
{
   ALLEGRO_EVENT event;
   ALLEGRO_BITMAP* fundo = al_load_bitmap("./assets/background/ryu_stage.png");
   Player* player_01 = create_player(char_p1, 20, DISPLAY_HEIGHT - 300, 70, 95);
   Player* player_02 = create_player(char_p2, DISPLAY_WIDTH - 200, DISPLAY_HEIGHT - 300, 70, 95);
   unsigned short damage = 0;
   unsigned short frame_p1 = 0, frame_p2 = 0;

   while (true)
   {
      al_wait_for_event(event_queue, &event);

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
         al_draw_filled_rectangle(30, 50, 330, 70, al_color_name("black"));
         if (player_01->hit_points >= 0) 
            al_draw_filled_rectangle(30, 50, 330 * (player_01->hit_points / DEFAULT_HIT_POINTS), 70, al_color_name("yellow"));
         al_draw_rectangle(29.5, 49.5, 330.5, 70.5, al_color_name("white"), 3);

         /* Barra de Vida - Jogador 02*/
         al_draw_filled_rectangle(470, 50, 770, 70, al_color_name("black"));
         if (player_02->hit_points >= 0)
            al_draw_filled_rectangle(470, 50, 770 * (player_02->hit_points / DEFAULT_HIT_POINTS), 70, al_color_name("yellow"));
         al_draw_rectangle(469.5, 49.5, 770.5, 70.5, al_color_name("white"), 3);

         /* Sprites dos Jogadores */
         draw_sprite_player(player_01, &frame_p1);
         draw_sprite_player(player_02, &frame_p2);

         al_flip_display();
      }
      /* Atualização dos atributos dos Jogadores */
      update_players_positions(player_01, player_02);
   }
   /* Destruição dos Elementos da Luta */
   destroy_player(player_01);
   destroy_player(player_02);
   al_destroy_bitmap(fundo);
}
/* -------------------------------- */

int main(int argc, char* argv[])
{
   /* Variáveis de uso geral */
   ALLEGRO_EVENT event;
   Character char_p1, char_p2;

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

   /* -------------
   *  MENU INICIAL
   ---------------- */
   /* Executa a fase de Menu Inicial.
    * Caso o retorno da função seja 0: Prossegue para a fase de Tela de Seleção 
    * Caso o retorno da função seja 1: Encerra o jogo */
   if (execMainMenu(event_queue) == 1){
      destroy_game_elements(timer, event_queue, display, icon_display);
      return 0;
   }

   /* ----------------
   *  TELA DE SELEÇÃO
   ------------------- */
   /* Executa a fase de Tela de Seleção. 
    * Caso o retorno da função seja 0: Prossegue para a fase de Luta 
    * Caso o retorno da função seja 1: Encerra o jogo */
   if (execSelectScreen(event_queue, &char_p1, &char_p2) == 1){
      destroy_game_elements(timer, event_queue, display, icon_display);
      return 0;
   }

   /* ---------------------
   *  CONFIGURAÇÃO DA LUTA
   ------------------------ */
   /* Executa a fase de Luta. */
   //execFight(event_queue, RYU, KEN);
   destroy_game_elements(timer, event_queue, display, icon_display);
   return 0;
}