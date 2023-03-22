#include <stdio.h>
#include <string.h>

#include "common.h"
#include "system.h"
#include "memory.h"
#include "registers.h"
#include "opcode.h"

uint8_t  g_screen[SCREEN_WIDTH * SCREEN_HEIGHT];
uint8_t  g_keys[KEYS_COUNT];

uint8_t  g_delay_timer;
uint8_t  g_sound_timer;

int
sys_init(void)
{
        g_delay_timer = 0;
        g_sound_timer = 0;

        op_reset();
        reg_reset();
        mem_reset();

        memset(g_screen, 0, SCREEN_WIDTH * SCREEN_HEIGHT);

        mem_ld_font();
        
        return 0;
}

int
sys_load(const char *file_name)
{
        printf("FILE_NAME: %s\n", file_name);
        return 0;
}

void
sys_cycle(void)
{
        
}
