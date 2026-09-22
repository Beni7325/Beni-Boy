#define _POSIX_C_SOURCE 200809L

#include "cartridge.h"

#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>


static const uint32_t ext_ram_size[6] = {0, 2048, 8192, 32768, 131072, 65536};

static void no_mbc(gb_cartridge *cart, uint16_t addr, uint8_t data);
static void mbc1(gb_cartridge *cart, uint16_t addr, uint8_t data);
static void mbc3_ram(gb_cartridge *cart, uint16_t addr, uint8_t data);

int load_rom(gb_cartridge *cart, char *rom_name) {

    struct stat s;

    int fd = open(rom_name, O_RDONLY);
    if (fd < 0) {
        perror("Error opening the rom");
        return 1;
    }

    if (fstat(fd, &s) == -1) {
        perror("Error measuring the rom's size");
        return 1;
    }
    cart->rom_size = s.st_size;
    if (cart->rom_size < 0x8000) {
        printf("ROM is too small to be a cartridge (%u bytes)\n", cart->rom_size);
        close(fd);
        return 1;
    }
    cart->rom_bank_count = cart->rom_size / 16384;
    cart->rom = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (cart->rom == MAP_FAILED) {
        perror("Error loading the ROM into cartory");
        return 1;
    }
    close(fd);

    if (cart->rom[0x149] >= (sizeof(ext_ram_size) / sizeof(ext_ram_size[0]))) {
        printf("Unknown External RAM size code %02X\n", cart->rom[0x149]);
        return 1;
    }
    cart->external_ram_size = ext_ram_size[cart->rom[0x149]];

    cart->external_ram = NULL;
    if (cart->external_ram_size > 0) {

        char save_name[100];
        strcpy(save_name, rom_name);
        save_name[strlen(save_name) - 2] = '\0';
        strcat(save_name, "sav");
        int fd = open(save_name, O_RDWR | O_CREAT, 0664);
        if (fd < 0) {
            perror("Error opening the rom");
            return 1;
        }

        if (ftruncate(fd, cart->external_ram_size) == -1) {
            perror("Error sizing the save file");
            close(fd);
            return 1;
        }
        cart->external_ram = mmap(0, cart->external_ram_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (cart->external_ram == MAP_FAILED) {
            perror("Error loading the External Ram");
            return 1;
        }

        close(fd);


    }


    // Temp until I implement MBC's
    cart->rom_bank_00 = cart->rom;
    cart->rom_bank_n = cart->rom + 0x4000;
    cart->rom_bank_no = 0x01;

    cart->external_ram_bank = cart->external_ram;
    cart->last_written_mbc = 0xFF;

    switch (cart->rom[0x147]) {

        case 0x00:
            printf("No MBC\n");
            cart->mbc_write = no_mbc;
            break;
        case 0x01:
            printf("MBC1\n");
            cart->mbc_write = mbc1;
            break;
        case 0x13:
        case 0x10:
            printf("MBC3+RAM\n");
            cart->mbc_write = mbc3_ram;
            break;
        default:
            printf("Missing MBC combo %02X\n", cart->rom[0x147]);
            return 1;
    }
    printf("External RAM: %u\n", cart->external_ram_size);

    return 0;
}

void free_cartridge(gb_cartridge *cart) {

    if (cart->external_ram != NULL && cart->external_ram != MAP_FAILED) {
        munmap(cart->external_ram, cart->external_ram_size);
    }

    if (cart->rom != NULL && cart->rom != MAP_FAILED) {
        munmap(cart->rom, cart->rom_size);
    }

}

static void no_mbc(gb_cartridge *cart, uint16_t addr, uint8_t data) {

    (void)cart;
    (void)addr;
    (void)data;

}

static void mbc1(gb_cartridge *cart, uint16_t addr, uint8_t data) {

    if (addr < 0x2000) {
        // RAM enable: not implemented
    } else if (addr < 0x4000) {

        cart->rom_bank_no = (cart->rom_bank_no & 0x60) | (data & 0x1F);

    } else if (addr < 0x6000) {

        cart->rom_bank_no = (cart->rom_bank_no & 0x1F) | ((data & 0x03) << 5);

    } else {
        // Banking mode select: not implemented, mode 0 is assumed above
    }

    switch (cart->rom_bank_no) {
        case 0x00:
        case 0x20:
        case 0x40:
        case 0x60:
            ++cart->rom_bank_no;
            break;
    }
    cart->rom_bank_n = cart->rom + 16384*(cart->rom_bank_no % cart->rom_bank_count);


}

static void mbc3_ram(gb_cartridge *cart, uint16_t addr, uint8_t data) {

    time_t rawtime;
    struct tm *timeinfo;

    if (addr < 0x2000) {
        // RAM and timer enable: not implemented
    } else if (addr < 0x4000) {
        if (data == 0) {
            cart->rom_bank_n = cart->rom + 16384;
        } else {
            cart->rom_bank_n = cart->rom + 16384*((data&0x7F) % cart->rom_bank_count);
        }
    } else if (addr < 0x6000) {
        if (data <= 3 && 8192*(uint32_t)data < cart->external_ram_size) {
            cart->external_ram_bank = cart->external_ram + 8192*data;
        } else if ((data <= 0x0C) && (data >= 0x08)) {
            cart->external_ram_bank = &cart->rtc[data - 0x08];
        }
    } else {
        if (cart->last_written_mbc == 0x00 && data == 0x01) {

            time (&rawtime);
            timeinfo = localtime (&rawtime);
            cart->rtc[0] = timeinfo->tm_sec;
            cart->rtc[1] = timeinfo->tm_min;
            cart->rtc[2] = timeinfo->tm_hour;
            cart->rtc[3] = 0x00;
            cart->rtc[4] = 0x00;

        }

        cart->last_written_mbc = data;
    }

}
