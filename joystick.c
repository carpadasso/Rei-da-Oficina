#include <stdbool.h>
#include <stdlib.h>
#include "joystick.h"

Joystick* create_joystick()
{
   /* Aloca memória */
   Joystick* cntrl = (Joystick*) malloc(sizeof(Joystick));
   if (!cntrl) return NULL;

   /* Inicializa as flags (inicialmente não pressionadas) */
   cntrl->up = false;
   cntrl->down = false;
   cntrl->left = false;
   cntrl->right = false;
   cntrl->button_1 = false;
   cntrl->button_2 = false;

   return cntrl;
}

void destroy_joystick(Joystick* cntrl) { free(cntrl); cntrl = NULL; }

void joystick_up(Joystick* cntrl) { cntrl->up = cntrl->up ^ 1; }

void joystick_down(Joystick* cntrl) { cntrl->down = cntrl->down ^ 1; }

void joystick_left(Joystick* cntrl) { cntrl->left = cntrl->left ^ 1; }

void joystick_right(Joystick* cntrl) { cntrl->right = cntrl->right ^ 1; }

void joystick_button_1(Joystick* cntrl) { cntrl->button_1 = cntrl->button_1 ^ 1; }

void joystick_button_2(Joystick* cntrl) { cntrl->button_2 = cntrl->button_2 ^ 1; }