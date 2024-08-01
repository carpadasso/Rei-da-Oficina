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
#include "player.h"

/* --------------------------------
 * Funções Secundárias / Auxiliares
 * -------------------------------- */
// A utility function to reverse a string
void reverse(char str[], int length)
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

// Implementation of citoa()
char* citoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
 
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
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
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

   p1->move = p2->move = IDLE;

   /* --------------------------
    * Reinicialização das Flags
    * -------------------------- */
   p1->isAtkSup  = p1->isAtkInf  = false;
   p1->isJumping = p1->isFalling = false;
   p1->isCrouching = false;
   p1->enableAtkSup = p1->enableAtkInf = p1->enableJump = true;

   p2->isAtkSup  = p2->isAtkInf  = false;
   p2->isJumping = p2->isFalling = false;
   p2->isCrouching = false;
   p2->enableAtkSup = p2->enableAtkInf = p2->enableJump = true;

   /* ---------------------
    * Reinicialização do HP
    * --------------------- */
   p1->hit_points = p2->hit_points = DEFAULT_HIT_POINTS;

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

int execSelectScreen(ALLEGRO_EVENT_QUEUE *event_queue, Character *char_p1, Character *char_p2,
                        ALLEGRO_BITMAP** stage)
{
   ALLEGRO_EVENT event;
   bool closeGame = false;

   Player* p1 = create_player(RYU, 90, 250, 70, 95);
   Player* p2 = create_player(KEN, 540, 250, 70, 95);

   ALLEGRO_BITMAP* stage01 = al_load_bitmap("./assets/background/city_stage.jpg");
   ALLEGRO_BITMAP* stage02 = al_load_bitmap("./assets/background/ryu_stage.png");

   /* Fonte utilizada na Seleção de Personagem */
   ALLEGRO_FONT* bg_select_32 = al_load_ttf_font("./assets/font/Fatal Fighter.ttf", 32, 0);
   ALLEGRO_FONT* font_select_48 = al_load_ttf_font("./assets/font/Break Brush.ttf", 48, 0);
   ALLEGRO_FONT* font_select_32 = al_load_ttf_font("./assets/font/Break Brush.ttf", 32, 0);

   /* Cores utilizadas para o texto */
   ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
   ALLEGRO_COLOR preto = al_map_rgb(0, 0, 0);
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

   *stage = stage01;
   while (true)
   {
      al_wait_for_event(event_queue, &event);

      /* Evento 01:
       * Encerramento do Jogo */
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
         closeGame = true;
         break;
      }
      else if (event.type == ALLEGRO_EVENT_KEY_DOWN){
         if (event.keyboard.keycode == ALLEGRO_KEY_LEFT || event.keyboard.keycode == ALLEGRO_KEY_RIGHT){
            if (option == 0){
               frame = 0;
               gamemode = gamemode ^ 1;
            }
            else {
               *stage = (sel_stage == 0) ? stage02 : stage01;
               sel_stage = sel_stage ^ 1;
            }
         }
         else if (event.keyboard.keycode == ALLEGRO_KEY_UP || event.keyboard.keycode == ALLEGRO_KEY_DOWN){
            option = option ^ 1;
         }
         else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER){
            if (gamemode == 0){ *char_p1 = RYU; *char_p2 = KEN; }
            else              { *char_p1 = KEN; *char_p2 = RYU; }
            break;
         }
      }
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
               al_draw_text(bg_select_32, al_map_rgb(77, 78, 79), w, 
                              (-(h_string + 10 + 400) + (h + 10)
                              + (int) movement), 0, "STREET FIGHTER");
         
         al_identity_transform(&transf);
         al_use_transform(&transf);
         /* --- --- Fim da Rotação --- --- */

         /* Estágio de Fundo selecionado */
         al_draw_scaled_bitmap(*stage, 200, 25, 300, 200, 80, 60, 640, 480, 0);
         al_draw_rectangle(80, 60, 800-80, 600-60, al_map_rgb(23, 24, 25), 5);

         /* ------------------
          * Texto de Escolha
          * ------------------ */
         al_draw_text(font_select_48, al_map_rgb(0, 0, 0),       402, 12, ALLEGRO_ALIGN_CENTER, "ESCOLHA SEU LUTADOR");
         al_draw_text(font_select_48, al_map_rgb(255, 255, 255), 400, 10, ALLEGRO_ALIGN_CENTER, "ESCOLHA SEU LUTADOR");

         al_draw_text(font_select_32, al_map_rgb(0, 0, 0),       92, 502, 0, "JOGADOR 01");
         al_draw_text(font_select_32, al_map_rgb(255, 255, 255), 90, 500, 0, "JOGADOR 01");

         al_draw_text(font_select_32, al_map_rgb(0, 0, 0),       582, 502, 0, "JOGADOR 02");
         al_draw_text(font_select_32, al_map_rgb(255, 255, 255), 580, 500, 0, "JOGADOR 02");

         al_draw_text(font_select_32, al_map_rgb(0, 0, 0),       402, 452, ALLEGRO_ALIGN_CENTER, "Modo de Jogo");
         al_draw_text(font_select_32, al_map_rgb(255, 255, 255), 400, 450, ALLEGRO_ALIGN_CENTER, "Modo de Jogo");

         al_draw_text(font_select_32, al_map_rgb(0, 0, 0),       402, 522, ALLEGRO_ALIGN_CENTER, "Estagio de Luta");
         al_draw_text(font_select_32, al_map_rgb(255, 255, 255), 400, 520, ALLEGRO_ALIGN_CENTER, "Estagio de Luta");
      
         if (gamemode == 0){
            if (option == 0) cor = amarelo;
            else cor = branco;
            al_draw_text(font_select_32, preto, 402, 482, ALLEGRO_ALIGN_CENTER, "< RYU vs. KEN >");
            al_draw_text(font_select_32, cor,   400, 480, ALLEGRO_ALIGN_CENTER, "< RYU vs. KEN >");
         } 
         else if (gamemode == 1){
            if (option == 0) cor = amarelo;
            else cor = branco;
            al_draw_text(font_select_32, preto, 402, 482, ALLEGRO_ALIGN_CENTER, "< KEN vs. RYU >");
            al_draw_text(font_select_32, cor,   400, 480, ALLEGRO_ALIGN_CENTER, "< KEN vs. RYU >");
         }

         if (sel_stage == 0){
            if (option == 1) cor = amarelo;
            else cor = branco;
            al_draw_text(font_select_32, preto, 402, 552, ALLEGRO_ALIGN_CENTER, "< CIDADE JAPONESA>");
            al_draw_text(font_select_32, cor,   400, 550, ALLEGRO_ALIGN_CENTER, "< CIDADE JAPONESA>");
         } 
         else if (sel_stage == 1){
            if (option == 1) cor = amarelo;
            else cor = branco;
            al_draw_text(font_select_32, preto, 402, 552, ALLEGRO_ALIGN_CENTER, "< CASTELO SUZAKU >");
            al_draw_text(font_select_32, cor,   400, 550, ALLEGRO_ALIGN_CENTER, "< CASTELO SUZAKU >");
         }

         /* -------------------------------------------
          * Animação dos Personagens na Tela de Seleção
          * ------------------------------------------- */
         frame += 0.15;
         if (frame >= 5) frame = 0;

         if (gamemode == 0){
            al_draw_scaled_bitmap(p1->sprites->idle, 70*floor(frame), 0, 70, 95, p1->x, p1->y, 70*2.5, 95*2.5, 0);
            al_draw_scaled_bitmap(p2->sprites->idle, 70*floor(frame), 0, 70, 95, p2->x, p2->y, 70*2.5, 95*2.5, 1);
         } else {
            al_draw_scaled_bitmap(p2->sprites->idle, 70*floor(frame), 0, 70, 95, p1->x, p1->y, 70*2.5, 95*2.5, 0);
            al_draw_scaled_bitmap(p1->sprites->idle, 70*floor(frame), 0, 70, 95, p2->x, p2->y, 70*2.5, 95*2.5, 1);
         }

         al_flip_display();
      }
   }
   /* Destruição dos Elementos da Tela de Seleção */
   destroy_player(p1);
   destroy_player(p2);
   if (*stage != stage01) al_destroy_bitmap(stage01);
   if (*stage != stage02) al_destroy_bitmap(stage02);
   al_destroy_font(bg_select_32);
   al_destroy_font(font_select_48);
   al_destroy_font(font_select_32);
   al_stop_sample(&sel_music_id); 
   al_destroy_sample(sel_music);

   return closeGame;
}

int execFight(ALLEGRO_EVENT_QUEUE *ev_queue, Character char_p1, Character char_p2,
                  ALLEGRO_BITMAP* stage)
{
   ALLEGRO_EVENT ev;

   /* Criação dos Jogadores */
   Player* p1 = create_player(char_p1, 20, Y_MAX, 70, 95);
   Player* p2 = create_player(char_p2, DISPLAY_WIDTH - 200, Y_MAX, 70, 95);

   /* Fontes usadas na HUD da luta */
   ALLEGRO_FONT* fight_font_60 = al_load_ttf_font("./assets/font/Break Brush.ttf", 60, 0);
   ALLEGRO_FONT* fight_font_48 = al_load_ttf_font("./assets/font/Break Brush.ttf", 48, 0);
   ALLEGRO_FONT* fight_font_32 = al_load_ttf_font("./assets/font/Break Brush.ttf", 32, 0);

   /* Imagem utilizada para transparência do Menu de Pause */
   ALLEGRO_BITMAP* transparency = al_load_bitmap("./assets/screen.png");


   int x_stage;

   /* frame_p1, frame_p2: Controla o frame a ser exibido do sprite 
    * rounds_p1, rounds_p2: Controla quantos rounds cada jogador venceu
    * gameFinished: Controla se o jogo acabou */
   float frame_p1, frame_p2;
   unsigned short rounds_p1 = 0, rounds_p2 = 0;

   bool finishedGame = false;
   bool closeGame = false;

   /* Variáveis de uso do Menu de Pause */
   unsigned short pause_opt = 0;

   /* clk_timer: Controla o tempo de duração da batalha */
   float seconds;
   float clk_timer;
   char clk_string[3];

   /* ---------------
    * Loop da Partida
    * --------------- */
   while (true){
      /* Reinício do Round - Precisamos resetar as variáveis
       * de controle de frames, a coordenada de corte do cenário e
       * os elementos da luta: jogadores e contador */
      frame_p1 = 0;
      frame_p2 = 0;
      x_stage = 250;
      restart_round(p1, p2, &clk_timer);

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

         /* Atualiza a
          * Flag de Posição */
         update_player_pos_flags(p1, p2);

         /* --------------------
          * Encerramento da Luta
          * -------------------- */
         if (finishedGame){
            p1->move = p2->move = IDLE;
         }

         /* ---------------------
          * Tratamento de Eventos
          * --------------------- */
         /* Evento 01: Encerrar o Programa */
         if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            closeGame = true;
            break;
         }
         /* Evento 02: Menu de Pause */
         else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE && !finishedGame){
            do 
            {
               al_wait_for_event(ev_queue, &ev);

               /* Fechar o Programa enquanto está pausado */
               if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){ closeGame = true; break; }

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
                        frame_p1  = frame_p2  = 0;
                        x_stage   = 250;
                        restart_round(p1, p2, &clk_timer);
                        break;
                     }
                     /* Voltar ao Menu Principal
                      * LEMBRETE: Dar FREE em tudo da Luta! */
                     if (pause_opt == 2){
                        destroy_player(p1);
                        destroy_player(p2);
                        al_destroy_font(fight_font_60);
                        al_destroy_font(fight_font_48);
                        al_destroy_font(fight_font_32);
                        al_destroy_bitmap(stage);
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

            if (closeGame) break;
         }
         /* Evento 03: Inputs do Controle */
         else if ((ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_KEY_UP) && !finishedGame){
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

            /* -----------------------------
             * Tratamento de Flags de Enable
             * ----------------------------- */
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
               /* Flags do Jogador 01 */
               if      (ev.keyboard.keycode == ALLEGRO_KEY_W) p1->enableJump   = false;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_S) p1->isCrouching  = true;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_Z) p1->enableAtkSup = false;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_X) p1->enableAtkInf = false;

               /* Flags do Jogador 02 */
               if      (ev.keyboard.keycode == ALLEGRO_KEY_W) p1->enableJump   = false;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_S) p1->isCrouching  = true;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_Z) p1->enableAtkSup = false;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_X) p1->enableAtkInf = false;
            }
            if (ev.type == ALLEGRO_EVENT_KEY_UP){
               /* Flags do Jogador 01 */
               if      (ev.keyboard.keycode == ALLEGRO_KEY_UP) p1->enableJump   = true;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) p1->isCrouching  = false;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_K) p1->enableAtkSup = true;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_L) p1->enableAtkInf = true;

               /* Flags do Jogador 02 */
               if      (ev.keyboard.keycode == ALLEGRO_KEY_UP) p1->enableJump   = false;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) p1->isCrouching  = true;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_K) p1->enableAtkSup = false;
               else if (ev.keyboard.keycode == ALLEGRO_KEY_L) p1->enableAtkInf = false;
            }
         }
         /* Evento 04: Atualização do Temporizador */
         else if (ev.type == ALLEGRO_EVENT_TIMER){

            /* Exibição do Cenário de Fundo */
            al_draw_scaled_bitmap(stage, x_stage, 0, 300, 224, 0, 0, 800, 600, 0);

            /* Exibição dos Nomes dos Personagens */
            if (p1->selected_char == RYU){
               al_draw_text(fight_font_32, al_color_name("black"), 27.0,  82.0, 0, "RYU");
               al_draw_text(fight_font_32, al_color_name("white"), 25.0,  80.0, 0, "RYU");
               al_draw_text(fight_font_32, al_color_name("black"), 722.0, 82.0, 0, "KEN");
               al_draw_text(fight_font_32, al_color_name("white"), 720.0, 80.0, 0, "KEN");
            }
            else {
               al_draw_text(fight_font_32, al_color_name("black"), 722.0, 82.0, 0, "RYU");
               al_draw_text(fight_font_32, al_color_name("white"), 720.0, 80.0, 0, "RYU");
               al_draw_text(fight_font_32, al_color_name("black"), 27.0,  82.0, 0, "KEN");
               al_draw_text(fight_font_32, al_color_name("white"), 25.0,  80.0, 0, "KEN");
            }

            /* Exibição de informações importantes:
             * Pontuação, Contador da Luta, Barras de Vida */
            
            /* Barra de Vida - Jogador 01*/
            al_draw_filled_rectangle(30, 50, 330, 70, al_color_name("black"));
            if (p1->hit_points > 0) 
               al_draw_filled_rectangle(30.0, 50.0, 30.0 + 300*(p1->hit_points / (1.0*DEFAULT_HIT_POINTS)), 70.0, al_color_name("yellow"));
            al_draw_rectangle(29.5, 49.5, 330.5, 70.5, al_color_name("white"), 3);

            /* Barra de Vida - Jogador 02*/
            al_draw_filled_rectangle(470, 50, 770, 70, al_color_name("black"));
            if (p2->hit_points > 0)
               al_draw_filled_rectangle(470.0, 50.0, 470.0 + 300*(p2->hit_points / (1.0*DEFAULT_HIT_POINTS)), 70.0, al_color_name("yellow"));
            al_draw_rectangle(469.5, 49.5, 770.5, 70.5, al_color_name("white"), 3);

            /* Pontuação - Jogador 01 */
            if (rounds_p1 >= 1) al_draw_filled_rectangle(319.5, 79.5, 329.5, 99.5, al_color_name("orange"));
            al_draw_rectangle(320.0, 80.0, 330.0, 100.0, al_color_name("white"), 3.0);
            if (rounds_p1 >= 2) al_draw_filled_rectangle(299.5, 79.5, 309.5, 99.5, al_color_name("orange"));
            al_draw_rectangle(300.0, 80.0, 310.0, 100.0, al_color_name("white"), 3.0);

            /* Pontuação - Jogador 02 */
            if (rounds_p2 >= 1) al_draw_filled_rectangle(469.5, 79.5, 479.5, 99.5, al_color_name("orange"));
            al_draw_rectangle(470.0, 80.0, 480.0, 100.0, al_color_name("white"), 3.0);
            if (rounds_p2 >= 2) al_draw_filled_rectangle(489.5, 79.5, 499.5, 99.5, al_color_name("orange"));
            al_draw_rectangle(490.0, 80.0, 500.0, 100.0, al_color_name("white"), 3.0);

            if (!finishedGame){
               /* Contador de Tempo... (tic, tac...) */
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

            /* Fim da Luta */
            if (finishedGame){
               al_draw_text(fight_font_48, al_color_name("black"), 402.0, 132.0, ALLEGRO_ALIGN_CENTER, "Resultado da Partida");
               al_draw_text(fight_font_48, al_color_name("white"), 400.0, 130.0, ALLEGRO_ALIGN_CENTER, "Resultado da Partida");

               if (rounds_p1 == 2){
                  al_draw_text(fight_font_60, al_color_name("black"), 402.0, 182.0, ALLEGRO_ALIGN_CENTER, "JOGADOR 01");
                  al_draw_text(fight_font_60, al_color_name("white"), 400.0, 180.0, ALLEGRO_ALIGN_CENTER, "JOGADOR 01");
               }
               else if (rounds_p2 == 2){
                  al_draw_text(fight_font_60, al_color_name("black"), 402.0, 182.0, ALLEGRO_ALIGN_CENTER, "JOGADOR 02");
                  al_draw_text(fight_font_60, al_color_name("white"), 400.0, 180.0, ALLEGRO_ALIGN_CENTER, "JOGADOR 02");
               }
            }

            /* Exibição dos Sprites dos Jogadores */

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
            
            if (draw_sprite_player(p1, &frame_p1)) frame_p1 = 0;
            if (draw_sprite_player(p2, &frame_p2)) frame_p2 = 0;

            al_flip_display();
         }
         
         /* -------------------------------------
          * Tratamento de Movimentação e Colisões
          * ------------------------------------- */
         /* Tenta mexer o cenário de fundo, mas o que faz de fato
          * é modificar a coordenada x do corte da imagem de fundo */
         if (  ((p1->move == WALKING_POSITIVE && p1->pos_flag == 1)
               || (p2->move == WALKING_POSITIVE && p2->pos_flag == 1))
               && ((p1->x - p2->x <= DISPLAY_WIDTH - 70*2.5) || (p2->x - p1->x <= DISPLAY_WIDTH - 70*2.5))){
               x_stage += 1;
               if (x_stage > al_get_bitmap_width(stage) - 300)
                  x_stage -= 1;
         }
         if (  ((p1->move == WALKING_NEGATIVE && p1->pos_flag == 0)
               || (p2->move == WALKING_NEGATIVE && p2->pos_flag == 0))
               && ((p1->x - p2->x <= DISPLAY_WIDTH - 70*2.5) || (p2->x - p1->x <= DISPLAY_WIDTH - 70*2.5))){
               x_stage -= 1;
               if (x_stage < 0)
                  x_stage += 1;
         }

         /* Modificando a variável 'move' dos jogadores */
         update_player_movement(p1, p2);
         update_player_movement(p2, p1);
         
         /* Modifica a Flag de Pulo */
         if (p1->move == JUMPING && !p1->isJumping) p1->isJumping = true;
         if (p2->move == JUMPING && !p2->isJumping) p2->isJumping = true;

         /* Modifica as coordenadas dos jogadores baseado no movimento,
          * ou seja, realiza de fato o movimento dos jogadores */
         update_player_coordinates(p1, p2);
         update_player_coordinates(p2, p1);

         /* Modifica as hitboxes + hurtboxes dos jogadores baseado no
          * que eles estão fazendo, se estão pulando, agachando ou golpeando */
         update_player_boxes(p1, frame_p1);
         update_player_boxes(p2, frame_p2);

         /* Executa a matemática do ataque, baseado na colisão da hurtbox -> hitbox
          * Se a flag de enable não estiver ativa, não contabiliza o ataque */
         if (p1->enableAtkSup || p1->enableAtkInf) execute_attack(p1, p2);
         if (p2->enableAtkSup || p2->enableAtkInf) execute_attack(p2, p1);
      }

      /* Fim da Luta - Verifica se algum dos jogadores venceu 2 rounds
       * ou também se o jogo quer ser fechado, tornando a flag de fim de jogo verdadeira */
      if (rounds_p1 == 2 || rounds_p2 == 2 || closeGame) finishedGame = true;

      if (closeGame) break;
   }

   /* Destruição da Luta */
   destroy_player(p1);
   destroy_player(p2);
   al_destroy_font(fight_font_60);
   al_destroy_font(fight_font_48);
   al_destroy_font(fight_font_32);
   al_destroy_bitmap(stage);
   al_destroy_bitmap(transparency);

   return 1;
}

/* -------------------------------- */

int main(int argc, char* argv[])
{
   /* Variáveis de uso geral */
   ALLEGRO_BITMAP* stage;
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