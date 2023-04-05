#ifndef _SCREEN_H
#define _SCREEN_H

#include <stdarg.h>

void scr_reset(void);
void scr_init(void);
void scr_quit(void);
uint8_t scr_get(int w, int h);
void scr_set(int w, int h, int val);
void scr_clear(void);
void scr_render(void);
void scr_add_dbg(const char *fmt, va_list list); 

#endif
