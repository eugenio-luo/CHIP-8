#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <SDL2/SDL.h>

#include "common.h"
#include "system.h"
#include "memory.h"
#include "registers.h"
#include "opcode.h"
#include "stack.h"
#include "debug.h"
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int
sys_init(void)
{
        op_reset();
        reg_reset();
        stc_reset();
        mem_reset();
        key_reset();
        /* TODO: better naming convention? */
        scr_init();
        tme_reset();

        mem_ld_font();
        
        return 0;
}
        
int
sys_reset(void)
{
        op_reset();
        reg_reset();
        stc_reset();
        mem_reset();
        key_reset();
        scr_reset();
        tme_reset();
        
        mem_ld_font();
        
        return 0;
}

void
sys_load(const char *file_name)
{
        FILE *f_ptr = fopen(file_name, "r");
        if (!f_ptr)
                dbg_err("[system.c: sys_load] fopen failed: %s", strerror(errno));

        if (fseek(f_ptr, 0L, SEEK_END))
                dbg_err("[system.c: sys_load] fseek failed");

        long f_size = ftell(f_ptr);

        if (f_size == -1 || f_size > (MEMORY_SIZE - PROGRAM_START))
                dbg_err("[system.c: sys_load] file size error: %d", f_size);

        if (fseek(f_ptr, 0L, SEEK_SET))
                dbg_err("[system.c: sys_load] fseek failed");

        mem_load(f_ptr, f_size);

        if (ferror(f_ptr))
                dbg_err("[system.c: sys_load] error in reading file");

        fclose(f_ptr);
}

void
sys_cycle(void)
{
        mem_set(0x1FF, 1);
        
        for (int i = 0; i < 39; ++i) {
                printf("%d: ", i);
                op_nxt();
        }
       
        int quit = 0;
        SDL_Event event;
        while (!quit) {
                while (SDL_PollEvent(&event)) {
                        switch (event.type) {
                        case SDL_QUIT:
                                quit = 1;
                                break;

                        case SDL_KEYDOWN:
                        case SDL_KEYUP:
                                key_handle(&event);
                        }
                }
        }
}

void
sys_quit(void)
{
        scr_quit();
}

