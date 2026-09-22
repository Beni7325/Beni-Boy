#include "memory.h"
#include "gb.h"
#include "ppu.h"
#include "cartridge.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_mem(gb_memory *mem) {

    mem->vram         = (uint8_t*)calloc(0x2000, sizeof(uint8_t));
    mem->wram_0       = (uint8_t*)calloc(0x1000, sizeof(uint8_t));
    mem->wram_1       = (uint8_t*)calloc(0x1000, sizeof(uint8_t));
    mem->oam          = (uint8_t*)calloc(0x00A0, sizeof(uint8_t));
    mem->unusable     = (uint8_t*)calloc(0x0060, sizeof(uint8_t));
    mem->io           = (uint8_t*)calloc(0x0080, sizeof(uint8_t));
    mem->hram         = (uint8_t*)calloc(0x007F, sizeof(uint8_t));

    for (int i=0; i<0x80; ++i) {
        mem->io[i] = 0xFF;
    }

    mem->ie_reg = 0x00;

}

void free_mem(gb_memory *mem) {

    free(mem->vram);
    free(mem->wram_0);
    free(mem->wram_1);
    free(mem->oam);
    free(mem->unusable);
    free(mem->io);
    free(mem->hram);

}

/**
 +  Function that returns a pointer of the memory with the corresponding high nibble (From 0x00 to 0xF1)
 *  Its used *ONLY* to get a src pointer for memcpy for a DMA transfer
 */
static uint8_t *get_ptr_at_high_nibble(gb_console *gb, uint8_t high_addr_nibble) {

    if (high_addr_nibble > 0xF1) {
        return NULL;
    }

    switch ((high_addr_nibble >> 4)) {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
            return gb->cart.rom_bank_00 + (high_addr_nibble << 8);
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
            return gb->cart.rom_bank_n + ((high_addr_nibble << 8) - 0x4000);
        case 0x8:
        case 0x9:
            return gb->mem.vram + ((high_addr_nibble << 8) - 0x8000);
        case 0xA:
        case 0xB:
            if (gb->cart.external_ram_bank == NULL) {
                return NULL;
            }
            return gb->cart.external_ram_bank + ((high_addr_nibble << 8) - 0xA000);
        case 0xC:
            return gb->mem.wram_0 + ((high_addr_nibble << 8) - 0xC000);
        case 0xD:
            return gb->mem.wram_1 + ((high_addr_nibble << 8) - 0xD000);
        case 0xE:
            return gb->mem.wram_0 + ((high_addr_nibble << 8) - 0xE000);
        case 0xF:
            return gb->mem.wram_1 + ((high_addr_nibble << 8) - 0xF000);
    }
    return NULL;
}

void write_byte(gb_console *gb, uint16_t addr, uint8_t data) {

    uint8_t *src, key_inputs = 0x0F;

    if (addr < 0x8000) {
        gb->cart.mbc_write(&gb->cart, addr, data);
    } else if (addr < 0xA000) {
        gb->mem.vram[addr - 0x8000] = data;
    } else if (addr < 0xC000) {
        if (gb->cart.external_ram_bank == NULL) {
            return;
        }
        gb->cart.external_ram_bank[addr - 0xA000] = data;
    } else if (addr < 0xD000) {
        gb->mem.wram_0[addr - 0xC000] = data;
    } else if (addr < 0xE000) {
        gb->mem.wram_1[addr - 0xD000] = data;
    } else if (addr < 0xFE00) {
        if (addr < 0xF000) {
            gb->mem.wram_0[addr - 0xE000] = data;
        } else {
            gb->mem.wram_1[addr - 0xF000] = data;
        }
    } else if (addr < 0xFEA0) {
        gb->mem.oam[addr - 0xFE00] = data;
    } else if (addr < 0xFF00) {
        gb->mem.unusable[addr - 0xFEA0] = data;
    } else if (addr < 0xFF80) {
        switch (addr) {
            case GB_REG_P1:

                switch ((data & 0x30) >> 4) {

                    case 0x1: // Buttons

                        if (gb->keys[GB_KEY_START]) {
                            key_inputs &= 0xF7;
                        }

                        if (gb->keys[GB_KEY_SELECT]) {
                            key_inputs &= 0xFB;
                        }

                        if (gb->keys[GB_KEY_B]) {
                            key_inputs &= 0xFD;
                        }

                        if (gb->keys[GB_KEY_A]) {
                            key_inputs &= 0xFE;
                        }

                        break;

                    case 0x2: // Direction keys

                        if (gb->keys[GB_KEY_DOWN]) {
                            key_inputs &= 0xF7;
                        }

                        if (gb->keys[GB_KEY_UP]) {
                            key_inputs &= 0xFB;
                        }

                        if (gb->keys[GB_KEY_LEFT]) {
                            key_inputs &= 0xFD;
                        }

                        if (gb->keys[GB_KEY_RIGHT]) {
                            key_inputs &= 0xFE;
                        }

                        break;

                    //case 0x3: // None
                }
                gb->joypad_reg = (gb->joypad_reg & 0xC0) | (data & 0x30) | key_inputs;
                return;
            case GB_REG_SC:
                if (data == 0x81) {
                    printf("\033[31;1;4m%c\033[0m", (char)gb->mem.io[GB_REG_SB - 0xFF00]);
                }
                return;
            case GB_REG_DIV: // DIV
                gb->timer.div = 0;
                return;
            case GB_REG_TIMA: // TIMA
                gb->timer.tima = data;
                return;
            case GB_REG_TMA: // TMA
                gb->timer.tma = data;
                return;
            case GB_REG_TAC: // TAC
                gb->timer.tac = data;
                return;
            case GB_REG_IF:
                gb->cpu.int_flag = data;
                return;
            case GB_REG_DMA: // DMA
                src = get_ptr_at_high_nibble(gb, data);
                if (src == NULL) {
                    return;
                }

                memcpy(gb->mem.oam, src, 160);
                return;

            //PPU
            case GB_REG_LCDC:
                gb->ppu.lcdc = data;
                return;
            case GB_REG_STAT:
                gb->ppu.stat = (gb->ppu.stat & 0x83) | (data & 0x7C);
                return;
            case GB_REG_SCY:
                gb->ppu.scy = data;
                return;
            case GB_REG_SCX:
                gb->ppu.scx = data;
                return;
            case GB_REG_LY:
                return;
            case GB_REG_LYC:
                gb->ppu.lyc = data;
                return;
            case GB_REG_BGP:
                gb->ppu.bgp = data;
                return;
            case GB_REG_OBP0:
                gb->ppu.obp0 = data;
                return;
            case GB_REG_OBP1:
                gb->ppu.obp1 = data;
                return;
            case GB_REG_WY:
                gb->ppu.wy = data;
                return;
            case GB_REG_WX:
                gb->ppu.wx = data;
                return;
            default:
                gb->mem.io[addr - 0xFF00] = data;
                return;
        }


    } else if (addr < 0xFFFF) {
        gb->mem.hram[addr - 0xFF80] = data;
    } else {
        gb->mem.ie_reg = data;
    }

}

void write_word(gb_console *gb, uint16_t addr, uint16_t data) {
    write_byte(gb, addr, data & 0x00FF);
    write_byte(gb, addr+1, (data & 0xFF00)>>8);
}

uint8_t read_byte(gb_console *gb, uint16_t addr) {
    if (addr < 0x4000) {
        return gb->cart.rom_bank_00[addr];
    } else if (addr < 0x8000) {
        return gb->cart.rom_bank_n[addr - 0x4000];
    } else if (addr < 0xA000) {
        return gb->mem.vram[addr - 0x8000];
    } else if (addr < 0xC000) {
        if (gb->cart.external_ram_bank == NULL) {
            return 0xFF;
        }
        return gb->cart.external_ram_bank[addr - 0xA000];
    } else if (addr < 0xD000) {
        return gb->mem.wram_0[addr - 0xC000];
    } else if (addr < 0xE000) {
        return gb->mem.wram_1[addr - 0xD000];
    } else if (addr < 0xFE00) {
        if (addr < 0xF000) {
            return gb->mem.wram_0[addr - 0xE000];
        } else {
            return gb->mem.wram_1[addr - 0xF000];
        }
    } else if (addr < 0xFEA0) {
        return gb->mem.oam[addr - 0xFE00];
    } else if (addr < 0xFF00) {
        return gb->mem.unusable[addr - 0xFEA0];
    } else if (addr < 0xFF80) {
        switch (addr) {
            case GB_REG_P1:
                return gb->joypad_reg;
            case GB_REG_DIV: // DIV
                return gb->timer.div;
            case GB_REG_TIMA: // TIMA
                return gb->timer.tima;
            case GB_REG_TMA: // TMA
                return gb->timer.tma;
            case GB_REG_TAC: // TAC
                return gb->timer.tac;
            case GB_REG_IF: // Int Flag
                return gb->cpu.int_flag;


            //PPU
            case GB_REG_LCDC:
                return gb->ppu.lcdc;
            case GB_REG_STAT:
                return gb->ppu.stat;
            case GB_REG_SCY:
                return gb->ppu.scy;
            case GB_REG_SCX:
                return gb->ppu.scx;
            case GB_REG_LY:
                return gb->ppu.ly;
            case GB_REG_LYC:
                return gb->ppu.lyc;
            case GB_REG_BGP:
                return gb->ppu.bgp;
            case GB_REG_OBP0:
                return gb->ppu.obp0;
            case GB_REG_OBP1:
                return gb->ppu.obp1;
            case GB_REG_WY:
                return gb->ppu.wy;
            case GB_REG_WX:
                return gb->ppu.wx;

            default:
                return gb->mem.io[addr - 0xFF00];
        }
    } else if (addr < 0xFFFF) {
        return gb->mem.hram[addr - 0xFF80];
    } else {
        return gb->mem.ie_reg;
    }
}

uint16_t read_word(gb_console *gb, uint16_t addr) {
    return read_byte(gb, addr+1)<<8 | read_byte(gb, addr);
}
