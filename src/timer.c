#include "timer.h"
#include "gb.h"
#include "interrupts.h"


static const uint32_t tima_periods[4] = {GB_CLOCK_HZ/4096 - 1, GB_CLOCK_HZ/262144 - 1, GB_CLOCK_HZ/65536 - 1, GB_CLOCK_HZ/16384 - 1};

void init_timer(gb_timer *timer) {
    timer->cycles = 0x00;
    timer->tima = 0x00;
    timer->tma = 0x00;
    timer->tac = 0x00;
    timer->cycles_since_last_update = 0;
}

void update_timer(gb_console *gb) {

    // There's really no reason to update the cycles one by one, given that
    // the smallest period for the clock is 16 t-cycles
    if (gb->timer.cycles_since_last_update == 15) {
        gb->timer.cycles_since_last_update = 0;

        gb->timer.cycles += 16;

        if (gb->timer.tac & 0x04) {
            if (!(gb->timer.cycles & (tima_periods[gb->timer.tac & 0x03]))) {

                ++gb->timer.tima;

                if (!gb->timer.tima) {
                    gb->timer.tima = gb->timer.tma;
                    timer_interrupt(gb);
                }

            }
        }

    } else {
        ++gb->timer.cycles_since_last_update;
    }

}
