#include <string.h>
#include <stdio.h>

#include "memory.h"
#include "common.h"
#include "debug.h"

uint8_t  g_memory[MEMORY_SIZE];
uint8_t g_fontset[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */ 
        0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */ 
        0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
        0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
        0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
        0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
        0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
        0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
        0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
        0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
        0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
        0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
        0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
        0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
        0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
        0xF0, 0x80, 0xF0, 0x80, 0x80, /* F */
};

void
mem_reset(void)
{
        memset(g_memory, 0, MEMORY_SIZE);
}

void
mem_ld_font(void)
{
        for (int i = 0; i < FONTSET_SIZE; ++i)
                g_memory[i + FONTSET_OFF] = g_fontset[i];
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

void
mem_load(FILE *f_ptr, long f_size)
{
        fread(&g_memory[PROGRAM_START], sizeof(uint8_t), f_size, f_ptr);
}

/* DEBUG & TEST ONLY USE */

uint8_t *
mem_ptr(void)
{
        return &g_memory[0];
}

