#include "./../includes/gb.h"
#include "./../includes/interrupts.h"
#include "./../includes/timer.h"


static uint32_t tima_periods[4] = {CLOCKS_PER_SEC/4096 - 1, CLOCKS_PER_SEC/262144 - 1, CLOCKS_PER_SEC/65536 - 1, CLOCKS_PER_SEC/16384 - 1};


void init_timer(Timer *timer) {
    timer->cycles = 0x00;
    timer->tima = 0x00;
    timer->tma = 0x00;
    timer->tac = 0x00;
    timer->cycles_since_last_update = 0;
}

void tick_timer(GB *gb) {
    
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