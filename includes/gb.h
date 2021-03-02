#ifndef GB_H
#define GB_H

#include "cpu.h"
#include "memory.h"
#include "cartridge.h"


typedef struct GB {
    Cpu cpu;
    Memory mem;
    Cartridge cart;
} GB;


int init_gb(GB *gb, char *rom_name);

void free_gb(GB *gb);

#endif /* GB_H */
