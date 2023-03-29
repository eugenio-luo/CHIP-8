#include <string.h>
#include <SDL2/SDL.h>

#include "screen.h"
#include "common.h"
#include "debug.h"

uint8_t  g_screen[SCREEN_WIDTH * SCREEN_HEIGHT];

static SDL_Window *g_window;
static SDL_Renderer *g_renderer;

/* main color RGB, for text, objects */
static uint8_t mcolor[3] = {MCOLOR_R, MCOLOR_G, MCOLOR_B};
/* Off color RGB, for background */
static uint8_t ocolor[3] = {OCOLOR_R, OCOLOR_G, OCOLOR_B};

void
scr_reset(void)
{
        memset(g_screen, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
}
        
void
scr_init(void)
{
        scr_reset();
        
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
                dbg_err("[screen.c: scr_reset] SDL error: %s\n", SDL_GetError());

        g_window = SDL_CreateWindow(PROG_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    DP_WIDTH, DP_HEIGHT, SDL_WINDOW_SHOWN);

        if (!g_window)
                dbg_err("[screen.c: scr_reset] SDL error: %s", SDL_GetError());

        g_renderer = SDL_CreateRenderer(g_window, -1, 0);

        if (!g_renderer)
                dbg_err("[screen.c: scr_reset] SDL error: %s", SDL_GetError());

        SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_NONE);
}

void
scr_quit(void)
{
        SDL_DestroyRenderer(g_renderer);
        SDL_DestroyWindow(g_window);
        SDL_Quit();
}

uint8_t
scr_get(int w, int h)
{
        if (w >= SCREEN_WIDTH)
                dbg_err("[screen.c: scr_get] width out of bound");

        if (h >= SCREEN_HEIGHT)
                dbg_err("[screen.c: scr_get] height out of bound");
        
        return g_screen[w + h * SCREEN_WIDTH];
}

void
scr_on(int w, int h)
{
        if (w >= SCREEN_WIDTH)
                dbg_err("[screen.c: scr_on] width out of bound");

        if (h >= SCREEN_HEIGHT)
                dbg_err("[screen.c: scr_on] height out of bound");
        
        g_screen[w + h * SCREEN_WIDTH] = 1;
}

void
scr_off(int w, int h)
{
        if (w >= SCREEN_WIDTH)
                dbg_err("[screen.c: scr_on] width out of bound");
        
        if (h >= SCREEN_HEIGHT)
                dbg_err("[screen.c: scr_on] height out of bound");
        
        g_screen[w + h * SCREEN_WIDTH] = 0;
}
        
void
scr_render(void)
{
        SDL_RenderClear(g_renderer);
        SDL_SetRenderDrawColor(g_renderer, ocolor[0], ocolor[1], ocolor[2], SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(g_renderer, NULL);

        SDL_SetRenderDrawColor(g_renderer, mcolor[0], mcolor[1], mcolor[2], SDL_ALPHA_OPAQUE);
        for (int h = 0; h < SCREEN_HEIGHT; ++h) {
                for (int w = 0; w < SCREEN_WIDTH; ++w) {
                        if (!scr_get(w, h))
                                continue;

                        SDL_Rect rect = {w * DP_MULT, h * DP_MULT, DP_MULT, DP_MULT};
                        SDL_RenderFillRect(g_renderer, &rect);
                } 
        }
        
        SDL_RenderPresent(g_renderer);
}

