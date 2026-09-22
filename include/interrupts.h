#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "gb.h"

#define GB_INT_VBLANK 0x01
#define GB_INT_LCD_STAT 0x02
#define GB_INT_TIMER 0x04
#define GB_INT_SERIAL 0x08
#define GB_INT_JOYPAD 0x10


void handle_interrupts(gb_console *gb);

void timer_interrupt(gb_console *gb);

void v_blank_interrupt(gb_console *gb);

void stat_interrupt(gb_console *gb);

void joypad_interrupt(gb_console *gb);

#endif /* INTERRUPTS_H */
