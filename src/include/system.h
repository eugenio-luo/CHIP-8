#ifndef _SYSTEM_H
#define _SYSTEM_H

int sys_init(void);
int sys_reset(void);
void sys_load(const char *file_name);
void sys_cycle(void);
void sys_quit(void);

#endif
