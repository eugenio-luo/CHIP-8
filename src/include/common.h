#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>

#define SCREEN_HEIGHT  32
#define SCREEN_WIDTH   64
#define DP_MULT        10
#define DP_HEIGHT      (SCREEN_HEIGHT * DP_MULT)
#define DP_WIDTH       (SCREEN_WIDTH * DP_MULT)

#define MCOLOR_R       0xFF
#define MCOLOR_G       0xFF
#define MCOLOR_B       0xFF

#define OCOLOR_R       0x0
#define OCOLOR_G       0x0
#define OCOLOR_B       0x0

#define PROG_NAME      "CHIP-8 emulator" 
#define MEMORY_SIZE    4096
#define REGS_COUNT     16
#define FLAG_REG       0xF
#define STACK_SIZE     16
#define KEYS_COUNT     16
#define PROGRAM_START  0x200
#define FONTSET_OFF    0x0
#define FONTSET_SIZE   0x50

#define TIMER_PERIOD   16

typedef uint16_t opcode_t;
typedef uint8_t reg_t;

#endif
