#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

typedef struct gb_console gb_console; // Forward declaration

#define GB_REG_P1   0xFF00
#define GB_REG_SB   0xFF01
#define GB_REG_SC   0xFF02
#define GB_REG_DIV  0xFF04
#define GB_REG_TIMA 0xFF05
#define GB_REG_TMA  0xFF06
#define GB_REG_TAC  0xFF07
#define GB_REG_IF   0xFF0F
#define GB_REG_LCDC 0xFF40
#define GB_REG_STAT 0xFF41
#define GB_REG_SCY  0xFF42
#define GB_REG_SCX  0xFF43
#define GB_REG_LY   0xFF44
#define GB_REG_LYC  0xFF45
#define GB_REG_DMA  0xFF46
#define GB_REG_BGP  0xFF47
#define GB_REG_OBP0 0xFF48
#define GB_REG_OBP1 0xFF49
#define GB_REG_WY   0xFF4A
#define GB_REG_WX   0xFF4B


/*
 *    0x0000 - 0x3FFF   ROM bank 00           cart.rom_bank_00
 *    0x4000 - 0x7FFF   ROM bank 01..NN       cart.rom_bank_n
 *    0x8000 - 0x9FFF   VRAM                  mem.vram
 *    0xA000 - 0xBFFF   External RAM          cart.external_ram_bank
 *    0xC000 - 0xCFFF   WRAM bank 0           mem.wram_0
 *    0xD000 - 0xDFFF   WRAM bank 1           mem.wram_1
 *    0xE000 - 0xFDFF   Echo RAM              mem.wram_0 / mem.wram_1, mirrored
 *    0xFE00 - 0xFE9F   OAM                   mem.oam
 *    0xFEA0 - 0xFEFF   Unusable              mem.unusable
 *    0xFF00 - 0xFF7F   I/O registers         split per register, see below
 *    0xFF80 - 0xFFFE   HRAM                  mem.hram
 *    0xFFFF            Interrupt enable      mem.ie_reg
 *
 *  The I/O range is dispatched one register at a time:
 *
 *    0xFF00            P1   joypad           joypad_reg
 *    0xFF01            SB   serial data      mem.io
 *    0xFF02            SC   serial control   printed to stdout, not stored
 *    0xFF04 - 0xFF07   DIV TIMA TMA TAC      timer.div .. timer.tac
 *    0xFF0F            IF   interrupt flags  cpu.int_flag
 *    0xFF40 - 0xFF45   LCDC STAT scroll LY   ppu.lcdc .. ppu.lyc
 *    0xFF46            DMA  OAM transfer     copied into mem.oam
 *    0xFF47 - 0xFF4B   palettes, window      ppu.bgp .. ppu.wx
 *    anything else                           mem.io
 */
typedef struct {

    uint8_t *vram;                  // 0x8000 - 0x9FFF

    uint8_t *wram_0;                // 0xC000 - 0xCFFF
    uint8_t *wram_1;                // 0xD000 - 0xDFFF

    uint8_t *oam;                   // 0xFE00 - 0xFE9F

    uint8_t *unusable;              // 0xFEA0 - 0xFEFF

    uint8_t *io;                    // 0xFF00 - 0xFF7F

    uint8_t *hram;                  // 0xFF80 - 0xFFFE

    uint8_t ie_reg;                 // 0xFFFF

} gb_memory;


void init_mem(gb_memory *mem);

void free_mem(gb_memory *mem);

void write_byte(gb_console *gb, uint16_t addr, uint8_t data);

void write_word(gb_console *gb, uint16_t addr, uint16_t data);

uint8_t read_byte(gb_console *gb, uint16_t addr);

uint16_t read_word(gb_console *gb, uint16_t addr);

#endif /* MEMORY_H */
