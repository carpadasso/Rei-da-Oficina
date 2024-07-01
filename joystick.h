#ifndef __JOYSTICK__
#define __JOYSTICK__

#include <stdbool.h>

typedef struct {
   bool up;
   bool down;
   bool left;
   bool right;
} Joystick;

Joystick* joystick_create(void);
void joystick_destroy(Joystick*);
void joystick_up(Joystick*);
void joystick_down(Joystick*);
void joystick_left(Joystick*);
void joystick_right(Joystick*);

#endif