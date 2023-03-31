#include <string.h>

#include "stack.h"
#include "common.h"
#include "debug.h"

int16_t g_stack_ptr; // stack pointer
uint16_t g_stack[STACK_SIZE];

void
stc_reset(void)
{
        g_stack_ptr = -1;
        memset(g_stack, 0, STACK_SIZE * sizeof(uint16_t));
}

int16_t
stc_get_ptr(void)
{
        return g_stack_ptr;
}

void
stc_push(uint16_t val)
{
        if (g_stack_ptr >= STACK_SIZE)
                dbg_err("[stack.c: stc_push] stack overflow");
        
        g_stack[++g_stack_ptr] = val;
}

void
stc_pop(void)
{
        if (g_stack_ptr < 0)
                dbg_err("[stack.c: stc_pop] stack underflow");

        --g_stack_ptr;
}

uint16_t
stc_top(void)
{
        if (g_stack_ptr == -1)
                return 0;
        
        return g_stack[g_stack_ptr];
}
