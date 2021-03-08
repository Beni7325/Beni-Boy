#include <stdlib.h>
#include <string.h>
#include "./../includes/ppu.h"
#include "./../includes/gb.h"


#define LCDC_LCD_ENABLE         0x80
#define LCDC_WIN_TILEMAP        0x40
#define LCDC_WIN_ENABLE         0x20
#define LCDC_BG_WIN_TILEMAP     0x10
#define LCDC_BG_TILEMAP         0x08
#define LCDC_OBJ_SIZE           0x04
#define LCDC_OBJ_ENABLE         0x02
#define LCDC_BG_WIN_PRIORITY    0x01


// Color palette for the GB screen
static uint32_t colors[4] = {0xFFFFFFFF,0xFFA9A9A9,0xFF696969,0xFF000000};


void init_ppu(PPU *ppu) {

    ppu->lcdc = 0x91;
    ppu->stat = 0x82;
    ppu->scy = 0x00;
    ppu->scx = 0x00;
    ppu->ly = 0x00;
    ppu->lyc = 0xFF; // TODO: Check if it is really initialized to this
    ppu->bgp = 0xFC;
    ppu->obp0 = 0xFF;
    ppu->obp1 = 0xFF;
    ppu->wy = 0x00;
    ppu->wx = 0x00;

    ppu->state = OAM_SEARCH;
    ppu->curr_pxl = 0;
    ppu->ticks = 0;

    ppu->screen = (uint32_t*)malloc(WIDTH*HEIGHT*sizeof(uint32_t));
    memset(ppu->screen, 0x00, WIDTH*HEIGHT*sizeof(uint32_t));

    ppu->stat_cond_met = 0;
    ppu->window_line_counter = 0;

    ppu->screen_off = 1;

}

void free_ppu(PPU *ppu) {

    free(ppu->screen);

}

void tick_ppu(GB *gb) {
    
}
