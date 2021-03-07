#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include "gb.h"


typedef struct {
    uint8_t mnemonic[16];
    uint8_t length;        // Length in bytes
    uint8_t cycles;        // The lower bound if the num of cycles can vary (e.g. cond jump)
    uint32_t (*func)(GB*);  // Function pointer to the instruction
} Instruction;


// Pushes a word into memory. Updates SP
void push(GB *gb, uint16_t data);

// Pops a word from memory. Updates SP
uint16_t pop(GB *gb);

#endif /* INSTRUCTIONS_H */
