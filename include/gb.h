#ifndef GB_H
#define GB_H

#include "cpu.h"
#include "memory.h"
#include "timer.h"
#include "ppu.h"
#include "cartridge.h"

#define GB_CYCLES_PER_FRAME 70224

typedef enum {
    GB_KEY_UP=0, GB_KEY_DOWN, GB_KEY_LEFT, GB_KEY_RIGHT,
    GB_KEY_A, GB_KEY_B, GB_KEY_START, GB_KEY_SELECT
} gb_key;

typedef struct gb_console {
    gb_cpu cpu;
    gb_memory mem;
    gb_timer timer;
    gb_ppu ppu;
    gb_cartridge cart;
    uint8_t joypad_reg;
    uint8_t keys[8];
} gb_console;

int init_gb(gb_console *gb, char *rom_name);

void free_gb(gb_console *gb);

void run_frame(gb_console *gb);

void press_key(gb_console *gb, gb_key key);

void release_key(gb_console *gb, gb_key key);

#endif /* GB_H */
