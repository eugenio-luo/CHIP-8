#include <string.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "screen.h"
#include "common.h"
#include "debug.h"

uint8_t  g_screen[SCREEN_WIDTH * SCREEN_HEIGHT];

static SDL_Window *g_window;
static SDL_Renderer *g_renderer;
static TTF_Font *g_font;

typedef struct {
        SDL_Surface *srf;
        SDL_Texture *txt;
        int w;
        int h;
} dline;

static struct {
        int size;
        int start;
        int end;
        dline lines[DEBUG_LINES];
} g_dbg_scr;

/* main color RGB, for text, objects */
static SDL_Color mcolor = {
        .r = MCOLOR_R,
        .g = MCOLOR_G,
        .b = MCOLOR_B,
        .a = SDL_ALPHA_OPAQUE
};
/* Off color RGB, for background */
static SDL_Color ocolor = {
        .r = OCOLOR_R,
        .g = OCOLOR_G,
        .b = OCOLOR_B,
        .a = SDL_ALPHA_OPAQUE
};

void
scr_reset(void)
{
        g_dbg_scr.start = 0;
        g_dbg_scr.size = 0;
        g_dbg_scr.end = 0;
        
        memset(g_dbg_scr.lines, 0, DEBUG_LINES * sizeof(g_dbg_scr.lines[0]));
        memset(g_screen, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
}
        
void
scr_init(void)
{
        scr_reset();
        
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
                dbg_err("[screen.c: scr_reset] SDL error: %s\n", SDL_GetError());

        if (TTF_Init())
                dbg_err("[screen.c: scr_reset] TTF error: %s\n", TTF_GetError());

        g_font = TTF_OpenFont("/usr/share/fonts/iosevka.ttc", 16);
        g_window = SDL_CreateWindow(PROG_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    DP_WIDTH + DEBUG_WIDTH, DP_HEIGHT, SDL_WINDOW_SHOWN);

        if (!g_window)
                dbg_err("[screen.c: scr_reset] SDL error: %s", SDL_GetError());

        g_renderer = SDL_CreateRenderer(g_window, -1, 0);

        if (!g_renderer)
                dbg_err("[screen.c: scr_reset] SDL error: %s", SDL_GetError());

        SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_NONE);
}

static void
scr_free_dbg_line(dline *line)
{
        if (line->srf)
                SDL_FreeSurface(line->srf);

        if (line->txt)
                SDL_DestroyTexture(line->txt);
}

void
scr_quit(void)
{
        SDL_DestroyRenderer(g_renderer);
        SDL_DestroyWindow(g_window);

        for (int i = 0; i < DEBUG_LINES; ++i)
                scr_free_dbg_line(&g_dbg_scr.lines[i]);
                
        TTF_CloseFont(g_font);
        TTF_Quit();
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
scr_set(int w, int h, int val)
{
        if (w >= SCREEN_WIDTH)
                dbg_err("[screen.c: scr_on] width out of bound");

        if (h >= SCREEN_HEIGHT)
                dbg_err("[screen.c: scr_on] height out of bound");
        
        g_screen[w + h * SCREEN_WIDTH] = (val & 0x1);
}

static dline *
src_nxt_dbg_line(void)
{
        dline *line = &g_dbg_scr.lines[g_dbg_scr.end];

        g_dbg_scr.end = (g_dbg_scr.end + 1) % DEBUG_LINES;
        if (g_dbg_scr.size < DEBUG_LINES)
                ++g_dbg_scr.size;
        else
                g_dbg_scr.start = (g_dbg_scr.start) % DEBUG_LINES;
        
        scr_free_dbg_line(line);
        return line;
}

void
scr_add_dbg(const char *fmt, va_list list)
{
        static char buffer[DEBUG_LINE_SIZE];

        sprintf(&buffer[0], "[%d] ", SDL_GetTicks());
        vsprintf(&buffer[strlen(buffer)], fmt, list);
        
        dline *line = src_nxt_dbg_line();
        line->srf = TTF_RenderText_Solid(g_font, &buffer[0], mcolor);
        line->txt = SDL_CreateTextureFromSurface(g_renderer, line->srf);
        TTF_SizeText(g_font, &buffer[0], &line->w, &line->h);
}

static void
scr_render_dbg(void)
{
        SDL_SetRenderDrawColor(g_renderer, mcolor.r, mcolor.g, mcolor.b, mcolor.a);
        SDL_Rect rect = {DP_WIDTH + 1, 0, 10, DP_HEIGHT};
        SDL_RenderFillRect(g_renderer, &rect);

        for (int i = 0; i < g_dbg_scr.size; ++i) {

                int idx = (g_dbg_scr.start + i) % DEBUG_LINES;
                dline *line = &g_dbg_scr.lines[idx];

                rect = (SDL_Rect) {DP_WIDTH + 15, 10 + i * line->h, line->w, line->h};
                SDL_RenderCopy(g_renderer, line->txt, NULL, &rect);
        }
}

void
scr_clear(void)
{
        memset(g_screen, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
        
        SDL_SetRenderDrawColor(g_renderer, ocolor.r, ocolor.g, ocolor.b, ocolor.a);
        SDL_RenderClear(g_renderer);

        scr_render_dbg();
        
        SDL_RenderPresent(g_renderer);
}

void
scr_render(void)
{
        SDL_SetRenderDrawColor(g_renderer, ocolor.r, ocolor.g, ocolor.b, ocolor.a);
        SDL_RenderClear(g_renderer);

        SDL_SetRenderDrawColor(g_renderer, mcolor.r, mcolor.g, mcolor.b, mcolor.a);
        for (int h = 0; h < SCREEN_HEIGHT; ++h) {
                for (int w = 0; w < SCREEN_WIDTH; ++w) {
                        if (!scr_get(w, h))
                                continue;

                        SDL_Rect rect = {w * DP_MULT, h * DP_MULT, DP_MULT, DP_MULT};
                        SDL_RenderFillRect(g_renderer, &rect);
                } 
        }

        scr_render_dbg();
        
        SDL_RenderPresent(g_renderer);
}

