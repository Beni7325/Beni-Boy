#include "./../includes/gb.h"


int init_gb(GB *gb, char *rom_name) {

    init_cpu(&gb->cpu);
    init_mem(&gb->mem);
    if (init_cartridge(&gb->cart, rom_name) == -1) {
        return -1;
    }

    return 0; // will handle errors later
}

void free_gb(GB *gb) {

    free_mem(&gb->mem);
    free_cartridge(&gb->cart);

}
