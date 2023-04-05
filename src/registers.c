#include <string.h>

#include "common.h"
#include "registers.h"
#include "debug.h"

reg_t    g_reg[REGS_COUNT]; // general registers
uint16_t g_idx_reg;   // index register
uint16_t g_prog_cntr; // program counter

void
reg_reset(void)
{
        g_prog_cntr = PROGRAM_START;
        g_idx_reg = 0;
        
        memset(g_reg, 0, REGS_COUNT * sizeof(reg_t));
}

uint8_t
reg_get(int idx)
{
        if ((idx >= REGS_COUNT) | (idx < 0))
                dbg_err("[registers.c: reg_get] register index out of bound");

        return g_reg[idx];
}

void
reg_set(int idx, uint8_t val)
{
        if ((idx >= REGS_COUNT) | (idx < 0))
                dbg_err("[registers.c: reg_get] register index out of bound");

        g_reg[idx] = val;
}

uint16_t
reg_get_pc(void)
{
        return g_prog_cntr;
}

void
reg_inc_pc(void)
{
        g_prog_cntr += 2;
}

void
reg_dec_pc(void)
{
        g_prog_cntr -= 2;
}

void
reg_set_pc(uint16_t addr)
{
        if ((addr >= MEMORY_SIZE) | (addr < PROGRAM_START))
                dbg_err("[registers.c: reg_set_pc] program counter out of bound");
        
        g_prog_cntr = addr;
}

uint16_t
reg_get_idx(void)
{
        return g_idx_reg;
}

void
reg_set_idx(uint16_t addr)
{
        if (addr >= MEMORY_SIZE)
                dbg_err("[registers.c: reg_set_idx] index register out of bound");

        g_idx_reg = addr;
}

void
reg_inc_idx(void)
{
        ++g_idx_reg;
}
