#include <string.h>

#include "common.h"
#include "registers.h"
#include "debug.h"

reg_t    g_reg[REGS_COUNT]; // general registers
uint16_t g_idx_reg;   // index register
uint16_t g_prog_cntr; // program counter
uint16_t g_stack_ptr; // stack pointer

uint16_t g_stack[STACK_SIZE];

void
reg_reset(void)
{
        g_prog_cntr = PROGRAM_START;
        g_idx_reg = 0;
        g_stack_ptr = 0;
        
        memset(g_reg, 0, REGS_COUNT * sizeof(reg_t));
        memset(g_stack, 0, STACK_SIZE * sizeof(uint16_t));
}

uint16_t
reg_get(int idx)
{
        if ((idx >= REGS_COUNT) | (idx < 0))
                dbg_err("[registers.c: reg_get] register index out of bound");

        return g_reg[idx];
}

void
reg_set(int idx, uint16_t val)
{
        if ((idx >= REGS_COUNT) | (idx < 0))
                dbg_err("[registers.c: reg_get] register index out of bound");

        g_reg[idx] = val;
}

