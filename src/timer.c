#include "timer.h"

uint8_t  g_delay_timer;
uint8_t  g_sound_timer;

void
tme_reset(void)
{
        g_delay_timer = 0;
        g_sound_timer = 0;
}

/* TODO: introduce timer */
void
tme_set_del(uint8_t val)
{
        g_delay_timer = val;
}

uint8_t
tme_get_del(void)
{
        return g_delay_timer;
}

/* TODO: introduce sound timer */
void
tme_set_snd(uint8_t val)
{
        g_sound_timer = val;
}
