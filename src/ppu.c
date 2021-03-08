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

#define STAT_COINC_INT          0x40
#define STAT_OAM_INT            0x20
#define STAT_VBLANK_INT         0x10
#define STAT_HBLANK_INT         0x08
#define STAT_COINC_FLAG         0x04
#define STAT_MODE_FLAG          0x03


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

    uint8_t stat_cond_this_tick = 0; // Will manage this later


    if (gb->ppu.ly == gb->ppu.lyc) {
        gb->ppu.stat |= STAT_COINC_FLAG;   // Sets the LY=LYC coincidence flag
        if (gb->ppu.stat & STAT_COINC_INT) {  // If LY=LYC stat interrupt is enabled
            stat_cond_this_tick = 1;
        }
    } else { // Clears the flag if LY!=LYC
        gb->ppu.stat &= 0xFB;
    }

    switch (gb->ppu.state) {

        case OAM_SEARCH:

            if (gb->ppu.stat & STAT_OAM_INT) {
                stat_cond_this_tick = 1;
            }

            if (gb->ppu.ticks == 0) {

                // TODO

            } else if (gb->ppu.ticks == 79) {
                gb->ppu.state = DATA_TRANSFER;
                gb->ppu.stat = (gb->ppu.stat & 0xFC) | DATA_TRANSFER;
            }

            ++gb->ppu.ticks;

            break;

        case OAM_BUG:

            if (gb->ppu.stat & STAT_OAM_INT) {
                stat_cond_this_tick = 1;
            }

            if (gb->ppu.ticks == 0) {

                // TODO

            } else if (gb->ppu.ticks == 75) {
                gb->ppu.state = DATA_TRANSFER;
                gb->ppu.stat = (gb->ppu.stat & 0xFC) | DATA_TRANSFER;
                gb->ppu.ticks += 4;
            }

            ++gb->ppu.ticks;

            break;

        case DATA_TRANSFER:

            // TODO

            ++gb->ppu.curr_pxl;
            if (gb->ppu.curr_pxl == WIDTH) {
                gb->ppu.curr_pxl = 0;
                gb->ppu.state = HBLANK;
                gb->ppu.stat = (gb->ppu.stat & 0xFC) | HBLANK;
            }

            ++gb->ppu.ticks;

            break;

        case HBLANK:

            if (gb->ppu.stat & STAT_HBLANK_INT) {
                stat_cond_this_tick = 1;
            }

            if (gb->ppu.ticks == 455) {
                ++gb->ppu.ly;
                gb->ppu.ticks = 0;
                if (gb->ppu.ly == HEIGHT) {
                    gb->ppu.state = VBLANK;
                    gb->ppu.stat = (gb->ppu.stat & 0xFC) | VBLANK;

                    // Reset line counter for the window
                    gb->ppu.window_line_counter = 0;

                    v_blank_interrupt(gb);

                } else {
                    gb->ppu.state = OAM_SEARCH;
                    gb->ppu.stat = (gb->ppu.stat & 0xFC) | OAM_SEARCH;
                }   
            } else {
                ++gb->ppu.ticks;
            }

            break;

        case VBLANK:

            if (gb->ppu.stat & STAT_VBLANK_INT) {
                stat_cond_this_tick = 1;
            }

            if (gb->ppu.ticks == 455) {
                ++gb->ppu.ly;
                gb->ppu.ticks = 0;
                if (gb->ppu.ly == 154) {
                    gb->ppu.ly = 0;
                    gb->ppu.state = OAM_SEARCH;
                    gb->ppu.stat = (gb->ppu.stat & 0xFC) | OAM_SEARCH;
                }   
            } else {
                ++gb->ppu.ticks;
            }
            break;

    }

    if (!gb->ppu.stat_cond_met && stat_cond_this_tick) {
        stat_interrupt(gb);
    }
    gb->ppu.stat_cond_met = stat_cond_this_tick;
    
}
