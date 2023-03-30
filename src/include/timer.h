#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>

void tme_reset(void);
void tme_set_del(uint8_t val);
uint8_t tme_get_del(void);
void tme_set_snd(uint8_t val);

#endif
