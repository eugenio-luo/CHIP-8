#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "system.h"
#include "memory.h"
#include "registers.h"
#include "opcode.h"
#include "stack.h"
#include "debug.h"

uint8_t  g_screen[SCREEN_WIDTH * SCREEN_HEIGHT];
uint8_t  g_keys[KEYS_COUNT];

uint8_t  g_delay_timer;
uint8_t  g_sound_timer;

int
sys_reset(void)
{
        g_delay_timer = 0;
        g_sound_timer = 0;

        op_reset();
        reg_reset();
        stc_reset();
        mem_reset();

        memset(g_screen, 0, SCREEN_WIDTH * SCREEN_HEIGHT);

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
        op_nxt();
}
