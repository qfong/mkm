#ifndef HOOKS_NEW_H
#define HOOKS_NEW_H
void reg_hooks(unsigned long **syscall_table);
void unreg_hooks(unsigned long**syscall_table);
#endif
