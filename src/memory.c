#include <stdlib.h>
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
