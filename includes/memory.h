#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>


typedef struct GB GB; // Forward declaration


typedef struct {

    uint8_t *rom;
    uint32_t rom_size;

    uint8_t *rom_bank_00;           // 0x0000 - 0x3FFF
    uint8_t *rom_bank_01__NN;       // 0x4000 - 0x7FFF

    uint8_t *vram;                  // 0x8000 - 0x9FFF

    uint8_t *external_ram;          // 0xA000 - 0xBFFF

    uint8_t *wram_0;                // 0xC000 - 0xCFFF
    uint8_t *wram_1;                // 0xD000 - 0xDFFF
    uint8_t *echo;                  // 0xE000 - 0xFDFF

    uint8_t *oam;                   // 0xFE00 - 0xFE9F

    uint8_t *nu;                    // 0xFEA0 - 0xFEFF

    uint8_t *io;                    // 0xFF00 - 0xFF7F

    uint8_t *hram;                  // 0xFF80 - 0xFFFE

    uint8_t interr_enable_reg;      // 0xFFFF

} Memory;


void init_mem(Memory *mem);

void free_mem(Memory *mem);

// TODO: Implement these functions. For now these prototypes exist so I can call them from instructions.c
void write_byte(GB *gb, uint16_t addr, uint8_t data);

void write_word(GB *gb, uint16_t addr, uint16_t data);

uint8_t read_byte(GB *gb, uint16_t addr);

uint16_t read_word(GB *gb, uint16_t addr);

#endif /* MEMORY_H */
