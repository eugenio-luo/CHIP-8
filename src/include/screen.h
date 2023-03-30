#ifndef _SCREEN_H
#define _SCREEN_H

void scr_reset(void);
void scr_init(void);
void scr_quit(void);
uint8_t scr_get(int w, int h);
void scr_set(int w, int h, int val);
void scr_clear(void);
void scr_render(void);

#endif
