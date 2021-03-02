#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "./../includes/cartridge.h"


uint16_t ext_ram_size[4] = {0, 2048, 8192, 32768};


int init_cartridge(Cartridge *cart, char *rom_name) {

    FILE *rom_file;


    rom_file = fopen(rom_name, "rb");

    // Calculate the size of the rom
    fseek(rom_file, 0, SEEK_END);
    cart->rom_size = ftell(rom_file);
    rewind(rom_file);

    // Read all the rom file contents
    cart->rom = (uint8_t *)malloc(cart->rom_size * sizeof(uint8_t));
    fread(cart->rom, cart->rom_size, 1, rom_file);
    fclose(rom_file);

    cart->rom_bank_00 = cart->rom;
    cart->rom_bank_01_NN = cart->rom + 0x4000;
    cart->rom_bank_no = 0x01;

    // Temporary until I properly implement the external ram
    cart->external_ram_bank = (uint8_t *)malloc(cart->rom_size * sizeof(uint8_t));

    return 0;
}

void free_cartridge(Cartridge *cart) {

    // It should be external_ram, when its implemented
    if (cart->external_ram_bank != NULL) {
        free(cart->external_ram_bank);
    }

    if (cart->rom != NULL) {
        free(cart->rom);
    }

}
