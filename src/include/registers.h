#ifndef _REGISTERS_H
#define _REGISTERS_H

void reg_reset(void);
uint16_t reg_get(int idx);
void reg_set(int idx, uint16_t val);

#endif
