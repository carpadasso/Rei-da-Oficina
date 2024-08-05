#ifndef __JOYSTICK__
#define __JOYSTICK__

#include <stdbool.h>

/* ------------------
 * Tipos e Estruturas
 * ------------------ */
/* Define a estrutura JOYSTICK
 * > Botões de Direção: up, down, left, right
 * > Botões de Ação: button_1, button_2, button_3 */
typedef struct {
   bool up, down, left, right;
   bool button_1, button_2, button_3;
} Joystick;

/* ---------------------
 * Funções da Biblioteca
 * --------------------- */
/* Aloca memória para uma estrutura JOYSTICK. */
Joystick* create_joystick(void);

/* Libera memória de uma estrutura JOYSTICK. */
void destroy_joystick(Joystick*);

/* -------------------------------------------------------------------------- */

/* Alterna o estado do botão UP de um JOYSTICK. */
void joystick_up(Joystick*);

/* Alterna o estado do botão DOWN de um JOYSTICK. */
void joystick_down(Joystick*);

/* Alterna o estado do botão LEFT de um JOYSTICK. */
void joystick_left(Joystick*);

/* Alterna o estado do botão RIGHT de um JOYSTICK. */
void joystick_right(Joystick*);

/* Alterna o estado do botão BUTTON_1 de um JOYSTICK. */
void joystick_button_1(Joystick*);

/* Alterna o estado do botão BUTTON_2 de um JOYSTICK. */
void joystick_button_2(Joystick*);

/* Alterna o estado do botão BUTTON_3 de um JOYSTICK. */
void joystick_button_3(Joystick*);

#endif