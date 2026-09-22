#ifndef PPU_H
#define PPU_H

#include <stdint.h>

typedef struct gb_console gb_console;

#define GB_SCREEN_WIDTH 160
#define GB_SCREEN_HEIGHT 144


typedef enum {
    GB_HBLANK = 0,
    GB_VBLANK = 1,
    GB_OAM_SEARCH = 2,
    GB_DATA_TRANSFER = 3,
    GB_OAM_BUG = 4
} gb_ppu_state;

typedef struct {
    uint8_t y;
    uint8_t x;
    uint8_t tile_no;
    uint8_t attributes;
} gb_sprite;

typedef struct {

    gb_ppu_state state;

    uint32_t *screen;
    uint32_t *lcd;
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


    // Array that contains the data of the 10 sprites in the current scanline.
    gb_sprite *sprites;
    // Number of sprites in the current scanline (Maximum of 10)
    uint8_t sprites_in_scanline;

} gb_ppu;


void init_ppu(gb_ppu *ppu);

void free_ppu(gb_ppu *ppu);

void tick_ppu(gb_console *gb);

#endif /* PPU_H */
