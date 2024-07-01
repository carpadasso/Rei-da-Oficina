#include <stdbool.h>
#include <stdlib.h>
#include "joystick.h"

Joystick* joystick_create()
{
   Joystick* cntrl = (Joystick*) malloc(sizeof(Joystick));
   if (!cntrl) return NULL;
   cntrl->up = false;
   cntrl->down = false;
   cntrl->left = false;
   cntrl->right = false;
   return cntrl;
}

void joystick_destroy(Joystick* cntrl) { free(cntrl); cntrl = NULL; }

void joystick_up(Joystick* cntrl) { cntrl->up = cntrl->up ^ 1; }

void joystick_down(Joystick* cntrl) { cntrl->down = cntrl->down ^ 1; }

void joystick_left(Joystick* cntrl) { cntrl->left = cntrl->left ^ 1; }

void joystick_right(Joystick* cntrl) { cntrl->right = cntrl->right ^ 1; }