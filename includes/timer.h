#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define CLOCKS_PER_SEC 4194304
#define DIV_FREQ 16384

typedef struct GB GB;


typedef struct {

    union {
        uint16_t cycles;
        struct {
            uint8_t none;
            uint8_t div;
        };
    };

    uint8_t tima;
    uint8_t tma;
    uint8_t tac;

    uint8_t cycles_since_last_update;

} Timer;


void init_timer(Timer *timer);

void tick_timer(GB *gb);

#endif /* TIMER_H */