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
#include <stdlib.h>
#include <math.h>

#include "enums.h"
#include "joystick.h"
#include "stage.h"
#include "player.h"

/* --------------------------------
 * Funções Secundárias / Auxiliares
 * -------------------------------- */
void inverte_string(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char* citoa(int num, char* str, int base)
{
    int i = 0;
    bool is_negative = false;
 
    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10) {
        is_negative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
 
    // If number is negative, append '-'
    if (is_negative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    inverte_string(str, i);
 
    return str;
}

void destroy_game_elements(ALLEGRO_TIMER *t, ALLEGRO_EVENT_QUEUE *ev_q, 
                              ALLEGRO_DISPLAY *d, ALLEGRO_BITMAP *icon)
{
   al_destroy_timer(t);
   al_destroy_event_queue(ev_q);
   al_destroy_display(d);
   al_destroy_bitmap(icon);
}

void restart_round(Player* p1, Player* p2, float* clk_timer)
{
   /* ----------------------------
    * Reinicialização de Movimento
    * ---------------------------- */
   p1->x = p1->x_hit = p1->x_hurt = 20;                  
   p1->y = p1->y_hit = p1->y_hurt = Y_MAX;
   p1->w = p1->w_hit = p1->w_hurt = 70;
   p1->h = p1->h_hit = p1->h_hurt = 95;

   p2->x = p2->x_hit = p2->x_hurt = DISPLAY_WIDTH - 200;                  
   p2->y = p2->y_hit = p2->y_hurt = Y_MAX;
   p2->w = p2->w_hit = p2->w_hurt = 70;
   p2->h = p2->h_hit = p2->h_hurt = 95;

   p1->move = IDLE;
   p2->move = IDLE;

   /* --------------------------
    * Reinicialização das Flags
    * -------------------------- */
   p1->sprites->frame = p2->sprites->frame = 0.0;
   p1->is_jumping = p1->is_falling = false;
   p1->enable_atk_p = p1->enable_atk_k = p1->enable_jump = 0;

   p2->is_jumping = p2->is_falling = false;
   p2->enable_atk_p = p2->enable_atk_k = p2->enable_jump = 0;

   /* --------------------------
    * Reinicialização dos PONTOS
    * -------------------------- */
   p1->hit_points   = p2->hit_points   = DEFAULT_HIT_POINTS;
   p1->power_points = p2->power_points = DEFAULT_POW_POINTS;

   /* --------------------------
    * Reinicialização do Relógio
    * -------------------------- */
   *clk_timer = al_get_time();
}

/* ---------------------------
 * Funções de Execução de Fase 
 * --------------------------- */
int execMainMenu(ALLEGRO_EVENT_QUEUE *event_queue)
{
   ALLEGRO_EVENT event;
   bool closeGame = false;

   /* Carrega as imagens do Menu */
   ALLEGRO_BITMAP* logo_menu = al_load_bitmap("./assets/main_menu/logo.png");
   ALLEGRO_BITMAP* arrow = al_load_bitmap("./assets/main_menu/menu_arrow.png");
   ALLEGRO_BITMAP* menu_bg = al_load_bitmap("./assets/main_menu/main_menu_bg.jpg");

   /* Fonte usada no Menu */
   ALLEGRO_FONT* font_menu = al_load_ttf_font("./assets/font/Break Brush.ttf", 48, 0);

   /* Música do Menu */
   al_reserve_samples(1);
   ALLEGRO_SAMPLE* menu_music = al_load_sample("./assets/music/main_menu.wav");
   ALLEGRO_SAMPLE_ID menu_music_id;
   al_play_sample(menu_music, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &menu_music_id);

   unsigned short option = 0;
   float frame = 0.0;

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
         /* UP ou DOWN -> Muda de Opção */
         if (event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_UP){
            option = option ^ 1;
            frame = 0.0;
         }
         /* ENTER -> Seleciona a Opção */
         else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) break;
      }
      /* Evento 03:
       * Atualização do Temporizador (Forma a estrutura gráfica do Menu Inicial) */
      else if (event.type == ALLEGRO_EVENT_TIMER){
         al_draw_tinted_scaled_bitmap(menu_bg, al_map_rgb(50, 50, 50), 600, 150, 800, 600, 0, 0, 800, 600, 0);         
         al_draw_scaled_bitmap(logo_menu, 0, 0, 593, 232, 400 - (593 / 2), 150, 593, 232, 0);

         if (frame < 7) frame += 0.35;
         al_draw_scaled_bitmap(arrow, 0, 116*floor(frame), 234, 116, 210 + 40*option, 400 + 60*option, 80, 40, 0);

         al_draw_text(font_menu, al_map_rgb(0, 0, 0), 402, 402, ALLEGRO_ALIGN_CENTER, "Novo Jogo");
         al_draw_text(font_menu, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTER, "Novo Jogo");

         al_draw_text(font_menu, al_map_rgb(0, 0, 0), 402, 462, ALLEGRO_ALIGN_CENTER, "Sair");
         al_draw_text(font_menu, al_map_rgb(255, 255, 255), 400, 460, ALLEGRO_ALIGN_CENTER, "Sair");

         al_flip_display();
      }
   }
   /* Destrói os elementos do Menu */
   al_stop_sample(&menu_music_id);  // Interrompe a música do Menu
   al_destroy_font(font_menu);
   al_destroy_bitmap(logo_menu);
   al_destroy_bitmap(arrow);
   al_destroy_sample(menu_music);

   if (closeGame || option == 1) return 1;
   return 0;
}

int execSelectScreen(ALLEGRO_EVENT_QUEUE *event_queue, Character *char_p1, Character *char_p2, Stage** stage)
{
   /* Variável de captura de evento
    * da fila de eventos */
   ALLEGRO_EVENT ev;

   bool close_game = false;

   Player* p1 = create_player(RYU,  90, 250, 70, 95);
   Player* p2 = create_player(KEN, 540, 250, 70, 95);

   Stage* amazon_preview       = create_stage(AMAZON);
   Stage* construction_preview = create_stage(CONSTRUCTION);
   Stage* factory_preview      = create_stage(FACTORY);
   Stage* market_preview       = create_stage(MARKET);
   Stage* stage_preview        = amazon_preview;

   /* Fonte utilizada na Seleção de Personagem */
   ALLEGRO_FONT* bg_select_32 = al_load_ttf_font("./assets/font/Fatal Fighter.ttf", 32, 0);
   ALLEGRO_FONT* font_select_48 = al_load_ttf_font("./assets/font/Break Brush.ttf", 48, 0);
   ALLEGRO_FONT* font_select_32 = al_load_ttf_font("./assets/font/Break Brush.ttf", 32, 0);

   /* Cores utilizadas para o texto */
   ALLEGRO_COLOR branco  = al_map_rgb(255, 255, 255);
   ALLEGRO_COLOR preto   = al_map_rgb(0, 0, 0);
   ALLEGRO_COLOR amarelo = al_map_rgb(255, 234, 8);
   ALLEGRO_COLOR cor;

   /* Música da Tela de Seleção */
   al_reserve_samples(1);
   ALLEGRO_SAMPLE* sel_music = al_load_sample("./assets/music/select_menu.wav");
   ALLEGRO_SAMPLE_ID sel_music_id;
   al_play_sample(sel_music, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &sel_music_id);

   /* Transformação usada para Rotacionar o Fundo */
   ALLEGRO_TRANSFORM transf;

   /* w_string: Armazena o comprimento da string Street Fighter 
    * h_string: Armazena a altura em pixels da string na fonte bg_select */
   unsigned short w, h;
   float movement = 0.0;
   unsigned short w_string = al_get_text_width(bg_select_32, "STREET FIGHTER");
   unsigned short h_string = al_get_font_line_height(bg_select_32);

   /* gamemode: Determina o modo de luta entre os personagens
    * gamemode == 0: Ryu vs. Ken
    * gamemode == 1: Ken vs. Ryu */
   unsigned short gamemode = 0;

   /* option: Determina a opção da Tela de Seleção
    * sel_stage: Determina o Estágio selecionado */
   unsigned short option = 0;
   unsigned short sel_stage = 0;

   float frame = 0.0;

   *stage = stage_preview;

   while (true)
   {
      al_wait_for_event(event_queue, &ev);

      /* Evento 01:
       * Encerramento do Jogo */
      if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
         close_game = true;
         break;
      }
      else if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
         if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT || ev.keyboard.keycode == ALLEGRO_KEY_RIGHT){
            if (option == 0){
               frame = 0;
               gamemode = gamemode ^ 1;
            }
            else if (option == 1){
               if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT){
                  if      (sel_stage == 0){
                     sel_stage = 3;
                     *stage = stage_preview = market_preview;
                  }
                  else if (sel_stage == 1){
                     sel_stage = 0;
                     *stage = stage_preview = amazon_preview;
                  }
                  else if (sel_stage == 2){
                     sel_stage = 1;
                     *stage = stage_preview = construction_preview;
                  }
                  else if (sel_stage == 3){
                     sel_stage = 2;
                     *stage = stage_preview = factory_preview;
                  }
               }
               if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT){
                  if      (sel_stage == 0){
                     sel_stage = 1;
                     *stage = stage_preview = construction_preview;
                  }
                  else if (sel_stage == 1){
                     sel_stage = 2;
                     *stage = stage_preview = factory_preview;
                  }
                  else if (sel_stage == 2){
                     sel_stage = 3;
                     *stage = stage_preview = market_preview;
                  }
                  else if (sel_stage == 3){
                     sel_stage = 0;
                     *stage = stage_preview = amazon_preview;
                  }
               }
            }
         }
         else if (ev.keyboard.keycode == ALLEGRO_KEY_UP || ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
            option = option ^ 1;
         }
         else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
            if      (gamemode == 0){ *char_p1 = RYU; *char_p2 = KEN; }
            else if (gamemode == 1){ *char_p1 = KEN; *char_p2 = RYU; }
            break;
         }
      }
      /* Evento 02:
       * Atualização do Temporizador */
      else if (ev.type == ALLEGRO_EVENT_TIMER){
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
               al_draw_text(bg_select_32, al_map_rgb(77, 78, 79), w, 
                              (-(h_string + 10 + 400) + (h + 10)
                              + (int) movement), 0, "STREET FIGHTER");
         
         al_identity_transform(&transf);
         al_use_transform(&transf);
         /* --- --- Fim da Rotação --- --- */

         /* Estágio de Fundo selecionado */
         draw_stage(stage_preview, 80, 60, 640, 480);
         al_draw_rectangle(80, 60, 800-80, 600-60, al_map_rgb(23, 24, 25), 5);

         /* ------------------
          * Texto de Escolha
          * ------------------ */
         al_draw_text(font_select_48, al_color_name("black"), 402.0, 12.0, ALLEGRO_ALIGN_CENTER, "ESCOLHA SEU LUTADOR");
         al_draw_text(font_select_48, al_color_name("white"), 400.0, 10.0, ALLEGRO_ALIGN_CENTER, "ESCOLHA SEU LUTADOR");

         al_draw_text(font_select_32, al_color_name("black"), 92.0, 502.0, 0, "JOGADOR 01");
         al_draw_text(font_select_32, al_color_name("white"), 90.0, 500.0, 0, "JOGADOR 01");

         al_draw_text(font_select_32, al_color_name("black"), 582.0, 502.0, 0, "JOGADOR 02");
         al_draw_text(font_select_32, al_color_name("white"), 580.0, 500.0, 0, "JOGADOR 02");

         al_draw_text(font_select_32, al_color_name("black"),      402.0, 352.0, ALLEGRO_ALIGN_CENTER, "Modo de Jogo");
         al_draw_text(font_select_32, al_color_name("peachpuff"),  400.0, 350.0, ALLEGRO_ALIGN_CENTER, "Modo de Jogo");

         al_draw_text(font_select_32, al_color_name("black"),     402.0, 422.0, ALLEGRO_ALIGN_CENTER, "Estagio de Luta");
         al_draw_text(font_select_32, al_color_name("peachpuff"), 400.0, 420.0, ALLEGRO_ALIGN_CENTER, "Estagio de Luta");
      
         if (gamemode == 0){
            if (option == 0) cor = amarelo;
            else cor = branco;
            al_draw_text(font_select_32, preto, 402.0, 382.0, ALLEGRO_ALIGN_CENTER, "< RYU vs. KEN >");
            al_draw_text(font_select_32, cor,   400.0, 380.0, ALLEGRO_ALIGN_CENTER, "< RYU vs. KEN >");
         } 
         else if (gamemode == 1){
            if (option == 0) cor = amarelo;
            else cor = branco;
            al_draw_text(font_select_32, preto, 402.0, 382.0, ALLEGRO_ALIGN_CENTER, "< KEN vs. RYU >");
            al_draw_text(font_select_32, cor,   400.0, 380.0, ALLEGRO_ALIGN_CENTER, "< KEN vs. RYU >");
         }

         if      (sel_stage == 0){
            if (option == 1) cor = amarelo;
            else cor = branco;
            al_draw_text(font_select_32, preto, 402.0, 452.0, ALLEGRO_ALIGN_CENTER, "< FLORESTA AMAZONICA >");
            al_draw_text(font_select_32, cor,   400.0, 450.0, ALLEGRO_ALIGN_CENTER, "< FLORESTA AMAZONICA >");
         } 
         else if (sel_stage == 1){
            if (option == 1) cor = amarelo;
            else cor = branco;
            al_draw_text(font_select_32, preto, 402.0, 452.0, ALLEGRO_ALIGN_CENTER, "< CANTEIRO DE OBRAS >");
            al_draw_text(font_select_32, cor,   400.0, 450.0, ALLEGRO_ALIGN_CENTER, "< CANTEIRO DE OBRAS >");
         }
         else if (sel_stage == 2){
            if (option == 1) cor = amarelo;
            else cor = branco;
            al_draw_text(font_select_32, preto, 402.0, 452.0, ALLEGRO_ALIGN_CENTER, "< USINA EM CHAMAS >");
            al_draw_text(font_select_32, cor,   400.0, 450.0, ALLEGRO_ALIGN_CENTER, "< USINA EM CHAMAS >");
         }
         else if (sel_stage == 3){
            if (option == 1) cor = amarelo;
            else cor = branco;
            al_draw_text(font_select_32, preto, 402.0, 452.0, ALLEGRO_ALIGN_CENTER, "< MERCADINHO >");
            al_draw_text(font_select_32, cor,   400.0, 450.0, ALLEGRO_ALIGN_CENTER, "< MERCADINHO >");
         }

         /* -------------------------------------------
          * Animação dos Personagens na Tela de Seleção
          * ------------------------------------------- */
         frame += 0.15;
         if (frame >= 5) frame = 0;

         if (gamemode == 0){
            al_draw_scaled_bitmap(p1->sprites->idle, 70*floor(frame), 0.0, 70.0, 95.0, p1->x, p1->y, 70*2.5, 95*2.5, 0);
            al_draw_scaled_bitmap(p2->sprites->idle, 70*floor(frame), 0.0, 70.0, 95.0, p2->x, p2->y, 70*2.5, 95*2.5, 1);
         } else {
            al_draw_scaled_bitmap(p2->sprites->idle, 70*floor(frame), 0.0, 70.0, 95.0, p1->x, p1->y, 70*2.5, 95*2.5, 0);
            al_draw_scaled_bitmap(p1->sprites->idle, 70*floor(frame), 0.0, 70.0, 95.0, p2->x, p2->y, 70*2.5, 95*2.5, 1);
         }

         al_flip_display();
      }
   }
   /* Destruição dos Elementos da Tela de Seleção */
   destroy_player(p1);
   destroy_player(p2);

   if (*stage != amazon_preview)       destroy_stage(amazon_preview);
   if (*stage != construction_preview) destroy_stage(construction_preview);
   if (*stage != factory_preview)      destroy_stage(factory_preview);
   if (*stage != market_preview)       destroy_stage(market_preview);

   al_destroy_font(bg_select_32);
   al_destroy_font(font_select_48);
   al_destroy_font(font_select_32);

   al_stop_sample(&sel_music_id); 
   al_destroy_sample(sel_music);

   return close_game;
}

int execFight(ALLEGRO_EVENT_QUEUE *ev_queue, Character char_p1, Character char_p2, Stage* stage)
{
   /* Variável de captura de evento
    * da fila de eventos */
   ALLEGRO_EVENT ev;

   /* Criação dos Jogadores */
   Player* p1 = create_player(char_p1, 20, Y_MAX, 70, 95);
   Player* p2 = create_player(char_p2, DISPLAY_WIDTH - 200, Y_MAX, 70, 95);
   ALLEGRO_BITMAP* shadow = al_load_bitmap("./assets/characters/shadow.png");

   /* Fontes usadas na HUD da luta */
   ALLEGRO_FONT* fight_font_60 = al_load_ttf_font("./assets/font/Break Brush.ttf", 60, 0);
   ALLEGRO_FONT* fight_font_48 = al_load_ttf_font("./assets/font/Break Brush.ttf", 48, 0);
   ALLEGRO_FONT* fight_font_32 = al_load_ttf_font("./assets/font/Break Brush.ttf", 32, 0);

   /* Contadores de Controle:
    * rounds_p1, rounds_p2 - Controla quantos rounds cada jogador venceu */
   unsigned short rounds_p1 = 0, rounds_p2 = 0;

   /* Flags de Controle:
    * close_game - Controla se o jogo deve ser encerrado 
    * finished_game - Controla se o fim de jogo aconteceu */
   bool close_game = false;
   bool finished_game = false;

   /* Variáveis de Controle do CONTADOR:
    * clk_timer: Captura o 'tempo inicial', isto é, t0 = 0 
    * seconds: Controla quantos segundos se passaram no rounds 
    * clk_string: Buffer para impressão do tempo na tela */
   float clk_timer, seconds;
   char clk_string[3];

   /* Variáveis de uso do Menu de Pause */
   unsigned short pause_opt = 0;
   ALLEGRO_BITMAP* transparency = al_load_bitmap("./assets/screen.png");

   /* Variáveis de uso do Menu de Fim de Jogo */
   unsigned short end_opt = 0;

   /* ---------------
    * Loop da Partida
    * --------------- */
   while (true){
      /* Reinício do Round - Precisamos resetar as variáveis
       * de controle de frames, a coordenada de corte do cenário e
       * os elementos da luta: jogadores e contador */
      stage->stage_x = al_get_bitmap_width(stage->stage_sprite) / 2;
      if (!finished_game) restart_round(p1, p2, &clk_timer);

      /* Execução do Round - Aqui acontece o tratamento de inputs 
       * dos controles, impressões na tela e cálculos de ataque,
       * movimentação e tempo */
      while (true)
      {
         al_wait_for_event(ev_queue, &ev);

         /* Verificando se algum Jogador
          * venceu o round atual */
         if (p1->hit_points <= 0) { rounds_p2++; break; }
         if (p2->hit_points <= 0) { rounds_p1++; break; }

         /* Fim da Luta - Verifica se algum dos jogadores venceu 2 rounds
         * ou também se o jogo quer ser fechado, tornando a flag de fim de jogo verdadeira */
         if (rounds_p1 == 2 || rounds_p2 == 2) finished_game = true;
         else finished_game = false;

         /* Atualiza a
          * Flag de Posição */
         update_player_pos_flags(p1, p2);

         /* ---------------------
          * Tratamento de Eventos
          * --------------------- */
         /* Evento 01: Encerrar o Programa
          * Fecha o jogo, simples assim. */
         if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            close_game = true;
            break;
         }
         
         /* Evento 02: Fim de Jogo
          * Gerencia as opções do Menu de Fim de Jogo. */
         else if (finished_game && ev.type == ALLEGRO_EVENT_KEY_DOWN){
            if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN || ev.keyboard.keycode == ALLEGRO_KEY_S){
               if      (end_opt == 0) end_opt = 1;
               else if (end_opt == 1) end_opt = 0;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_UP || ev.keyboard.keycode == ALLEGRO_KEY_W){
               if      (end_opt == 0) end_opt = 1;
               else if (end_opt == 1) end_opt = 0;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
               if (end_opt == 0){
                  rounds_p1 = rounds_p2 = 0;
                  finished_game = false;

                  /* --------------------------
                  * Reinicialização do JOYSTICK
                  * -------------------------- */
                  p1->joystick->left = p1->joystick->right = false;
                  p1->joystick->up   = p1->joystick->down  = false;
                  p1->joystick->button_1 = p1->joystick->button_2 = p1->joystick->button_3 = false;

                  p2->joystick->left = p2->joystick->right = false;
                  p2->joystick->up   = p2->joystick->down  = false;
                  p2->joystick->button_1 = p2->joystick->button_2 = p2->joystick->button_3 = false;

                  restart_round(p1, p2, &clk_timer);
               }
               else if (end_opt == 1){
                  destroy_player(p1);
                  destroy_player(p2);
                  
                  destroy_stage(stage);

                  al_destroy_font(fight_font_60);
                  al_destroy_font(fight_font_48);
                  al_destroy_font(fight_font_32);

                  al_destroy_bitmap(shadow);
                  al_destroy_bitmap(transparency);

                  return 0;
               }
            }
         }
         
         /* Evento 03: Menu de Pause 
          * Realiza o congelamento da luta, dando opção aos jogadores de
          * Retomar a Jogatina após uma pausa, Reiniciar a Luta caso algum dos
          * jogadores ficou bravo com um dos rounds, ou Voltar ao Menu Inicial
          * caso queiram trocar de personagens */
         else if (!finished_game && ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
            do 
            {
               al_wait_for_event(ev_queue, &ev);

               /* Fechar o Programa enquanto está pausado */
               if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){ close_game = true; break; }

               if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                  /* Alterar entre as opções do Menu de Pause */
                  if (ev.keyboard.keycode == ALLEGRO_KEY_UP || ev.keyboard.keycode == ALLEGRO_KEY_W){
                     if (pause_opt == 0)      pause_opt = 2;
                     else if (pause_opt == 1) pause_opt = 0;
                     else if (pause_opt == 2) pause_opt = 1;
                  }
                  if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN || ev.keyboard.keycode == ALLEGRO_KEY_S){
                     if (pause_opt == 0)      pause_opt = 1;
                     else if (pause_opt == 1) pause_opt = 2;
                     else if (pause_opt == 2) pause_opt = 0;
                  }
                  /* Selecionando a opção do Menu de Pause */
                  if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                     /* Retomar o Jogo */
                     if (pause_opt == 0){
                        break;
                     }
                     /* Reiniciar a Luta */
                     if (pause_opt == 1){
                        rounds_p1 = rounds_p2 = 0;
                        stage->stage_x = al_get_bitmap_width(stage->stage_sprite) / 2;
                        restart_round(p1, p2, &clk_timer);
                        break;
                     }
                     /* Voltar ao Menu Principal
                      * LEMBRETE: Dar FREE em tudo da Luta! */
                     if (pause_opt == 2){
                        destroy_player(p1);
                        destroy_player(p2);

                        destroy_stage(stage);

                        al_destroy_font(fight_font_60);
                        al_destroy_font(fight_font_48);
                        al_destroy_font(fight_font_32);

                        al_destroy_bitmap(shadow);
                        al_destroy_bitmap(transparency);

                        return 0;
                     }
                  }
               }
               /* -----------------------------------
                * Elementos Gráficos do Menu de Pause
                * ----------------------------------- */
               al_draw_tinted_bitmap(transparency, al_map_rgba(255, 255, 255, 2.0), 0, 0, 0);

               al_draw_text(fight_font_60, al_color_name("black"), 402.0, 152.0, ALLEGRO_ALIGN_CENTER, "Jogo Pausado");
               al_draw_text(fight_font_60, al_color_name("white"), 400.0, 150.0, ALLEGRO_ALIGN_CENTER, "Jogo Pausado");

               al_draw_text(fight_font_32, al_color_name("black"), 402.0, 272.0, ALLEGRO_ALIGN_CENTER, "Retomar Jogo");
               if (pause_opt == 0) al_draw_text(fight_font_32, al_color_name("yellow"), 400.0, 270.0, ALLEGRO_ALIGN_CENTER, "Retomar Jogo");
               else                al_draw_text(fight_font_32, al_color_name("white"), 400.0, 270.0, ALLEGRO_ALIGN_CENTER, "Retomar Jogo");

               al_draw_text(fight_font_32, al_color_name("black"), 402.0, 322.0, ALLEGRO_ALIGN_CENTER, "Reiniciar a Luta");
               if (pause_opt == 1) al_draw_text(fight_font_32, al_color_name("yellow"), 400.0, 320.0, ALLEGRO_ALIGN_CENTER, "Reiniciar a Luta");
               else                al_draw_text(fight_font_32, al_color_name("white"), 400.0, 320.0, ALLEGRO_ALIGN_CENTER, "Reiniciar a Luta");

               al_draw_text(fight_font_32, al_color_name("black"), 402.0, 372.0, ALLEGRO_ALIGN_CENTER, "Voltar ao Menu Inicial");
               if (pause_opt == 2) al_draw_text(fight_font_32, al_color_name("yellow"), 400.0, 370.0, ALLEGRO_ALIGN_CENTER, "Voltar ao Menu Inicial");
               else                al_draw_text(fight_font_32, al_color_name("white"), 400.0, 370.0, ALLEGRO_ALIGN_CENTER, "Voltar ao Menu Inicial");
               
               al_flip_display();
            } while (ev.type != ALLEGRO_EVENT_KEY_DOWN || ev.keyboard.keycode != ALLEGRO_KEY_ESCAPE);

            if (close_game) break;
         }
         
         /* Evento 04: Inputs do Controle 
          * Trata sobre o pressionar-soltar de uma tecla.
          * Essa parte faz com que se registre APENAS se uma tecla está ativa
          * ou não, ou seja, não contabiliza quantos segundos uma tecla está pressionada!! */
         else if (!finished_game && (ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_KEY_UP)){
            /* ------------------
             * Botões do Controle
             * ------------------ */
            /* Teclas do Jogador 01:
            * W, A, S, D -> Movimentação do Jogador 01
            *          Z -> Ataque com Membro Superior 
            *          X -> Ataque com Membro Inferior */
            if      (ev.keyboard.keycode == ALLEGRO_KEY_W) joystick_up(p1->joystick);
            else if (ev.keyboard.keycode == ALLEGRO_KEY_A) joystick_left(p1->joystick);
            else if (ev.keyboard.keycode == ALLEGRO_KEY_S) joystick_down(p1->joystick);
            else if (ev.keyboard.keycode == ALLEGRO_KEY_D) joystick_right(p1->joystick);
            else if (ev.keyboard.keycode == ALLEGRO_KEY_Z) joystick_button_1(p1->joystick);
            else if (ev.keyboard.keycode == ALLEGRO_KEY_X) joystick_button_2(p1->joystick);
            else if (ev.keyboard.keycode == ALLEGRO_KEY_C) joystick_button_3(p1->joystick);

            /* Teclas do Jogador 02:
            * CIMA, ESQUERDA, BAIXO, DIREITA -> Movimentação do Jogador 02
            *                              K -> Ataque com Membro Superior 
            *                              L -> Ataque com Membro Inferior */
            if      (ev.keyboard.keycode == ALLEGRO_KEY_UP)    joystick_up(p2->joystick);
            else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT)  joystick_left(p2->joystick);
            else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN)  joystick_down(p2->joystick);
            else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) joystick_right(p2->joystick);
            else if (ev.keyboard.keycode == ALLEGRO_KEY_K)     joystick_button_1(p2->joystick);
            else if (ev.keyboard.keycode == ALLEGRO_KEY_L)     joystick_button_2(p2->joystick);
            else if (ev.keyboard.keycode == ALLEGRO_KEY_M)     joystick_button_3(p2->joystick);

            /* ------------------------
             * Reset de Flags de Enable
             * ------------------------ */
            if (ev.type == ALLEGRO_EVENT_KEY_UP){
               /* Flags de Enable do Jogador 01 */
               if      (ev.keyboard.keycode == ALLEGRO_KEY_W) p1->enable_jump   = 0;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_Z) p1->enable_atk_p  = 0;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_X) p1->enable_atk_k  = 0;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_C) p1->enable_atk_sp = 0;

               /* Flags de Enable do Jogador 02 */
               if      (ev.keyboard.keycode == ALLEGRO_KEY_UP) p2->enable_jump   = 0;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_K)  p2->enable_atk_p  = 0;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_L)  p2->enable_atk_k  = 0;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_M)  p2->enable_atk_sp = 0;
            }
         }
         
         /* Evento 05: Atualização do Temporizador
          * Aqui trata-se, de maior parte, sobre imprimir coisas na tela,
          * como por exemplo os sprites dos jogadores, o cenário de fundo,
          * as barras de vida, etc. */
         else if (ev.type == ALLEGRO_EVENT_TIMER){
            /* ----------------
             * Cenário de Fundo
             * ---------------- */
            draw_stage(stage, 0, 0, 800, 600);

            /* ------------------
             * Nome do Personagem
             * ------------------ */
            if (p1->selected_char == RYU){
               al_draw_text(fight_font_32, al_color_name("black"), 32.0,  22.0, 0, "RYU");
               al_draw_text(fight_font_32, al_color_name("white"), 30.0,  20.0, 0, "RYU");
               al_draw_text(fight_font_32, al_color_name("black"), 722.0, 22.0, 0, "KEN");
               al_draw_text(fight_font_32, al_color_name("white"), 720.0, 20.0, 0, "KEN");
            }
            else {
               al_draw_text(fight_font_32, al_color_name("black"), 722.0, 22.0, 0, "RYU");
               al_draw_text(fight_font_32, al_color_name("white"), 720.0, 20.0, 0, "RYU");
               al_draw_text(fight_font_32, al_color_name("black"), 32.0,  22.0, 0, "KEN");
               al_draw_text(fight_font_32, al_color_name("white"), 30.0,  20.0, 0, "KEN");
            }

            /* -------------
             * Barra de Vida
             * Jogador 01
             * ------------- */
            al_draw_filled_rectangle(30, 50, 330, 70, al_color_name("black"));
            if (p1->hit_points > 0) 
               al_draw_filled_rectangle(30.0, 50.0, 30.0 + 300*(p1->hit_points / (1.0*DEFAULT_HIT_POINTS)), 70.0, al_color_name("gold"));
            al_draw_rectangle(29.5, 49.5, 330.5, 70.5, al_color_name("white"), 2.0);

            /* -------------
             * Barra de Vida
             * Jogador 02
             * ------------- */
            al_draw_filled_rectangle(470, 50, 770, 70, al_color_name("black"));
            if (p2->hit_points > 0)
               al_draw_filled_rectangle(470.0, 50.0, 470.0 + 300*(p2->hit_points / (1.0*DEFAULT_HIT_POINTS)), 70.0, al_color_name("gold"));
            al_draw_rectangle(469.5, 49.5, 770.5, 70.5, al_color_name("white"), 2.0);

            /* ----------
             * Pontuação
             * Jogador 01
             * ---------- */
            if (rounds_p1 >= 1) al_draw_filled_rectangle(319.5, 79.5, 329.5, 89.5, al_color_name("yellowgreen"));
            al_draw_rectangle(320.0, 80.0, 330.0, 90.0, al_color_name("white"), 2.0);
            if (rounds_p1 >= 2) al_draw_filled_rectangle(299.5, 79.5, 309.5, 89.5, al_color_name("yellowgreen"));
            al_draw_rectangle(300.0, 80.0, 310.0, 90.0, al_color_name("white"), 2.0);

            /* ----------
             * Pontuação
             * Jogador 02
             * ---------- */
            if (rounds_p2 >= 1) al_draw_filled_rectangle(469.5, 79.5, 479.5, 89.5, al_color_name("yellowgreen"));
            al_draw_rectangle(470.0, 80.0, 480.0, 90.0, al_color_name("white"), 2.0);
            if (rounds_p2 >= 2) al_draw_filled_rectangle(489.5, 79.5, 499.5, 89.5, al_color_name("yellowgreen"));
            al_draw_rectangle(490.0, 80.0, 500.0, 90.0, al_color_name("white"), 2.0);

            /* --------------
             * Barra de Poder
             * Jogador 01
             * -------------- */
            if (p1->power_points >= 1) al_draw_filled_rectangle(249.5, 79.5, 279.5, 89.5, al_color_name("darkturquoise"));
            al_draw_rectangle(250.0, 80.0, 280.0, 90.0, al_color_name("white"), 2.0);
            if (p1->power_points >= 2) al_draw_filled_rectangle(209.5, 79.5, 239.5, 89.5, al_color_name("darkturquoise"));
            al_draw_rectangle(210.0, 80.0, 240.0, 90.0, al_color_name("white"), 2.0);
            if (p1->power_points >= 3) al_draw_filled_rectangle(169.5, 79.5, 199.5, 89.5, al_color_name("darkturquoise"));
            al_draw_rectangle(170.0, 80.0, 200.0, 90.0, al_color_name("white"), 2.0);

            /* --------------
             * Barra de Poder
             * Jogador 02
             * -------------- */
            if (p2->power_points >= 1) al_draw_filled_rectangle(519.5, 79.5, 549.5, 89.5, al_color_name("darkturquoise"));
            al_draw_rectangle(520.0, 80.0, 550.0, 90.0, al_color_name("white"), 2.0);
            if (p2->power_points >= 2) al_draw_filled_rectangle(559.5, 79.5, 589.5, 89.5, al_color_name("darkturquoise"));
            al_draw_rectangle(560.0, 80.0, 590.0, 90.0, al_color_name("white"), 2.0);
            if (p2->power_points >= 3) al_draw_filled_rectangle(599.5, 79.5, 629.5, 89.5, al_color_name("darkturquoise"));
            al_draw_rectangle(600.0, 80.0, 630.0, 90.0, al_color_name("white"), 2.0);

            /* -----------------
             * Contador de Tempo
             * ----------------- */
            if (!finished_game){
               al_draw_text(fight_font_32, al_color_name("black"),  402.0, 32.0, ALLEGRO_ALIGN_CENTER, "Tempo");
               al_draw_text(fight_font_32, al_color_name("yellow"), 400.0, 30.0, ALLEGRO_ALIGN_CENTER, "Tempo");

               /* Calcula os segundos (de trás pra frente) do relógio */
               seconds = 60.0 - ceil(al_get_time() - clk_timer);
               citoa(seconds, clk_string, 10);

               /* Exibe o número do contador na tela */
               if (seconds >= 0){
                  al_draw_text(fight_font_60, al_color_name("black"), 402.0, 47.0, ALLEGRO_ALIGN_CENTER, clk_string);
                  /* 60s - 11s: Exibe o contador em branco
                  * 10s - 00s: Exibe o contador em vermelho (para dar drama ao jogo) */
                  if (seconds > 10) al_draw_text(fight_font_60, al_color_name("white"), 400.0, 45.0, ALLEGRO_ALIGN_CENTER, clk_string);
                  else              al_draw_text(fight_font_60, al_color_name("red"),   400.0, 45.0, ALLEGRO_ALIGN_CENTER, clk_string);
               }
               /* Tempo Esgotado!
               * Confere qual jogador possui mais vida e dá vitória do round para ele */
               if (seconds < 0){
                  if (p1->hit_points > p2->hit_points) rounds_p1++;
                  if (p2->hit_points > p1->hit_points) rounds_p2++;
                  break;
               }
            }

            /* ------------------
             * Contador de Rounds
             * ------------------ */
            if      (rounds_p1 + rounds_p2 == 0 && !finished_game){
               al_draw_text(fight_font_32, al_color_name("black"),  402.0, 97.0, ALLEGRO_ALIGN_CENTER, "Round 01");
               al_draw_text(fight_font_32,  al_color_name("palegoldenrod"),  400.0, 95.0, ALLEGRO_ALIGN_CENTER, "Round 01");

            }
            else if (rounds_p1 + rounds_p2 == 1 && !finished_game){
               al_draw_text(fight_font_32, al_color_name("black"),  402.0, 97.0, ALLEGRO_ALIGN_CENTER, "Round 02");
               al_draw_text(fight_font_32,  al_color_name("palegoldenrod"),  400.0, 95.0, ALLEGRO_ALIGN_CENTER, "Round 02");
            }
            else if (rounds_p1 + rounds_p2 == 2 && !finished_game){
               al_draw_text(fight_font_32, al_color_name("black"),  402.0, 97.0, ALLEGRO_ALIGN_CENTER, "Round 03");
               al_draw_text(fight_font_32,  al_color_name("palegoldenrod"),  400.0, 95.0, ALLEGRO_ALIGN_CENTER, "Round 03");
            }

            /* -----------
             * Fim da Luta
             * ----------- */
            if (finished_game){
               al_draw_text(fight_font_48, al_color_name("black"), 402.0, 132.0, ALLEGRO_ALIGN_CENTER, "Resultado da Partida");
               al_draw_text(fight_font_48, al_color_name("coral"), 400.0, 130.0, ALLEGRO_ALIGN_CENTER, "Resultado da Partida");

               if (rounds_p1 == 2){
                  al_draw_text(fight_font_60, al_color_name("black"), 402.0, 182.0, ALLEGRO_ALIGN_CENTER, "JOGADOR 01");
                  al_draw_text(fight_font_60, al_color_name("white"), 400.0, 180.0, ALLEGRO_ALIGN_CENTER, "JOGADOR 01");
               }
               else if (rounds_p2 == 2){
                  al_draw_text(fight_font_60, al_color_name("black"), 402.0, 182.0, ALLEGRO_ALIGN_CENTER, "JOGADOR 02");
                  al_draw_text(fight_font_60, al_color_name("white"), 400.0, 180.0, ALLEGRO_ALIGN_CENTER, "JOGADOR 02");
               }

               al_draw_text(fight_font_48, al_color_name("black"), 402.0, 302.0, ALLEGRO_ALIGN_CENTER, "Fim de Jogo");
               al_draw_text(fight_font_48, al_color_name("coral"), 400.0, 300.0, ALLEGRO_ALIGN_CENTER, "Fim de Jogo");

               al_draw_text(fight_font_32, al_color_name("black"), 402.0, 352.0, ALLEGRO_ALIGN_CENTER, "Revanche");
               if (end_opt == 0) al_draw_text(fight_font_32, al_color_name("yellow"), 400.0, 350.0, ALLEGRO_ALIGN_CENTER, "Revanche");
               else              al_draw_text(fight_font_32, al_color_name("white"),  400.0, 350.0, ALLEGRO_ALIGN_CENTER, "Revanche");

               al_draw_text(fight_font_32, al_color_name("black"), 402.0, 392.0, ALLEGRO_ALIGN_CENTER, "Voltar ao Menu Inicial");
               if (end_opt == 1) al_draw_text(fight_font_32, al_color_name("yellow"), 400.0, 390.0, ALLEGRO_ALIGN_CENTER, "Voltar ao Menu Inicial");
               else              al_draw_text(fight_font_32, al_color_name("white"),  400.0, 390.0, ALLEGRO_ALIGN_CENTER, "Voltar ao Menu Inicial");
            }

            /* ---------------------
             * Sprites dos Jogadores
             * --------------------- */

            /* Hitbox: P1 */
            al_draw_rectangle(p1->x_hit, p1->y_hit, p1->x_hit+(p1->w_hit*2.5), 
                              p1->y_hit+(p1->h_hit*2.5), al_color_name("blue"), 1.0);

            /* Hitbox: P2 */
            al_draw_rectangle(p2->x_hit-0.5, p2->y_hit-0.5, p2->x_hit+(p2->w_hit*2.5)+0.5, 
                              p2->y_hit+(p2->h_hit*2.5)+0.5, al_color_name("blue"), 1.0);

            /* Hurtbox: P1 */
            al_draw_rectangle(p1->x_hurt-0.5, p1->y_hurt-0.5, p1->x_hurt+(p1->w_hurt*2.5)+0.5, 
                              p1->y_hurt+(p1->h_hurt*2.5)+0.5, al_color_name("red"), 1.0);

            /* Hurtbox: P2 */
            al_draw_rectangle(p2->x_hurt-0.5, p2->y_hurt-0.5, p2->x_hurt+(p2->w_hurt*2.5)+0.5, 
                              p2->y_hurt+(p2->h_hurt*2.5)+0.5, al_color_name("red"), 1.0);
            
            /* Sombras */
            al_draw_scaled_bitmap(shadow, 0, 0, 82, 14, p1->x - 20, Y_MAX + 210, 2.5*82, 2.5*14, p1->pos_flag);
            al_draw_scaled_bitmap(shadow, 0, 0, 82, 14, p2->x - 20, Y_MAX + 210, 2.5*82, 2.5*14, p2->pos_flag);

            /* -------------------
             * Exibição de Sprites
             * ------------------- */
            if (!finished_game){
               draw_sprite_player(p1);
               draw_sprite_player(p2);
            }
            else {
               /* Jogador 01 VENCEU */
               if (rounds_p1 == 2){
                  if (p1->selected_char == RYU){
                     al_draw_scaled_bitmap(p1->sprites->victory, 0, 0,  70, 125, p1->x,  p1->y - 75,  2.5*70, 2.5*125, p1->pos_flag);
                     al_draw_scaled_bitmap(p2->sprites->defeat,  0, 0, 140,  95, p2->x - 175, p2->y, 2.5*140,  2.5*95, p2->pos_flag);
                  }
                  else {
                     al_draw_scaled_bitmap(p1->sprites->victory, 0, 0,  70,  95, p1->x,       p1->y,  2.5*70, 2.5*95, p1->pos_flag);
                     al_draw_scaled_bitmap(p2->sprites->defeat,  0, 0, 140,  95, p2->x - 175, p2->y, 2.5*140,  2.5*95, p2->pos_flag);
                  }
               }
               /* Jogador 02 VENCEU */
               else {
                  if (p1->selected_char == RYU){
                     al_draw_scaled_bitmap(p2->sprites->victory, 0, 0,  70,  95, p2->x, p2->y,  2.5*70,  2.5*95, p2->pos_flag);
                     al_draw_scaled_bitmap(p1->sprites->defeat,  0, 0, 140,  95, p1->x, p1->y, 2.5*140,  2.5*95, p1->pos_flag);
                  }
                  else {
                     al_draw_scaled_bitmap(p2->sprites->victory, 0, 0,  70,  125, p2->x, p2->y - 75,  2.5*70,  2.5*125, p2->pos_flag);
                     al_draw_scaled_bitmap(p1->sprites->defeat,  0, 0, 140,  95, p1->x, p1->y, 2.5*140,  2.5*95, p1->pos_flag);
                  }
               }
            }

            /* --------------------
             * Exibição de DISPAROS
             * -------------------- */
            if (p1->shot)
               al_draw_rectangle(p1->shot->x_hurt-0.5, p1->shot->y_hurt-0.5, p1->shot->x_hurt+(p1->shot->w_hurt*2.5)+0.5, 
                                 p1->shot->y_hurt+(p1->shot->h_hurt*2.5)+0.5, al_color_name("green"), 1.0);
            if (p2->shot)
               al_draw_rectangle(p2->shot->x_hurt-0.5, p2->shot->y_hurt-0.5, p2->shot->x_hurt+(p2->shot->w_hurt*2.5)+0.5, 
                                 p2->shot->y_hurt+(p2->shot->h_hurt*2.5)+0.5, al_color_name("green"), 1.0);

            show_shot(p1);
            show_shot(p2);

            al_flip_display();
         }
         
         /* -----------------------------
          * Movimento do Cenário de Fundo
          * ----------------------------- */
         /* Tenta mexer o cenário de fundo, mas o que faz de fato
          * é modificar a coordenada x do corte da imagem de fundo */
         if (p1->move == WALK_POSITIVE && p1->pos_flag == 1 && p2->x > 10){
            if (p1->x - p2->x < DISPLAY_WIDTH - 2.5*p1->w){
               stage->stage_x += 2;
               if (stage->stage_x > al_get_bitmap_width(stage->stage_sprite) - 300)
                  stage->stage_x -= 2;
               else
                  p2->x -= 5;
            }
         }
         else if (p2->move == WALK_POSITIVE && p2->pos_flag == 1 && p1->x > 10){
            if (p2->x - p1->x < DISPLAY_WIDTH - 2.5*p2->w){
               stage->stage_x += 2;
               if (stage->stage_x > al_get_bitmap_width(stage->stage_sprite) - 300)
                  stage->stage_x -= 2;
               else
                  p1->x -= 5;
            }
         }
         else if (p1->move == WALK_NEGATIVE && p1->pos_flag == 0 && p2->x < DISPLAY_WIDTH - 2.5*p2->w){
            if (p2->x - p1->x < DISPLAY_WIDTH - 2.5*p1->w){
               stage->stage_x -= 2;
               if (stage->stage_x < 0)
                  stage->stage_x += 2;
               else
                  p2->x += 5;
            }
         }
         else if (p2->move == WALK_NEGATIVE && p2->pos_flag == 0 && p1->x < DISPLAY_WIDTH - 2.5*p1->w){
            if (p1->x - p2->x < DISPLAY_WIDTH - 2.5*p2->w){
               stage->stage_x -= 2;
               if (stage->stage_x < 0)
                  stage->stage_x += 2;
               else
                  p1->x += 5;
            }
         }

         /* ----------------------------
         * Tratamento de Flags de Enable
         * ----------------------------- */
         /* Flags do Jogador 01 */
         if      (p1->joystick->up       && p1->enable_jump < 2)   p1->enable_jump   += 1;
         else if (p1->joystick->button_1 && p1->enable_atk_p < 2)  p1->enable_atk_p  += 1;
         else if (p1->joystick->button_2 && p1->enable_atk_k < 2)  p1->enable_atk_k  += 1;
         else if (p1->joystick->button_3 && p1->enable_atk_sp < 2) p1->enable_atk_sp += 1;

         /* Flags do Jogador 02 */
         if      (p2->joystick->up       && p2->enable_jump < 2)   p2->enable_jump   += 1;
         else if (p2->joystick->button_1 && p2->enable_atk_p < 2)  p2->enable_atk_p  += 1;
         else if (p2->joystick->button_2 && p2->enable_atk_k < 2)  p2->enable_atk_k  += 1;
         else if (p2->joystick->button_3 && p2->enable_atk_sp < 2) p2->enable_atk_sp += 1;

         /* ----------------------
          * Movimentação e Ataques
          * ---------------------- */

         /* Modificando a variável 'move' dos jogadores */
         update_player_movement(p1, p2);
         update_player_movement(p2, p1);
         
         /* Modifica a Flag de Pulo */
         if (p1->move == JUMP && !p1->is_jumping) p1->is_jumping = true;
         if (p2->move == JUMP && !p2->is_jumping) p2->is_jumping = true;

         /* Modifica as coordenadas dos jogadores baseado no movimento,
          * ou seja, realiza de fato o movimento dos jogadores */
         update_player_coordinates(p1, p2);
         update_player_coordinates(p2, p1);

         /* Modifica as hitboxes + hurtboxes dos jogadores baseado no
          * que eles estão fazendo, se estão pulando, agachando ou golpeando */
         update_player_boxes(p1);
         update_player_boxes(p2);

         /* Executa a matemática do ataque, baseado na colisão da hurtbox -> hitbox
          * Se a flag de enable não estiver igual a 1, não contabiliza o ataque */
         if (p1->enable_atk_p == 1 || p1->enable_atk_k == 1) execute_attack(p1, p2);
         if (p2->enable_atk_p == 1 || p2->enable_atk_k == 1) execute_attack(p2, p1);

         /* Executa o ATAQUE ESPECIAL
          * (nesse caso, Hadouken para ambos) */
         if (p1->power_points > 0 && p1->shot == NULL && p1->enable_atk_sp == 1) 
            { spawn_player_shot(p1); p1->power_points--; }
         if (p2->power_points > 0 && p2->shot == NULL && p2->enable_atk_sp == 1)
            { spawn_player_shot(p2); p2->power_points--; }

         if (verify_damage_shot(p1->shot, p2) || (p1->shot && (p1->shot->x < 0 || p1->shot->x > DISPLAY_WIDTH - p1->shot->w)))
            { despawn_player_shot(p1); p1->shot = NULL; }
         if (verify_damage_shot(p2->shot, p1) || (p2->shot && (p2->shot->x < 0 || p2->shot->x > DISPLAY_WIDTH - p2->shot->w)))
            { despawn_player_shot(p2); p2->shot = NULL; }
         
         move_shot(p1);
         move_shot(p2);
      }

      if (close_game) break;
   }

   /* Destruição da Luta */
   destroy_player(p1);
   destroy_player(p2);

   destroy_stage(stage);

   al_destroy_font(fight_font_60);
   al_destroy_font(fight_font_48);
   al_destroy_font(fight_font_32);

   al_destroy_bitmap(shadow);
   al_destroy_bitmap(transparency);

   return 1;
}

/* -------------------------------- */

int main(int argc, char* argv[])
{
   /* Variáveis de uso geral */
   Stage* stage;
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
   al_set_window_title(display, "Street Fighter II: Edição dos Campeões");
   ALLEGRO_BITMAP* icon_display = al_load_bitmap("./assets/icon.png");
   al_set_display_icon(display, icon_display);

   while (true)
   {
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
      if (execSelectScreen(event_queue, &char_p1, &char_p2, &stage) == 1){
         destroy_game_elements(timer, event_queue, display, icon_display);
         return 0;
      }

      /* ---------------------
      *  CONFIGURAÇÃO DA LUTA
      ------------------------ */
      /* Executa a fase de Luta. */
      if (execFight(event_queue, char_p1, char_p2, stage) == 1){
         destroy_game_elements(timer, event_queue, display, icon_display);
         return 0;
      }
   }
}