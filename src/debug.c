#include <stdarg.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "memory.h"
#include "registers.h"
#include "common.h"
#include "stack.h"
#include "opcode.h"
#include "system.h"

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

#define TEST_CHECK(N, M, CONDITION) if (!(CONDITION)) { \
                                     fprintf(stderr, RED BOLD "ERROR: " RESET); \
                                     fprintf(stderr, "test %d.%d\n", N, M); \
                                     exit(1); \
                                 } else { \
                                     fprintf(stderr, GREEN BOLD "PASSED " RESET); \
                                     fprintf(stderr, "test %d.%d\n", N, M); \
                                     err_code = 0; }

static int err_code = 0;

void
dbg_err(const char *format, ...)
{
#ifdef TEST

        (void) format;
        err_code = 1;
        
#else 
        
        va_list args;
        va_start(args, format);

        fprintf(stderr, RED BOLD "ERROR: " RESET); 
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");

        va_end(args);
        exit(1);

#endif
}

void
dbg_test(void)
{
        /* #1 MEMORY */
        
        /* #1.1: try to get out of bound memory */
        mem_get(MEMORY_SIZE);
        TEST_CHECK(1, 1, err_code);
        
        /* #1.2: try to set out of bound memory */
        mem_set(MEMORY_SIZE, 1);
        TEST_CHECK(1, 2, err_code);

        /* #1.3: set and get memory */
        mem_set(MEMORY_SIZE - 1, 1);
        TEST_CHECK(1, 3, mem_get(MEMORY_SIZE - 1) == 1);

        /* #2 REGISTERS */

        /* #2.1: try to get out of bound register */
        reg_get(REGS_COUNT);
        TEST_CHECK(2, 1, err_code);

        /* #2.2: try to set out of bound register */
        reg_set(REGS_COUNT, 1);
        TEST_CHECK(2, 2, err_code);

        /* #1.3: set and get register */
        reg_set(REGS_COUNT - 1, 1);
        TEST_CHECK(2, 3, reg_get(REGS_COUNT - 1) == 1);

        /* #3 STACK */

        /* #3.1: try to overflow stack */
        for (int i = 0; i < (STACK_SIZE + 1); ++i)
                stc_push(1);
        TEST_CHECK(3, 1, err_code);
        stc_reset();

        /* #3.2: try to underflow stack */
        stc_pop();
        TEST_CHECK(3, 2, err_code);
        stc_reset();

        /* #3.3: push, check value */
        stc_push(1);
        TEST_CHECK(3, 3, stc_top() == 1);

        /* #3.4: pop, check value */
        stc_pop();
        TEST_CHECK(3, 4, stc_top() == 0);
}

void
dbg_test_init(void)
{
        /* #101 INIT */

        /* #101.1 check if memory is empty apart from fontset */
        size_t size = MEMORY_SIZE - FONTSET_SIZE; 
        char *tmp_buf = calloc(size, sizeof(uint8_t));
        TEST_CHECK(101, 1, !memcmp(tmp_buf, mem_ptr() + FONTSET_SIZE, size));
        free(tmp_buf);
        
        /* #101.2-17 check if registers are empty */
        for (int i = 0; i < REGS_COUNT; ++i)
                TEST_CHECK(101, i + 2, !reg_get(i));

        /* #101.18 check if stack ptr is 0 */
        TEST_CHECK(101, 18, stc_get_ptr() == -1);
        
        /* #101.19 check if stack bottom is empty */
        TEST_CHECK(101, 19, stc_top() == 0);
}

void
dbg_test_inst(void)
{
        /* #200 INSTRUCTIONS */

        /* #201 SYS (0x0NNN) */
        
        /* #201.1 check if SYS works */
        op_set(OP_SYS(0x400));
        op_exec();
        TEST_CHECK(201, 1, reg_get_pc() == 0x400);
        sys_reset();

        /* #202 JMP (0x1NNN) */
        
        /* #202.1 check if JMP works */
        op_set(OP_JMP(0x400));
        op_exec();
        TEST_CHECK(202, 1, reg_get_pc() == 0x400);
        sys_reset();

        /* #203 CALL (0x2NNN) */
        
        /* #203.1 check if CALL works */
        uint16_t cur_pc = reg_get_pc();
        op_set(OP_CALL(0x400));
        op_exec();
        TEST_CHECK(203, 1, reg_get_pc() == 0x400 && stc_top() == cur_pc);
        sys_reset();
        
        /* #204 RET (0x00EE) */
        
        /* #204.1 check if RET works */
        op_set(OP_CALL(0x400));
        op_exec();

        op_set(OP_RET);
        op_exec();
        TEST_CHECK(204, 1, reg_get_pc() == 0x200 && stc_get_ptr() == -1);
        sys_reset();

        /* #205 SEV (0x3XKK) */
        
        /* #205.1 check if SEV works */
        cur_pc = reg_get_pc();
        op_set(OP_SEV(1, 0));
        op_exec();
        TEST_CHECK(205, 1, reg_get_pc() == cur_pc + 2);
        sys_reset();

        /* #205.2 check if SEV fails */
        cur_pc = reg_get_pc();
        op_set(OP_SEV(1, 1));
        op_exec();
        TEST_CHECK(205, 2, reg_get_pc() == cur_pc);
        sys_reset();

        /* #206 SNE (0x4XKK) */

        /* #206.1 check if SNE works */
        cur_pc = reg_get_pc();
        op_set(OP_SNE(1, 1));
        op_exec();
        TEST_CHECK(206, 1, reg_get_pc() == cur_pc + 2);
        sys_reset();

        /* #206.2 check if SNE fails */
        cur_pc = reg_get_pc();
        op_set(OP_SNE(1, 0));
        op_exec();
        TEST_CHECK(206, 2, reg_get_pc() == cur_pc);
        sys_reset();

        /* #207 LDV (0x6XKK) */

        /* #207.1 check if LDV works */
        op_set(OP_LDV(1, 1));
        op_exec();
        TEST_CHECK(207, 1, reg_get(1) == 1);
        sys_reset();

        /* #207.2 load a out of bound value */
        op_set(OP_LDV(1, 0x1234));
        op_exec();
        TEST_CHECK(207, 2, reg_get(1) == 0x34);
        sys_reset();
        
        /* #208 SER (0x5XY0) */

        /* #208.1 check if SER works */
        cur_pc = reg_get_pc();
        op_set(OP_SER(0, 1));
        op_exec();
        TEST_CHECK(208, 1, reg_get_pc() == cur_pc + 2);
        sys_reset();

        /* #208.2 check if SER fails */
        cur_pc = reg_get_pc();
        op_set(OP_LDV(1, 1));
        op_exec();
        op_set(OP_SER(0, 1));
        op_exec();
        TEST_CHECK(208, 2, reg_get_pc() == cur_pc);
        sys_reset();

        /* #209 ADDV (0x7XKK) */

        /* #209.1 check if ADDV works */
        op_set(OP_LDV(1, 3));
        op_exec();
        op_set(OP_ADDV(1, 5));
        op_exec();
        TEST_CHECK(209, 1, reg_get(1) == 8);
        sys_reset();
        
        /* #209.2 add out of bound value */
        op_set(OP_LDV(1, -1));
        op_exec();
        op_set(OP_ADDV(1, 2));
        op_exec();
        TEST_CHECK(209, 2, reg_get(1) == 1);
        sys_reset();

        /* 210 LDR (0x8XY0) */

        /* #210.1 check if LDR works */
        op_set(OP_LDV(1, 5));
        op_exec();
        op_set(OP_LDR(2, 1));
        op_exec();
        TEST_CHECK(210, 1, reg_get(2) == 5);
        sys_reset();

        /* 211 OR (0x8XY1) */
        
        /* #211.1 check if OR works */
        op_set(OP_LDV(1, 0x21));
        op_exec();
        op_set(OP_LDV(2, 0x42));
        op_exec();
        op_set(OP_OR(2, 1));
        op_exec();
        TEST_CHECK(211, 1, reg_get(2) == 0x63);
        sys_reset();

        /* 212 AND (0x8XY2) */

        /* 212.1 check if AND works */
        op_set(OP_LDV(1, 0x21));
        op_exec();
        op_set(OP_LDV(2, 0x60));
        op_exec();
        op_set(OP_AND(2, 1));
        op_exec();
        TEST_CHECK(212, 1, reg_get(2) == 0x20);
        sys_reset();

        /* 213 XOR (0x8XY3) */

        /* 213.1 check if XOR works */
        op_set(OP_LDV(1, 0x21));
        op_exec();
        op_set(OP_LDV(2, 0x60));
        op_exec();
        op_set(OP_XOR(2, 1));
        op_exec();
        TEST_CHECK(213, 1, reg_get(2) == 0x41);
        sys_reset();

        /* 214 ADDC (0x8XY4) */

        /* 214.1 check if ADDC works */
        op_set(OP_LDV(1, 0xFE));
        op_exec();
        op_set(OP_LDV(2, 0x1));
        op_exec();
        op_set(OP_ADDC(2, 1));
        op_exec();
        TEST_CHECK(214, 1, reg_get(2) == 0xFF && reg_get(0xF) == 0);
        sys_reset();

        /* 214.2 check if ADDC carry works */
        op_set(OP_LDV(1, 0xFE));
        op_exec();
        op_set(OP_LDV(2, 0xFC));
        op_exec();
        op_set(OP_ADDC(2, 1));
        op_exec();
        TEST_CHECK(214, 2, reg_get(2) == 0xFA && reg_get(0xF) == 1);
        sys_reset();

        /* 215 SUB (0x8XY5) */

        /* 215.1 check if SUB works */
        op_set(OP_LDV(1, 2));
        op_exec();
        op_set(OP_LDV(2, 3));
        op_exec();
        op_set(OP_SUB(2, 1));
        op_exec();
        TEST_CHECK(215, 1, reg_get(2) == 1 && reg_get(0xF) == 1);
        sys_reset();

        /* 215.2 check if SUB borrow works */
        op_set(OP_LDV(1, 3));
        op_exec();
        op_set(OP_LDV(2, 2));
        op_exec();
        op_set(OP_SUB(2, 1));
        op_exec();
        TEST_CHECK(215, 2, reg_get(2) == 0xFF && reg_get(0xF) == 0);
        sys_reset();

        
}

void
dbg_show_regs(void)
{
        fprintf(stderr, "PC: %#6x\n", reg_get_pc());
        fprintf(stderr, "ID: %#6x\n", reg_get_idx());
        fprintf(stderr, "OP: %#6x\n", op_get());
        fprintf(stderr, "ST: %#6x\n", stc_get_ptr());
        fprintf(stderr, "STACK TOP: %#6x\n", stc_top());
        
        for (int i = 0; i < REGS_COUNT; ++i) {
                fprintf(stderr, "V%2d: %#4x ", i, reg_get(i));

                if ((i + 1) % 4 == 0)
                        fprintf(stderr, "\n");
        }
}
