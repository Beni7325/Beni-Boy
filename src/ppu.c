#include "ppu.h"
#include "gb.h"
#include "interrupts.h"

#include <stdlib.h>
#include <string.h>


static const uint32_t colors[4] = {0xFFFFFFFF,0xFFA9A9A9,0xFF696969,0xFF000000}; // BW
//static uint32_t colors[4] = {0xFFE8DED2,0xFFA3D2CA,0xFF5EAAA8,0xFF056676}; // Blue
//static uint32_t colors[4] = {0xFFFFC93C,0xFFFF9A3C,0xFFFF6F3C,0xFF155263};
//static uint32_t colors[4] = {0xFFeeaf61,0xFFee5d6c,0xFFce4993,0xFF6a0d83}; // Sunset
// static uint32_t colors[4] = {0xFFFFFDAF,0xFF00FAAC,0xFFFF3796,0xFF302387}; // C O L O R
//static uint32_t colors[4] = {0xFFF4F4F4,0xFF65eeb7,0xFFff5722,0xFF474744};
//static uint32_t colors[4] = {0xFFf4f4f8,0xFFfed766,0xFF2ab7ca,0xFFfe4a49};  // Beach towel colors

void init_ppu(gb_ppu *ppu) {

    ppu->lcdc = 0x91;
    ppu->stat = 0x82;
    ppu->scy = 0x00;
    ppu->scx = 0x00;
    ppu->ly = 0x00;
    ppu->lyc = 0xFF; // TODO: mirar a que esta inicializado
    ppu->bgp = 0xFC;
    ppu->obp0 = 0xFF;
    ppu->obp1 = 0xFF;
    ppu->wy = 0x00;
    ppu->wx = 0x00;

    ppu->state = GB_OAM_SEARCH;
    ppu->curr_pxl = 0;
    ppu->ticks = 0;

    ppu->screen = (uint32_t*)malloc(GB_SCREEN_WIDTH*GB_SCREEN_HEIGHT*sizeof(uint32_t));
    ppu->lcd = (uint32_t*)malloc(GB_SCREEN_WIDTH*GB_SCREEN_HEIGHT*sizeof(uint32_t));
    memset(ppu->lcd, 0x00, GB_SCREEN_WIDTH*GB_SCREEN_HEIGHT*sizeof(uint32_t));

    ppu->stat_cond_met = 0;
    ppu->window_line_counter = 0;

    ppu->sprites = (gb_sprite*)malloc(10*sizeof(gb_sprite));
    ppu->sprites_in_scanline = 0;

    ppu->screen_off = 1;

}

void free_ppu(gb_ppu *ppu) {
    free(ppu->screen);
    free(ppu->lcd);
    free(ppu->sprites);
}

static void swap_lcd(gb_ppu *ppu) {
    uint32_t *temp;
    temp = ppu->lcd;
    ppu->lcd = ppu->screen;
    ppu->screen = temp;
}

// It's a stable sort (Insertion Sort). This means that two sprites with the
// same priority remain in the same relative order as they were before the sorting.
static void sort_sprites_by_priority(gb_sprite *sprites, uint8_t n_sprites) {

    int16_t i, j;
    gb_sprite n;

    if (sprites == NULL) {
        return;
    }

    for (i=1; i<n_sprites; i++) {

        n = sprites[i];
        j = i - 1;

        while (sprites[j].x > n.x) {
            sprites[j+1] = sprites[j];
            j--;
            if (j<0) {
                break;
            }
        }

        sprites[j+1] = n;

    }

}

static void scan_oam_for_scanline(gb_console *gb) {

    gb_sprite tmp_sprite;

    // Check for the 10 sprites in the current line and store their "id"? in an array
    gb->ppu.sprites_in_scanline = 0;
    for (int i=0; i<40 && gb->ppu.sprites_in_scanline < 10; ++i) { // Theres at *MOST* 40 sprites in OAM

        if ((gb->ppu.ly >= ((int16_t)gb->mem.oam[i<<2] - 16)) && (gb->ppu.ly <= ((int16_t)gb->mem.oam[i<<2] - 1))) {
            if (!(gb->ppu.lcdc & 0x04) && (((16 + gb->ppu.ly) - gb->mem.oam[i<<2])>=8)) {
                continue;
            }
            tmp_sprite.y = gb->mem.oam[i<<2];
            tmp_sprite.x = gb->mem.oam[(i<<2) + 1];
            tmp_sprite.tile_no = gb->mem.oam[(i<<2) + 2];
            tmp_sprite.attributes = gb->mem.oam[(i<<2) + 3];

            gb->ppu.sprites[gb->ppu.sprites_in_scanline] = tmp_sprite;
            ++gb->ppu.sprites_in_scanline;
        }

    }

    sort_sprites_by_priority(gb->ppu.sprites, gb->ppu.sprites_in_scanline);

}

static uint8_t get_sprite_pixel(gb_console *gb, gb_sprite *s) {

    uint8_t x, y, tile_no, sprite_attribs, sprite_pxl, msc, lsc;

    x = (8 + gb->ppu.curr_pxl) - s->x;
    y = (16 + gb->ppu.ly) - s->y;
    tile_no = s->tile_no;
    sprite_attribs = s->attributes;

    if (sprite_attribs & 0x20) { // X flipped
        x = 7 - x;
    }

    if (!(gb->ppu.lcdc & 0x04)) {

        if (sprite_attribs & 0x40) { // Y flipped
            y = 7 - y;
        }

        lsc = gb->mem.vram[((tile_no)<<4) + 2*y];
        msc = gb->mem.vram[((tile_no)<<4) + 2*y + 1];
        sprite_pxl = ((msc >> (7 - x)) & 0x1) << 1 | ((lsc >> (7 - x)) & 0x1);
    } else {

        if (sprite_attribs & 0x40) { // Y flipped
            y = 15 - y;
        }

        if (y < 8) {

            lsc = gb->mem.vram[((tile_no & 0xFE)<<4) + 2*y];
            msc = gb->mem.vram[((tile_no & 0xFE)<<4) + 2*y + 1];
        } else {
            y -= 8;

            lsc = gb->mem.vram[((tile_no | 0x01)<<4) + 2*y];
            msc = gb->mem.vram[((tile_no | 0x01)<<4) + 2*y + 1];
        }

        sprite_pxl = ((msc >> (7 - x)) & 0x1) << 1 | ((lsc >> (7 - x)) & 0x1);

    }

    return sprite_pxl;

}

void tick_ppu(gb_console *gb) {

    uint8_t stat_cond_this_tick = 0;

    if (!(gb->ppu.lcdc & 0x80) && !gb->ppu.screen_off) {
        gb->ppu.screen_off = 1;
        memset(gb->ppu.screen, 0x00, GB_SCREEN_WIDTH*GB_SCREEN_HEIGHT*sizeof(uint32_t));
        swap_lcd(&gb->ppu);
        gb->ppu.ly = 0;
        gb->ppu.ticks = 0;
        gb->ppu.stat = (gb->ppu.stat & 0xFC) | GB_HBLANK;
        gb->ppu.state = GB_OAM_BUG;
        return;
    } else if (gb->ppu.screen_off) {
        if (gb->ppu.lcdc & 0x80) {
            gb->ppu.screen_off = 0;
        } else {
            return;
        }
    }

    // Checks if LY equals LYC
    if (gb->ppu.ly == gb->ppu.lyc) {
        gb->ppu.stat |= 0x04;   // Sets the LY=LYC coincidence flag
        if (gb->ppu.stat & 0x40) {  // If LY=LYC stat interrupt is enabled
            stat_cond_this_tick = 1;
        }
    } else { // Clears the flag if LY!=LYC
        gb->ppu.stat &= 0xFB;
    }

    uint8_t tile_no;
    uint8_t x, y, line, column, pxl, lsc,msc, tile_pattern_table_select, no_bg_win;
    uint32_t color;
    uint8_t sprite_id, sprite_attribs, sprite_pxl;

    switch (gb->ppu.state) {

        case GB_OAM_SEARCH:

            if (gb->ppu.stat & 0x20) {
                stat_cond_this_tick = 1;
            }

            if (gb->ppu.ticks == 0) {
                scan_oam_for_scanline(gb);
            } else if (gb->ppu.ticks == 79) {
                gb->ppu.state = GB_DATA_TRANSFER;
                gb->ppu.stat = (gb->ppu.stat & 0xFC) | GB_DATA_TRANSFER;
            }

            ++gb->ppu.ticks;

            break;

        case GB_OAM_BUG:

            if (gb->ppu.stat & 0x20) {
                stat_cond_this_tick = 1;
            }

            if (gb->ppu.ticks == 0) {
                scan_oam_for_scanline(gb);
            } else if (gb->ppu.ticks == 75) {
                gb->ppu.state = GB_DATA_TRANSFER;
                gb->ppu.stat = (gb->ppu.stat & 0xFC) | GB_DATA_TRANSFER;
                gb->ppu.ticks += 4;
            }

            ++gb->ppu.ticks;

            break;

        case GB_DATA_TRANSFER:

            // Screen turned off
            if (!(gb->ppu.lcdc & 0x80)) { // TODO: Ver como funciona correctamente lo de apagar la pantalla
                color = colors[0];
            } else {

                no_bg_win = 0;

                // If window display is enabled AND the pixel is inside the window
                if ((gb->ppu.lcdc & 0x20) && (gb->ppu.ly >= gb->ppu.wy) && (gb->ppu.curr_pxl >= gb->ppu.wx-7)) {
                    x = gb->ppu.curr_pxl - gb->ppu.wx + 7;
                    y = gb->ppu.window_line_counter;
                    if (gb->ppu.curr_pxl == 159) {
                        ++gb->ppu.window_line_counter;
                    }
                    tile_pattern_table_select = (gb->ppu.lcdc & 0x40) >> 6;
                } else if (!(gb->ppu.lcdc & 0x01)) { // No BG nor Window
                    color = colors[0];
                    pxl = 0;
                    no_bg_win = 1;
                } else {
                    x = gb->ppu.scx + gb->ppu.curr_pxl;
                    y = gb->ppu.scy + gb->ppu.ly;
                    tile_pattern_table_select = (gb->ppu.lcdc & 0x08) >> 3;
                }


                if (!no_bg_win) {

                    line = y & 0x7;
                    column = x & 0x7;

                    if (tile_pattern_table_select) { // 9c00 - 9ffff
                        tile_no = gb->mem.vram[0x1c00 + (((y>>3) << 5) + (x>>3))];
                    } else {    // 9800 - 9bff
                        tile_no = gb->mem.vram[0x1800 + (((y>>3) << 5) + (x>>3))];
                    }

                    if (gb->ppu.lcdc & 0x10) {
                        lsc = gb->mem.vram[(tile_no<<4) + 2*line];
                        msc = gb->mem.vram[(tile_no<<4) + 2*line + 1];
                    } else {
                        lsc = gb->mem.vram[0x1000 + (((int8_t)tile_no)<<4) + 2*line];
                        msc = gb->mem.vram[0x1000 + (((int8_t)tile_no)<<4) + 2*line + 1];
                    }

                    pxl = ((msc >> (7 - column)) & 0x1) << 1 | ((lsc >> (7 - column)) & 0x1);
                    color = colors[(gb->ppu.bgp >> (pxl<<1)) & 0x03];

                }

                // If there's sprites in this line AND sprites are enabled
                if ((gb->ppu.sprites_in_scanline > 0) && (gb->ppu.lcdc & 0x02)) {

                    sprite_id = 0xFF; // Initialized to 0xFF (No Sprite overlaps this pixel)
                    // See if one (or more) sprites overlap this pixel (checking x pos)
                    for (int i=0; i < gb->ppu.sprites_in_scanline; ++i) { // TODO <<1 or <<2 =
                        x = gb->ppu.sprites[i].x;
                        y = gb->ppu.sprites[i].y;
                        if ((gb->ppu.curr_pxl >= ((int16_t)x - 8)) && (gb->ppu.curr_pxl <= ((int16_t)x - 1))) {
                            sprite_pxl = get_sprite_pixel(gb, &gb->ppu.sprites[i]);
                            if (no_bg_win) {
                                if (sprite_pxl != 0) {
                                    sprite_id = i;
                                }
                                break;
                            }

                            if (sprite_pxl == 0) {
                                continue;
                            }

                            if (gb->ppu.sprites[i].attributes & 0x80) {
                                if (pxl == 0) {
                                    sprite_id = i;
                                    break;
                                }
                            } else {
                                sprite_id = i;
                                break;
                            }
                        }
                    }

                    if (sprite_id != 0xFF) {


                        sprite_attribs = gb->ppu.sprites[sprite_id].attributes;


                        if (sprite_attribs & 0x10) {
                            color = colors[(gb->ppu.obp1 >> (sprite_pxl<<1)) & 0x03];
                        } else {
                            color = colors[(gb->ppu.obp0 >> (sprite_pxl<<1)) & 0x03];
                        }

                    }

                }

            }
            gb->ppu.screen[gb->ppu.ly*GB_SCREEN_WIDTH + gb->ppu.curr_pxl] = color;

            ++gb->ppu.curr_pxl;
            if (gb->ppu.curr_pxl == GB_SCREEN_WIDTH) {
                gb->ppu.curr_pxl = 0;
                gb->ppu.state = GB_HBLANK;
                gb->ppu.stat = (gb->ppu.stat & 0xFC) | GB_HBLANK;
            }

            ++gb->ppu.ticks;

            break;

        case GB_HBLANK:

            if (gb->ppu.stat & 0x08) {
                stat_cond_this_tick = 1;
            }

            if (gb->ppu.ticks == 455) {
                ++gb->ppu.ly;
                gb->ppu.ticks = 0;
                if (gb->ppu.ly == GB_SCREEN_HEIGHT) {
                    gb->ppu.state = GB_VBLANK;
                    gb->ppu.stat = (gb->ppu.stat & 0xFC) | GB_VBLANK;

                    swap_lcd(&gb->ppu);

                    // Reset line counter for the window
                    gb->ppu.window_line_counter = 0;

                    v_blank_interrupt(gb);
                } else {
                    gb->ppu.state = GB_OAM_SEARCH;
                    gb->ppu.stat = (gb->ppu.stat & 0xFC) | GB_OAM_SEARCH;
                }
            } else {
                ++gb->ppu.ticks;
            }
            break;

        case GB_VBLANK:

            if (gb->ppu.stat & 0x10) {
                stat_cond_this_tick = 1;
            }

            if (gb->ppu.ticks == 455) {
                ++gb->ppu.ly;
                gb->ppu.ticks = 0;
                if (gb->ppu.ly == 154) {
                    gb->ppu.ly = 0;
                    gb->ppu.state = GB_OAM_SEARCH;
                    gb->ppu.stat = (gb->ppu.stat & 0xFC) | GB_OAM_SEARCH;
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
