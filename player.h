#include <allegro5/allegro5.h>
#include "joystick.h"
#include "sprite.h"

/* Define constantes de inicialização do jogo */
#define DISPLAY_WIDTH  800
#define DISPLAY_HEIGHT 600
#define REFRESH_RATE   60.0
#define STEP_FRONT     10
#define STEP_BACK      5

/* Define constantes para cada direção de movimento do jogador */
enum
{ 
   UP, 
   DOWN, 
   LEFT, 
   RIGHT 
};

/* Define constantes para cada personagem */
typedef enum { 
   RYU,
   CHUNLI
} Character;

/* Define a estrutura "Player"
 * selected_char: Personagem escolhido pelo jogador
 * joystick: Registro de movimento do jogador
 * sprites: Coleção dos sprites de movimento
 * current_x: Coordenada x atual do jogador
 * current_y: Coordenada y atual do jogador
 * current_lenght: Comprimento atual do sprite do jogador
 * current_height: Altura atual do sprite do jogador
 * hit_points: Pontos de vida atual do jogador */
typedef struct {
   Character selected_char;
   Joystick* joystick;
   Sprite* sprites;
   unsigned short current_x;
   unsigned short current_y;
   unsigned short current_lenght;
   unsigned short current_height;
   unsigned short hit_points;
} Player;

/* Funções da Interface "player" */

/* creater_player:
 * Aloca memória para um jogador recém criado.
 * Recebe e atribui os parâmetros do jogador fornecidos.
 * Retorna um ponteiro para a estrutura alocada. */
Player* create_player(Character char_selected, unsigned short x, unsigned short y,
                      unsigned short w, unsigned short h);
/* destroy_player:
 * */
void destroy_player(Player* player);
/* move_player:
 * */
int move_player(Player* player, unsigned short direction, unsigned short step,
                unsigned short velocity, unsigned short max_x, unsigned short max_y);