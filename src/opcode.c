#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "opcode.h"
#include "common.h"
#include "debug.h"
#include "memory.h"
#include "registers.h"
#include "stack.h"
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "fontset.h"

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
        scr_clear();
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
                reg_inc_pc();
}

static void
op_snev(void)
{
        reg_t reg_val = reg_get(BX(g_opcode));
        uint8_t val = BYTE(g_opcode); 
        
        if (reg_val != val)
                reg_inc_pc();
}

static void
op_ser(void)
{
        if (NIB(g_opcode) != 0)
                op_empty();
        
        reg_t reg1_val = reg_get(BX(g_opcode));
        reg_t reg2_val = reg_get(BY(g_opcode));

        if (reg1_val == reg2_val)
                reg_inc_pc();
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

        /* CHIP-8 quirk, flag register get reset by OR */
        reg_set(FLAG_REG, 0);
}

static void
op_and(void)
{
        reg_t reg1_val = reg_get(BX(g_opcode));
        reg_t reg2_val = reg_get(BY(g_opcode));
        reg_set(BX(g_opcode), reg1_val & reg2_val);

        /* CHIP-8 quirk, flag register get reset by AND */
        reg_set(FLAG_REG, 0);
}

static void
op_xor(void)
{
        reg_t reg1_val = reg_get(BX(g_opcode));
        reg_t reg2_val = reg_get(BY(g_opcode));
        reg_set(BX(g_opcode), reg1_val ^ reg2_val);

        /* CHIP-8 quirk, flag register get reset by XOR */
        reg_set(FLAG_REG, 0);
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
        /* in the original CHIP-8 interpreter, it put the VY register value
           in VX register, then shifted VX */
        
        reg_t reg_val = reg_get(BY(g_opcode));
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
        /* in the original CHIP-8 interpreter, it put the VY register value
           in VX register, then shifted VX */
        
        reg_t reg_val = reg_get(BY(g_opcode));
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
                reg_inc_pc();
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
op_drw(void)
{
        int w = reg_get(BX(g_opcode)) % SCREEN_WIDTH;
        int h = reg_get(BY(g_opcode)) % SCREEN_HEIGHT;
        int limit = NIB(g_opcode);
        uint16_t addr = reg_get_idx();

        usleep(18000);

        for (int i = 0; i < limit; ++i) {
                int byte = mem_get(addr + i);
                int y = (h + i) % SCREEN_HEIGHT; 

                /* the original CHIP-8 clipped the sprites written outside of the screen
                   instead of wrapping around */

                if (h + i >= SCREEN_HEIGHT)
                        break;
                
                for (int j = 0; j < 8; ++j) {
                        int x = (w + j) % SCREEN_WIDTH;

                        if (w + j >= SCREEN_WIDTH)
                                break;
                        
                        int pixel = scr_get(x, y);
                        int xor = ((byte >> (7 - j)) & 0x1) ^ pixel;

                        scr_set(x, y, xor);
                        if (pixel && !xor)
                                reg_set(FLAG_REG, 1);
                }

        }

        scr_render();
}

static void
op_skp(void)
{
        reg_t reg_val = reg_get(BX(g_opcode));

        if (key_get(reg_val))
                reg_inc_pc();
}

static void
op_sknp(void)
{
        reg_t reg_val = reg_get(BX(g_opcode));

        if (!key_get(reg_val))
                reg_inc_pc();
}

static void
op_ldrt(void)
{
        reg_set(BX(g_opcode), tme_get_del());
}

static void
op_ldk(void)
{
        reg_set(BX(g_opcode), key_wait());
        reg_inc_pc();
}

static void
op_ldtr(void)
{
        reg_t reg_val = reg_get(BX(g_opcode));
        tme_set_del(reg_val);
}

static void
op_ldsr(void)
{
        reg_t reg_val = reg_get(BX(g_opcode));
        tme_set_snd(reg_val);
}

static void
op_addi(void)
{
        reg_t reg_val = reg_get(BX(g_opcode));
        reg_set_idx(reg_get_idx() + reg_val);
}

static void
op_ldf(void)
{
        reg_t reg_val = reg_get(BX(g_opcode));
        reg_set_idx(FONT_ADDR(reg_val));
}

static void
op_ldb(void)
{
        reg_t reg_val = reg_get(BX(g_opcode));
        uint16_t addr = reg_get_idx();
        
        for (int i = 2; i >= 0; --i) {
                mem_set(addr + i, reg_val % 10);
                reg_val /= 10;
        }
}        

static void
op_ldir(void)
{
        uint16_t addr = reg_get_idx();

        for (int i = 0; i <= BX(g_opcode); ++i) {
                mem_set(addr + i, reg_get(i));

                /* the correct behaviour is that the index 
                   register is incremented too */
                reg_inc_idx();
        }
}

static void
op_ldri(void)
{
        uint16_t addr = reg_get_idx();
        
        for (int i = 0; i <= BX(g_opcode); ++i) {
                reg_set(i, mem_get(addr + i));

                /* the correct behaviour is that the index 
                   register is incremented too */
                reg_inc_idx();
        }
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

static void
op_instE(void)
{
        if (BYTE(g_opcode) == 0x9E)
                op_skp();
        else if (BYTE(g_opcode) == 0xA1)
                op_sknp();
        else
                op_empty();
}

static void
op_instF(void)
{
        switch (BYTE(g_opcode)) {
        case 0x07:
                op_ldrt();
                break;

        case 0x0A:
                op_ldk();
                break;

        case 0x15:
                op_ldtr();
                break;

        case 0x18:
                op_ldsr();
                break;

        case 0x1E:
                op_addi();
                break;

        case 0x29:
                op_ldf();
                break;

        case 0x33:
                op_ldb();
                break;

        case 0x55:
                op_ldir();
                break;

        case 0x65:
                op_ldri();
                break;
                
        default:
                op_empty();
        }
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
        op_drw,
        op_instE,
        op_instF,
};

void
op_nxt(void)
{
        /* the program counter should be incremented BEFORE executing the instruction */
        
        g_opcode = op_read(reg_get_pc());
        reg_inc_pc();

        dbg_log("executing: %x", g_opcode);
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
