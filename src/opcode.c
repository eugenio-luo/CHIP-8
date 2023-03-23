#include "opcode.h"
#include "common.h"
#include "debug.h"
#include "memory.h"
#include "registers.h"

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
}

static opcode_t
op_read(uint16_t pc)
{
        if (pc >= MEMORY_SIZE)
                dbg_err("[opcode.c: op_read] program counter bigger than memory");

        return mem_get(pc) << 8 | mem_get(pc + 1);
}

static void
op_sys(void)
{
        reg_set_pc(ADDR(g_opcode));
}

static void
op_cls(void)
{
        printf("CLS\n");
}

static void
op_ret(void)
{
        printf("RET\n");
}

static void
op_0inst(void)
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
                dbg_err("[opcode.c: op_0inst] unknown opcode %x\n", g_opcode);
}

static void
op_jmp(void)
{
        reg_set_pc(ADDR(g_opcode));
}

static void
op_call(void)
{
        printf("CALL %x\n", g_opcode & 0xFFF);
}

typedef void (*op_func_t)(void);
static op_func_t main_func_table[] = {
        op_0inst,
        op_jmp,
        op_call,
};

void
op_exec(void)
{
        g_opcode = op_read(reg_get_pc());
        
        op_func_t op_func = main_func_table[BZ(g_opcode)];
        op_func();
}
