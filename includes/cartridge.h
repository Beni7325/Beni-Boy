#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>


typedef struct Cartridge {

    uint32_t rom_size;
    uint8_t *rom;
    uint8_t *rom_bank_00;
    uint8_t *rom_bank_01_NN;
    uint8_t rom_bank_no;

    uint8_t *external_ram;  // not used for now
    uint8_t *external_ram_bank;

} Cartridge;


int init_cartridge(Cartridge *cart, char *rom_name);

void free_cartridge(Cartridge *cart);

#endif /* CARTRIDGE_H */