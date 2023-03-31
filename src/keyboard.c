#include <string.h>
#include <SDL2/SDL.h>

#include "common.h"
#include "keyboard.h"
#include "debug.h"
#include "registers.h"

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

int
key_wait(void)
{
        /* CHIP-8's strange quirk, 0xFX0A halts execution until key press, it means that
           the program counter shouldn't increased too, but we already increased it, so we
           decrease it again. It isn't possible to increase AFTER execution as the right
           behaviour is to increase the program counter BEFORE executing the instruction */

        reg_dec_pc();
        
        SDL_Event event;
        while (1) {
                while (SDL_PollEvent(&event)) {
                        if (event.type != SDL_KEYDOWN)
                                continue;

                        int key = key_handle(&event);

                        /* another strange quirk, there's a small delay after the key press
                           to check if there's a release, because it prevents spurious readings */

                        /* TODO: introduce quirk */
                        
                        if (key != -1)
                                return key;
                }
        }
}

int       
key_handle(SDL_Event *event)
{
        switch (event->key.keysym.scancode) {
        case SDL_SCANCODE_1:
                g_keys[1] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 1;
                
        case SDL_SCANCODE_2:
                g_keys[2] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 2;
                
        case SDL_SCANCODE_3:
                g_keys[3] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 3;
                
        case SDL_SCANCODE_4:
                g_keys[0xC] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 0xC;
                
        case SDL_SCANCODE_Q:
                g_keys[4] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 4;
                
        case SDL_SCANCODE_W:
                g_keys[5] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 5;

        case SDL_SCANCODE_E:
                g_keys[6] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 6;
                
        case SDL_SCANCODE_R:
                g_keys[0xD] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 0xD;
                
        case SDL_SCANCODE_A:
                g_keys[7] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 7;

        case SDL_SCANCODE_S:
                g_keys[8] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 8;
                
        case SDL_SCANCODE_D:
                g_keys[9] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 9;
                
        case SDL_SCANCODE_F:
                g_keys[0xE] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 0xE;
                
        case SDL_SCANCODE_Z:
                g_keys[0xA] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 0xA;
                
        case SDL_SCANCODE_X:
                g_keys[0] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 0;

        case SDL_SCANCODE_C:
                g_keys[0xB] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 0xB;
                
        case SDL_SCANCODE_V:
                g_keys[0xF] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                return 0xF;
                
        default:
                return -1;
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
