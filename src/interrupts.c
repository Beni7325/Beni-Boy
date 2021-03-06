#include "./../includes/instructions.h"
#include "./../includes/interrupts.h"


 // Addresses where each instruction starts
uint16_t int_addresses[5] = {
    0x40, 0x48, 0x50, 0x58, 0x60
};


// Checks if any interrupt is being requested and executes it
void handle_interrupts(GB *gb) {

    uint8_t mask = 1;


    // Checks if an interrupt is being requested in order (from highest to lowest priority)
    // If an interrupt request bit is set, the current pc is pushed into the stack and
    // the new pc is set to the position in memory where the interrupt starts.
    // Interrupts are disabled (ime = 0) and the bit of the interrupt is cleared
    // This whole process takes 20 T cycles
    for (uint8_t i=0; i<5; ++i) {
        if (gb->cpu.int_flag & gb->mem.interr_enable_reg & mask) {
            push(gb, gb->cpu.pc);
            gb->cpu.pc = int_addresses[i];
            gb->cpu.ime = 0;
            gb->cpu.int_flag &= ~mask;
            gb->cpu.remaining_cycles = 20; // 5 M (20 T) cycles delay (? need to consult documentation)
            //printf("EXECUTING INT %02X\n", gb->cpu.pc);
            return;
        }
        mask <<= 1; // Shifts the bit so next iteration it checks a different interrupt
    }

}

// Requests a timer interrupt
void timer_interrupt(GB *gb) {
    gb->cpu.int_flag |= TIMER_INT;
}

// Requests a v blank interrupt
void v_blank_interrupt(GB *gb) {
    gb->cpu.int_flag |= V_BLANK_INT;
}

// Requests a stat interrupt
void stat_interrupt(GB *gb) {
    gb->cpu.int_flag |= LCD_STAT_INT;
}

// Requests a joypad interrupt
void joypad_interrupt(GB *gb) {
    gb->cpu.int_flag |= JOYPAD_INT;
}
