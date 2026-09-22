#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>

typedef struct gb_cartridge {

    uint32_t rom_size;
    uint32_t rom_bank_count;
    uint8_t *rom;
    uint8_t *rom_bank_00;
    uint8_t *rom_bank_n;
    uint8_t rom_bank_no;
    uint8_t last_written_mbc;
    uint8_t rtc[5];   // Holds the rtc register last requested by the game
                        // Ill do a proper rtc and mbc struct later... maybe


    uint8_t *external_ram;
    uint8_t *external_ram_bank;
    uint32_t external_ram_size;

    void (*mbc_write)(struct gb_cartridge *cart, uint16_t, uint8_t);

} gb_cartridge;

int load_rom(gb_cartridge *cart, char *rom_name);

void free_cartridge(gb_cartridge *cart);

#endif /* CARTRIDGE_H */
