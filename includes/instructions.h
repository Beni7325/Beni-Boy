#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include "gb.h"


typedef struct {
    uint8_t mnemonic[16];
    uint32_t length;
    uint32_t cycles; // The lower bound if the num of cycles can vary (e.g. cond jump)
    uint32_t (*func)(GB*);
} Instruction;


void push(GB *gb, uint16_t data);

uint16_t pop(GB *gb);

#endif /* INSTRUCTIONS_H */
