#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>

void mem_reset(void);
void mem_ld_font(void);
uint8_t *mem_ptr(void);
uint8_t mem_get(int off);
void mem_set(int off, uint8_t val);

#endif
