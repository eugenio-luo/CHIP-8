#include "opcode.h"
#include "common.h"

opcode_t g_opcode;

void
op_reset(void)
{
        g_opcode = 0;
}
