#ifndef _STACK_H
#define _STACK_H

#include <stdint.h>

void stc_reset(void);
int16_t stc_get_ptr(void);
void stc_push(uint16_t val);
void stc_pop(void);
uint16_t stc_top(void);

#endif
