#ifndef _REGISTERS_H
#define _REGISTERS_H

void reg_reset(void);
uint8_t reg_get(int idx);
void reg_set(int idx, uint8_t val);
uint16_t reg_get_pc(void);
void reg_inc_pc(void);
void reg_dec_pc(void);
void reg_set_pc(uint16_t addr);
uint16_t reg_get_idx(void);
void reg_set_idx(uint16_t addr);
void reg_inc_idx(void);

#endif
