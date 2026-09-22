#include "instructions.h"
#include "interrupts.h"


static const uint16_t int_addresses[5] = {
    0x40, 0x48, 0x50, 0x58, 0x60
};


void handle_interrupts(gb_console *gb) {

    uint8_t mask = 1;

    for (uint8_t i=0; i<5; ++i) {
        if (gb->cpu.int_flag & gb->mem.ie_reg & mask) {
            push(gb, gb->cpu.pc);
            gb->cpu.pc = int_addresses[i];
            gb->cpu.ime = 0;
            gb->cpu.int_flag &= ~mask;
            gb->cpu.remaining_cycles = 3; // 4 T cycles delay because of the PUSH
            //printf("EXECUTING INT %02X\n", gb->cpu.pc);
            return;
        }
        mask <<= 1;
    }

}

void timer_interrupt(gb_console *gb) {
    gb->cpu.int_flag |= GB_INT_TIMER;
}

void v_blank_interrupt(gb_console *gb) {
    gb->cpu.int_flag |= GB_INT_VBLANK;
}

void stat_interrupt(gb_console *gb) {
    gb->cpu.int_flag |= GB_INT_LCD_STAT;
}

void joypad_interrupt(gb_console *gb) {
    gb->cpu.int_flag |= GB_INT_JOYPAD;
}
