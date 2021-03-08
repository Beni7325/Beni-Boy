#include <stdio.h>
#include <stdlib.h>
#include "./../includes/gb.h"
#include "./../includes/memory.h"


/**                Memory Map 
 *
 *  ROM BANK 0              - 0x0000 - 0x3FFF
 *  ROM BANK 1-NN           - 0x4000 - 0x7FFF
 *
 *  VRAM                    - 0x8000 - 0x9FFF
 *
 *  EXTERNAL RAM            - 0xA000 - 0xBFFF
 *
 *  WRAM 0                  - 0xC000 - 0xCFFF
 *  WRAM 1                  - 0xD000 - 0xDFFF
 *  ECHO                    - 0xE000 - 0xFDFF
 *
 *  OAM                     - 0xFE00 - 0xFE9F
 *
 *  NOT USED (NU)           - 0xFEA0 - 0xFEFF
 *
 *  IO REGISTERS            - 0xFF00 - 0xFF7F
 *
 *  HRAM                    - 0xFF80 - 0xFFFE
 *
 *  INTERRUPT ENABLE REG    - 0xFFFF
 *
 */


void init_mem(Memory *mem) {

    mem->vram         = (uint8_t*)calloc(0x2000, sizeof(uint8_t));

    // temp. will be in the cartridge when implemented
    mem->external_ram = (uint8_t*)calloc(0x2000, sizeof(uint8_t));

    mem->wram_0       = (uint8_t*)calloc(0x1000, sizeof(uint8_t));
    mem->wram_1       = (uint8_t*)calloc(0x1000, sizeof(uint8_t));
    mem->echo         = (uint8_t*)calloc(0x1E00, sizeof(uint8_t));
    mem->oam          = (uint8_t*)calloc(0x00A0, sizeof(uint8_t));
    mem->nu           = (uint8_t*)calloc(0x0060, sizeof(uint8_t));
    mem->io           = (uint8_t*)calloc(0x0080, sizeof(uint8_t));
    mem->hram         = (uint8_t*)calloc(0x007F, sizeof(uint8_t));

    mem->interr_enable_reg = 0x00;

}

void free_mem(Memory *mem) {

    free(mem->vram);

    // temp. will be in the cartridge when implemented
    free(mem->external_ram);

    free(mem->wram_0);
    free(mem->wram_1);
    free(mem->echo);
    free(mem->oam);
    free(mem->nu);
    free(mem->io);
    free(mem->hram);

}

void write_byte(GB *gb, uint16_t addr, uint8_t data) {

    if (addr < 0x8000) {
        // ROM is read only. Will manage write attempts when I implement MBCs (Memory Bank Controllers)
    } else if (addr < 0xA000) {
        gb->mem.vram[addr - 0x8000] = data;
    } else if (addr < 0xC000) {
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
        gb->mem.nu[addr - 0xFEA0] = data;
    } else if (addr < 0xFF80) {

        switch (addr & 0xFF) {
            case 0x02:
                printf("%c", (char)gb->mem.io[addr-0xFF01]);
                return;
            case 0x04:
                gb->timer.div = 0;
                return;
            case 0x05:
                gb->timer.tima = data;
                return;
            case 0x06:
                gb->timer.tma = data;
                return;
            case 0x07:
                gb->timer.tac = data;
                return;
            case 0x0F:
                gb->cpu.int_flag = data;
                return;

            //PPU
            case 0x40:
                gb->ppu.lcdc = data;
                return;
            case 0x41:
                gb->ppu.stat = (gb->ppu.stat & 0x83) | (data & 0x7C);
                return;
            case 0x42:
                gb->ppu.scy = data;
                return;
            case 0x43:
                gb->ppu.scx = data;
                return;
            case 0x44:
                /*gb->ppu.ly = 0;
                gb->ppu.ticks = 0;
                gb->ppu.state = OAM_SEARCH;*/
                printf("Write to LY\n");
                return;
            case 0x45:
                gb->ppu.lyc = data;
                return;
            case 0x47:
                gb->ppu.bgp = data;
                return;
            case 0x48:
                gb->ppu.obp0 = data;
                return;
            case 0x49:
                gb->ppu.obp1 = data;
                return;
            case 0x4A:
                gb->ppu.wy = data;
                return;
            case 0x4B:
                gb->ppu.wx = data;
                return;

            default:
                gb->mem.io[addr - 0xFF00] = data;
                return;
        }

        // For debugging. Prints the byte in the serial transfer data register
        if (addr == 0xFF02) {
            
        }
        
    } else if (addr < 0xFFFF) {
        gb->mem.hram[addr - 0xFF80] = data;
    } else {
        gb->mem.interr_enable_reg = data;
    }

}

void write_word(GB *gb, uint16_t addr, uint16_t data) {
    write_byte(gb, addr, data & 0x00FF);
    write_byte(gb, addr+1, (data & 0xFF00)>>8);
}

uint8_t read_byte(GB *gb, uint16_t addr) {
    if (addr < 0x4000) {
        return gb->cart.rom_bank_00[addr];
    } else if (addr < 0x8000) {
        return gb->cart.rom_bank_01_NN[addr - 0x4000];
    } else if (addr < 0xA000) {
        return gb->mem.vram[addr - 0x8000];
    } else if (addr < 0xC000) {
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
        return gb->mem.nu[addr - 0xFEA0];
    } else if (addr < 0xFF80) {

        switch (addr & 0xFF) {
            case 0x04: // DIV
                return gb->timer.div;
            case 0x05: // TIMA
                return gb->timer.tima;
            case 0x06: // TMA
                return gb->timer.tma;
            case 0x07: // TAC
                return gb->timer.tac;
            case 0x0F: // Int Flag
                return gb->cpu.int_flag;

            //PPU
            case 0x40:
                return gb->ppu.lcdc;
            case 0x41:
                return gb->ppu.stat;
            case 0x42:
                return gb->ppu.scy;
            case 0x43:
                return gb->ppu.scx;
            case 0x44:
                return gb->ppu.ly;
            case 0x45:
                return gb->ppu.lyc;
            case 0x47:
                return gb->ppu.bgp;
            case 0x48:
                return gb->ppu.obp0;
            case 0x49:
                return gb->ppu.obp1;
            case 0x4A:
                return gb->ppu.wy;
            case 0x4B:
                return gb->ppu.wx;

            default:
                return gb->mem.io[addr - 0xFF00];
                //return 0xFF;
        }

        return gb->mem.io[addr - 0xFF00];
    } else if (addr < 0xFFFF) {
        return gb->mem.hram[addr - 0xFF80];
    } else {
        return gb->mem.interr_enable_reg;
    }

    return 0;
}

uint16_t read_word(GB *gb, uint16_t addr) {
    return read_byte(gb, addr+1)<<8 | read_byte(gb, addr);
}
