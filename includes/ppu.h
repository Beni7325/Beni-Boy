#ifndef PPU_H
#define PPU_H

#include <stdint.h>
#include "gb.h"

#define WIDTH 160
#define HEIGHT 144

typedef struct GB GB;


typedef enum {
    HBLANK = 0,
    VBLANK = 1,
    OAM_SEARCH = 2,
    DATA_TRANSFER = 3,
    OAM_BUG = 4
} PPU_State;


typedef struct {

    PPU_State state;

    uint32_t *screen;
    uint8_t curr_pxl;
    uint16_t ticks;

    uint8_t lcdc;
    uint8_t stat;

    uint8_t scy;
    uint8_t scx;
    uint8_t ly;
    uint8_t lyc;
    uint8_t wy;
    uint8_t wx;

    uint8_t bgp;
    uint8_t obp0;
    uint8_t obp1;

    uint8_t stat_cond_met;
    uint8_t window_line_counter;
    uint8_t screen_off;

} PPU;


void init_ppu(PPU *ppu);

void free_ppu(PPU *ppu);

void tick_ppu(GB *gb);

#endif /* PPU_H */