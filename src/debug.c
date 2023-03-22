#include <stdarg.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "memory.h"
#include "registers.h"
#include "common.h"

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
        
        /* #101.2-18 check if registers are empty */
        for (int i = 0; i < REGS_COUNT; ++i)
                TEST_CHECK(101, i + 2, !reg_get(i));
}
