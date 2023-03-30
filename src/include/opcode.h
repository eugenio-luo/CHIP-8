#ifndef _OPCODE_H
#define _OPCODE_H

#include "common.h"

void op_reset(void);
opcode_t op_get(void);
void op_set(opcode_t op);
void op_nxt(void);
void op_exec(void);

#define OP_SYS(ADDR)            (ADDR & 0xFFF)
/* TODO: tests for OP_CLS */
#define OP_CLS                   0x00E0 
#define OP_RET                   0x00EE
#define OP_JMP(ADDR)            ((ADDR & 0xFFF) | 0x1000)
#define OP_CALL(ADDR)           ((ADDR & 0xFFF) | 0x2000)
#define OP_SEV(REG, VAL)        (((REG & 0xF) << 8) | (VAL & 0xFF) | 0x3000)
#define OP_SNEV(REG, VAL)       (((REG & 0xF) << 8) | (VAL & 0xFF) | 0x4000)
#define OP_SER(REG1, REG2)      (((REG1 & 0xF) << 8) | ((REG2 & 0xF) << 4) | 0x5000)
#define OP_LDV(REG, VAL)        (((REG & 0xF) << 8) | (VAL & 0xFF) | 0x6000)
#define OP_ADDV(REG, VAL)       (((REG & 0xF) << 8) | (VAL & 0xFF) | 0x7000)
#define OP_LDR(REG1, REG2)      (((REG1 & 0xF) << 8) | ((REG2 & 0xF) << 4) | 0x8000)
#define OP_OR(REG1, REG2)       (((REG1 & 0xF) << 8) | ((REG2 & 0xF) << 4) | 0x8001)
#define OP_AND(REG1, REG2)      (((REG1 & 0xF) << 8) | ((REG2 & 0xF) << 4) | 0x8002)
#define OP_XOR(REG1, REG2)      (((REG1 & 0xF) << 8) | ((REG2 & 0xF) << 4) | 0x8003)
#define OP_ADDC(REG1, REG2)     (((REG1 & 0xF) << 8) | ((REG2 & 0xF) << 4) | 0x8004)
#define OP_SUB(REG1, REG2)      (((REG1 & 0xF) << 8) | ((REG2 & 0xF) << 4) | 0x8005)
#define OP_SHR(REG1, REG2)      (((REG1 & 0xF) << 8) | ((REG2 & 0xF) << 4) | 0x8006)
#define OP_SUBN(REG1, REG2)     (((REG1 & 0xF) << 8) | ((REG2 & 0xF) << 4) | 0x8007)
#define OP_SHL(REG1, REG2)      (((REG1 & 0xF) << 8) | ((REG2 & 0xF) << 4) | 0x800E)
#define OP_SNER(REG1, REG2)     (((REG1 & 0xF) << 8) | ((REG2 & 0xF) << 4) | 0x9000)
#define OP_LDI(ADDR)            ((ADDR & 0xFFF) | 0xA000)
#define OP_JMPR(ADDR)           ((ADDR & 0xFFF) | 0xB000)
#define OP_RND(REG, VAL)        (((REG & 0xF) << 8) | (VAL & 0xFF) | 0xC000)
/* TODO: tests for OP_DRW */
#define OP_DRW(REG1, REG2, VAL) (((REG1 & 0xF) << 8) | ((REG2 & 0xF) << 4) | (VAL & 0xF) | 0xD000)
#define OP_SKP(REG)             (((REG & 0xF) << 8) | 0xE09E)
#define OP_SKNP(REG)            (((REG & 0xF) << 8) | 0xE0A1)
#define OP_LDRT(REG)            (((REG & 0xF) << 8) | 0xF007)

#endif
