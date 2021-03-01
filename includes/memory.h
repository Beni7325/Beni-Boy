#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

typedef struct GB GB; // Forward declaration


typedef struct {

    // TODO

} Memory;

// TODO: Implement these functions. For now these prototypes exist so I can call them from instructions.c
void write_byte(GB *gb, uint16_t addr, uint8_t data);

void write_word(GB *gb, uint16_t addr, uint16_t data);

uint8_t read_byte(GB *gb, uint16_t addr);

uint16_t read_word(GB *gb, uint16_t addr);

#endif /* MEMORY_H */
