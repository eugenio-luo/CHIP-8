#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <SDL2/SDL.h>

void key_reset(void);
uint8_t key_get(int idx);
int key_wait(void);
int key_handle(SDL_Event *event);

/* DEBUG & TEST ONLY USE */
void key_set(int idx, int val);

#endif
