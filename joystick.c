#include <stdbool.h>
#include <stdlib.h>
#include "joystick.h"

/* ----------------
 * Criar e Destruir
 * ---------------- */
Joystick* create_joystick()
{
   Joystick* cntrl = (Joystick*) malloc(sizeof(Joystick));
   if (!cntrl) return NULL;

   /* Inicializa as flags
    * (inicialmente não pressionadas) */
   cntrl->up       = false;
   cntrl->down     = false;
   cntrl->left     = false;
   cntrl->right    = false;
   cntrl->button_1 = false;
   cntrl->button_2 = false;
   cntrl->button_3 = false;

   return cntrl;
}

void destroy_joystick(Joystick* cntrl){ free(cntrl); cntrl = NULL; }


/* ------------------------------
 * Ativação de Botões do Controle
 * ------------------------------ */
void joystick_up(Joystick* j){
   if (j) j->up = j->up ^ 1;
}

void joystick_down(Joystick* j){
   if (j) j->down = j->down ^ 1;
}

void joystick_left(Joystick* j){
   if (j) j->left = j->left ^ 1;
}

void joystick_right(Joystick* j){
   if (j) j->right = j->right ^ 1;
}

void joystick_button_1(Joystick* j){
   if (j) j->button_1 = j->button_1 ^ 1;
}

void joystick_button_2(Joystick* j){
   if (j) j->button_2 = j->button_2 ^ 1;
}

void joystick_button_3(Joystick* j){
   if (j) j->button_3 = j->button_3 ^ 1;
}