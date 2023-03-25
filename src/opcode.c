#include "opcode.h"
#include "common.h"
#include "debug.h"
#include <stdlib.h>
#include <time.h>

#include "memory.h"
#include "registers.h"
#include "stack.h"

typedef void (*op_func_t)(void);

opcode_t g_opcode;

#define ADDR(OPCODE) (OPCODE & 0xFFF)
#define NIB(OPCODE) (OPCODE & 0xF)
#define BX(OPCODE) ((OPCODE >> 8) & 0xF) 
#define BY(OPCODE) ((OPCODE >> 4) & 0xF)
#define BYTE(OPCODE) (OPCODE & 0xFF)
#define BZ(OPCODE) (OPCODE >> 12) 

void
op_reset(void)
{
        g_opcode = 0;
        srand(time(NULL));
}

opcode_t
op_get(void)
{
        return g_opcode;
}

static opcode_t
op_read(uint16_t pc)
{
        if (pc >= MEMORY_SIZE)
                dbg_err("[opcode.c: op_read] program counter bigger than memory");

        return mem_get(pc) << 8 | mem_get(pc + 1);
}

static void
op_empty(void)
{
        dbg_err("[opcode.c: op_empty] unknown instruction %x\n", g_opcode);
}

static void
op_sys(void)
{
        reg_set_pc(ADDR(g_opcode));
}

static void
op_cls(void)
{
}

static void
op_ret(void)
{
        reg_set_pc(stc_top());
        stc_pop();
}

static void
op_jmp(void)
{
        reg_set_pc(ADDR(g_opcode));
}

static void
op_call(void)
{
        stc_push(reg_get_pc());
        reg_set_pc(ADDR(g_opcode));
}

static void
op_sev(void)
{
        reg_t reg_val = reg_get(BX(g_opcode));
        uint8_t val = BYTE(g_opcode); 

        if (reg_val == val)
                reg_set_pc(reg_get_pc() + 2);
}

static void
op_snev(void)
{
        reg_t reg_val = reg_get(BX(g_opcode));
        uint8_t val = BYTE(g_opcode); 
        
        if (reg_val != val)
                reg_set_pc(reg_get_pc() + 2);
}

static void
op_ser(void)
{
        if (NIB(g_opcode) != 0)
                op_empty();
        
        reg_t reg1_val = reg_get(BX(g_opcode));
        reg_t reg2_val = reg_get(BY(g_opcode));

        if (reg1_val == reg2_val)
                reg_set_pc(reg_get_pc() + 2);
}

static void
op_ldv(void)
{
        reg_set(BX(g_opcode), BYTE(g_opcode));
}

static void
op_addv(void)
{
        reg_t reg_val = reg_get(BX(g_opcode));
        reg_set(BX(g_opcode), reg_val + BYTE(g_opcode));
}

static void
op_ldr(void)
{
        reg_t reg2_val = reg_get(BY(g_opcode));
        reg_set(BX(g_opcode), reg2_val);
}

static void
op_or(void)
{
        reg_t reg1_val = reg_get(BX(g_opcode));
        reg_t reg2_val = reg_get(BY(g_opcode));
        reg_set(BX(g_opcode), reg1_val | reg2_val);
}

static void
op_and(void)
{
        reg_t reg1_val = reg_get(BX(g_opcode));
        reg_t reg2_val = reg_get(BY(g_opcode));
        reg_set(BX(g_opcode), reg1_val & reg2_val);
}

static void
op_xor(void)
{
        reg_t reg1_val = reg_get(BX(g_opcode));
        reg_t reg2_val = reg_get(BY(g_opcode));
        reg_set(BX(g_opcode), reg1_val ^ reg2_val);
}

static void
op_addc(void)
{
        reg_t reg1_val = reg_get(BX(g_opcode));
        reg_t reg2_val = reg_get(BY(g_opcode));
        uint16_t sum = reg1_val + reg2_val;
        reg_set(BX(g_opcode), sum & 0xFF);
        reg_set(FLAG_REG, sum > 0xFF);
}

static void
op_sub(void)
{
        reg_t reg1_val = reg_get(BX(g_opcode));
        reg_t reg2_val = reg_get(BY(g_opcode));
        reg_set(BX(g_opcode), reg1_val - reg2_val);
        reg_set(FLAG_REG, reg1_val > reg2_val);
}

static void
op_shr(void)
{
        reg_t reg_val = reg_get(BX(g_opcode));
        reg_set(BX(g_opcode), reg_val >> 1);
        reg_set(FLAG_REG, reg_val & 0x1);
}

static void
op_subn(void)
{
        reg_t reg1_val = reg_get(BX(g_opcode));
        reg_t reg2_val = reg_get(BY(g_opcode));
        reg_set(BX(g_opcode), reg2_val - reg1_val);
        reg_set(FLAG_REG, reg2_val > reg1_val);
}

static void
op_shl(void)
{
        reg_t reg_val = reg_get(BX(g_opcode));
        reg_set(BX(g_opcode), reg_val << 1);
        reg_set(FLAG_REG, (reg_val >> 7) & 0x1);
}

static void
op_sner(void)
{
        if (NIB(g_opcode) != 0)
                op_empty();

        reg_t reg1_val = reg_get(BX(g_opcode));
        reg_t reg2_val = reg_get(BY(g_opcode)); 
        
        if (reg1_val != reg2_val)
                reg_set_pc(reg_get_pc() + 2);
}        

static void
op_ldi(void)
{
        reg_set_idx(ADDR(g_opcode));
}

static void
op_jmpr(void)
{
        uint16_t addr = reg_get(0) + ADDR(g_opcode);
        reg_set_pc(addr);
}

static void
op_rnd(void)
{
        uint16_t val = BYTE(g_opcode) & rand();
        reg_set(BX(g_opcode), val);
}

static void
op_inst0(void)
{
        if (ADDR(g_opcode) >= PROGRAM_START) {
                op_sys();
                return;
        }

        if (g_opcode == 0x00E0)
                op_cls();
        else if (g_opcode == 0x00EE)
                op_ret();
        else
                op_empty();
}

static op_func_t inst8_func_table[] = {
        op_ldr,
        op_or,
        op_and,
        op_xor,
        op_addc,
        op_sub,
        op_shr,
        op_subn,
        op_empty, op_empty, op_empty, op_empty, op_empty, op_empty,
        op_shl,
        op_empty,
};

static void
op_inst8(void)
{
        op_func_t op_func = inst8_func_table[NIB(g_opcode)];
        op_func();
}

static op_func_t main_func_table[] = {
        op_inst0,
        op_jmp,
        op_call,
        op_sev,
        op_snev,
        op_ser,
        op_ldv,
        op_addv,
        op_inst8,
        op_sner,
        op_ldi,
        op_jmpr,
        op_rnd,
};

void
op_nxt(void)
{
        g_opcode = op_read(reg_get_pc());
        op_exec();
}

void
op_set(opcode_t op)
{
        g_opcode = op;
}

void
op_exec(void)
{
        op_func_t op_func = main_func_table[BZ(g_opcode)];
        op_func();
}
