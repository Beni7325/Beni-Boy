#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define GB_CLOCK_HZ 4194304

typedef struct gb_console gb_console;


typedef struct {

    union {
        uint16_t cycles;
        struct {
            uint8_t div_lo;
            uint8_t div;
        };
    };

    uint8_t tima;
    uint8_t tma;
    uint8_t tac;

    uint8_t cycles_since_last_update;

} gb_timer;

void init_timer(gb_timer *timer);

void update_timer(gb_console *gb);

#endif /* TIMER_H */
