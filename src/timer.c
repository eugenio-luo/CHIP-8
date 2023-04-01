#include <SDL2/SDL.h>

#include "timer.h"
#include "common.h"

uint8_t  g_delay_timer;
uint8_t  g_sound_timer;

uint32_t g_last_tick;

void
tme_reset(void)
{
        g_last_tick = SDL_GetTicks();
        g_delay_timer = 0;
        g_sound_timer = 0;
}

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

uint8_t
tme_get_snd(void)
{
        return g_sound_timer;
}

void
tme_update(void)
{
        uint32_t tick = SDL_GetTicks();
        if (tick - g_last_tick > TIMER_PERIOD) {
                if (g_delay_timer)
                        g_delay_timer--;

                if (g_sound_timer) {
                        printf("BEEP\n");
                        g_sound_timer--;
                }
                        
                g_last_tick = tick;
        }
}
