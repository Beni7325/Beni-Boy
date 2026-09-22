#include <stddef.h>
#include "gb.h"
#include "interrupts.h"


int init_gb(gb_console *gb, char *rom_name) {

    init_mem(&gb->mem);
    if (load_rom(&gb->cart, rom_name)) {
        return 1;
    }
    init_cpu(&gb->cpu);
    init_timer(&gb->timer);
    init_ppu(&gb->ppu);
    gb->joypad_reg = 0xFF;
    for (int m=0; m<8; ++m) {
        gb->keys[m] = 0;
    }

    return 0;
}

void free_gb(gb_console *gb) {
    free_mem(&gb->mem);
    free_ppu(&gb->ppu);
    free_cartridge(&gb->cart);
}

// Runs one frame's worth of cycles, ticking every component once per cycle.
void run_frame(gb_console *gb) {

    for (size_t c=0; c<GB_CYCLES_PER_FRAME; ++c) {
        update_timer(gb);
        execute_cpu_cycle(gb);
        tick_ppu(gb);
    }

}

// The joypad interrupt is only requested when a key goes from released to pressed.
void press_key(gb_console *gb, gb_key key) {
    if (!gb->keys[key]) {
        joypad_interrupt(gb);
    }
    gb->keys[key] = 1;
}

void release_key(gb_console *gb, gb_key key) {
    gb->keys[key] = 0;
}
