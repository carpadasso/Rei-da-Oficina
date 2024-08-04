#ifndef __JOYSTICK__
#define __JOYSTICK__

#include <stdbool.h>

typedef struct {
   bool up, down, left, right;
   bool button_1, button_2, button_3;
} Joystick;

Joystick* create_joystick(void);
void destroy_joystick(Joystick*);

void joystick_up(Joystick*);
void joystick_down(Joystick*);
void joystick_left(Joystick*);
void joystick_right(Joystick*);
void joystick_button_1(Joystick*);
void joystick_button_2(Joystick*);
void joystick_button_3(Joystick*);

#endif