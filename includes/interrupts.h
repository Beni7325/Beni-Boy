#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "gb.h"

#define INTERR_FLAG 0xFF0F

#define V_BLANK_INT 0x01
#define LCD_STAT_INT 0x02
#define TIMER_INT 0x04
#define SERIAL_INT 0x08
#define JOYPAD_INT 0x10


void handle_interrupts(GB *gb);

void timer_interrupt(GB *gb);

void v_blank_interrupt(GB *gb);

void stat_interrupt(GB *gb);

void joypad_interrupt(GB *gb);

#endif /* INTERRUPTS_H */
