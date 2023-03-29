#ifndef _SCREEN_H
#define _SCREEN_H

void scr_reset(void);
void scr_init(void);
void scr_quit(void);
uint8_t scr_get(int w, int h);
void scr_on(int w, int h);
void scr_off(int w, int h);
void scr_render(void);

#endif
