#ifndef __JOYSTICK__
#define __JOYSTICK__

#include <stdbool.h>

typedef struct {
   bool up;
   bool down;
   bool left;
   bool right;
   bool button_1;
   bool button_2;
} Joystick;

Joystick* create_joystick(void);
void destroy_joystick(Joystick*);
void joystick_up(Joystick*);
void joystick_down(Joystick*);
void joystick_left(Joystick*);
void joystick_right(Joystick*);
void joystick_button_1(Joystick*);
void joystick_button_2(Joystick*);

#endif