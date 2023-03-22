#include <string.h>

#include "memory.h"
#include "common.h"
#include "debug.h"

/* TODO: create its own file to manage fontset */
/* for debug reason it has its last value as 1 */
uint8_t  g_fontset[FONTSET_SIZE] = {[FONTSET_SIZE - 1] = 1}; 
uint8_t  g_memory[MEMORY_SIZE];

void
mem_reset(void)
{
        memset(g_memory, 0, MEMORY_SIZE);
}

void
mem_ld_font(void)
{
        for (int i = 0; i < FONTSET_SIZE; ++i)
                g_memory[i] = g_fontset[i];
}

uint8_t
mem_get(int off)
{
        if ((off >= MEMORY_SIZE) | (off < 0))
                dbg_err("[memory.c: mem_get] memory offset out of bound");
        
        return g_memory[off];
}

void
mem_set(int off, uint8_t val)
{
        if ((off >= MEMORY_SIZE) | (off < 0))
                dbg_err("[memory.c: mem_set] memory offset out of bound");

        g_memory[off] = val;
}

/* DEBUG & TEST ONLY USE */

uint8_t *
mem_ptr(void)
{
        return &g_memory[0];
}

