#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>

#define MEMORY_SIZE   4096
#define REGS_COUNT    16
#define SCREEN_HEIGHT 32
#define SCREEN_WIDTH  64
#define STACK_SIZE    16
#define KEYS_COUNT    16
#define PROGRAM_START 0x200
#define FONTSET_SIZE  0x50

typedef uint16_t opcode_t;
typedef uint8_t reg_t;

#endif
