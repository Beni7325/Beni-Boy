#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "gb.h"
#include "instructions.h"
#include "interrupts.h"


// There's no boot ROM, so the registers start with the values a real DMG
// has after running it, and execution begins at the cartridge entry point.
void init_cpu(gb_cpu *cpu) {

    cpu->A = 0x01; // AF = 0x01B0
    cpu->z_flag = 1;
    cpu->n_flag = 0;
    cpu->h_flag = 1;
    cpu->c_flag = 1;
    cpu->unused = 0;
    cpu->BC = 0x0013;
    cpu->DE = 0x00D8;
    cpu->HL = 0x014D;

    cpu->sp = 0xFFFE;
    cpu->pc = 0x0100;

    cpu->ime = 1;
    cpu->ime_delay = 0;
    cpu->int_flag = 0x00;

    cpu->remaining_cycles = 0x00;
    cpu->halted = 0;

}

void execute_cpu_cycle(gb_console *gb) {

    // Remaining cycles of the currently executed instruction

    if (gb->cpu.remaining_cycles == 0) {

        if (gb->cpu.halted) {
            if (gb->cpu.int_flag & 0x1F & gb->mem.ie_reg) {
                gb->cpu.halted = 0;
            } else {
                gb->cpu.remaining_cycles = 4; // TODO check if it waits another 4 cycles for an int
            }
        }

        if (!gb->cpu.halted) {

            if (gb->cpu.ime) {
                // If theres *at least* one interrupt request
                if (gb->cpu.int_flag & 0x1F & gb->mem.ie_reg) {
                    handle_interrupts(gb);
                    if (gb->cpu.remaining_cycles != 0) {
                        return;
                    }
                }
            } else if (gb->cpu.ime_delay) {
                gb->cpu.ime = 1;
                gb->cpu.ime_delay = 0;
            }

            uint8_t opcode = read_byte(gb, gb->cpu.pc);
            if (opcode == 0xCB) {
                // To do cb instructions
                opcode = read_byte(gb, gb->cpu.pc+1);
                if (cb_instructions[opcode].func == NULL) {
                    printf("Unimplemented CB instruction \"%s\"\n", cb_instructions[opcode].mnemonic);
                    free_gb(gb);
                    exit(1);
                }
                //printf("A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X SP: %04X PC: 00:%04X (%02X %02X %02X %02X)\n", gb->cpu.A, gb->cpu.F, gb->cpu.B, gb->cpu.C, gb->cpu.D, gb->cpu.E, gb->cpu.H, gb->cpu.L, gb->cpu.sp, gb->cpu.pc,read_byte(gb, gb->cpu.pc),read_byte(gb, gb->cpu.pc+1),read_byte(gb, gb->cpu.pc+2),read_byte(gb, gb->cpu.pc+3));
                //printf("%04X: %s", gb->cpu.pc, cb_instructions[opcode].mnemonic); printf("      \tRegs: %04X %04X %04X %04X sp: %04X pc: %04X x: %02X ly: %02X\n", gb->cpu.AF, gb->cpu.BC, gb->cpu.DE, gb->cpu.HL, gb->cpu.sp, gb->cpu.pc, gb->ppu.curr_pxl, gb->ppu.ly);
                gb->cpu.pc += cb_instructions[opcode].length;
                gb->cpu.remaining_cycles = cb_instructions[opcode].cycles;
                gb->cpu.remaining_cycles += cb_instructions[opcode].func(gb);
            } else {
                if (instructions[opcode].func == NULL) {
                    printf("Unimplemented instruction \"%s\"\n", instructions[opcode].mnemonic);
                    free_gb(gb);
                    exit(1);
                }
                //printf("A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X SP: %04X PC: 00:%04X (%02X %02X %02X %02X)\n", gb->cpu.A, gb->cpu.F, gb->cpu.B, gb->cpu.C, gb->cpu.D, gb->cpu.E, gb->cpu.H, gb->cpu.L, gb->cpu.sp, gb->cpu.pc, read_byte(gb, gb->cpu.pc),read_byte(gb, gb->cpu.pc+1),read_byte(gb, gb->cpu.pc+2),read_byte(gb, gb->cpu.pc+3));
                //printf("%04X: %s", gb->cpu.pc, instructions[opcode].mnemonic); for (int m=1;m<instructions[opcode].length;++m) printf(" %02X", read_byte(gb, m+gb->cpu.pc)); printf("      \tRegs: %04X %04X %04X %04X sp: %04X pc: %04X x: %02X ly: %02X\n", gb->cpu.AF, gb->cpu.BC, gb->cpu.DE, gb->cpu.HL, gb->cpu.sp, gb->cpu.pc, gb->ppu.curr_pxl, gb->ppu.ly);
                gb->cpu.pc += instructions[opcode].length;
                gb->cpu.remaining_cycles = instructions[opcode].cycles;
                gb->cpu.remaining_cycles += instructions[opcode].func(gb);
            }

        }

    }

    --gb->cpu.remaining_cycles;
}
