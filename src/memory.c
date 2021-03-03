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

    uint8_t *src, key_inputs = 0x0F, new_key_pressed = 0;

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

        // For debugging. Prints the byte in the serial transfer data register
        if (addr == 0xFF02) {
            printf("%c\n", (char)gb->mem.io[0xFF01]);
        }
        gb->mem.io[addr - 0xFF00] = data;
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
