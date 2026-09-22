#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include "gb.h"

typedef struct {
    const char *mnemonic;
    uint8_t length;
    uint8_t cycles; // The lower amount if it depends on a condition (e.g. cond jump)
    uint32_t (*func)(gb_console*);
} gb_instruction;

extern const gb_instruction instructions[256];
extern const gb_instruction cb_instructions[256];

void push(gb_console *gb, uint16_t data);

uint16_t pop(gb_console *gb);

#endif /* INSTRUCTIONS_H */