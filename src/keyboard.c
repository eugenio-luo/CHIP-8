#include <string.h>
#include <SDL2/SDL.h>

#include "common.h"
#include "keyboard.h"
#include "debug.h"

uint8_t  g_keys[KEYS_COUNT];

void
key_reset(void)
{
        memset(g_keys, 0, KEYS_COUNT);
}

uint8_t
key_get(int idx)
{
        if (idx > 0xF || idx < 0)
                dbg_err("[keyboard.c: key_get] out of bound key");

        return g_keys[idx];
}

void
key_handle(SDL_Event *event)
{
        switch (event->key.keysym.scancode) {
        case SDL_SCANCODE_1:
                g_keys[1] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_2:
                g_keys[2] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_3:
                g_keys[3] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_4:
                g_keys[0xC] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_Q:
                g_keys[4] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_W:
                g_keys[5] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_E:
                g_keys[6] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_R:
                g_keys[0xD] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_A:
                g_keys[7] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;
                
        case SDL_SCANCODE_S:
                g_keys[8] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_D:
                g_keys[9] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_F:
                g_keys[0xE] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_Z:
                g_keys[0xA] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_X:
                g_keys[0] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_B:
                g_keys[0xC] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;

        case SDL_SCANCODE_V:
                g_keys[0xF] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                break;
                
        default:
                break;
        }
}

/* DEBUG & TEST ONLY USE */

void
key_set(int idx, int val)
{
        if (idx > 0xF || idx < 0)
                dbg_err("[keyboard.c: key_set] out of bound key");

        g_keys[idx] = val & 0x1;
}
