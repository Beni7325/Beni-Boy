#include <stdlib.h>
#include "./../includes/instructions.h"
#include "./../includes/cpu.h"


extern const Instruction instructions[256], cb_instructions[256];


void init_cpu(Cpu *cpu) {

    cpu->A = 0x01; // AF = 0x01B0
    cpu->z_flag = 1;
    cpu->n_flag = 0;
    cpu->h_flag = 1;
    cpu->c_flag = 1;
    cpu->not_used = 0;
    cpu->BC = 0x0013;
    cpu->DE = 0x00D8;
    cpu->HL = 0x014D;

    cpu->sp = 0xFFFE;
    cpu->pc = 0x0100;

    cpu->ime = 1;
    cpu->int_flag = 0x00;

    cpu->remaining_cycles = 0x00;
    cpu->state = EXECUTION;
    
}

void tick_cpu(GB *gb) {

    if (gb->cpu.remaining_cycles == 0) {

        switch (gb->cpu.state) {

            case HALT:

                if (gb->cpu.int_flag & 0x1F & gb->mem.interr_enable_reg) {
                    gb->cpu.state = EXECUTION;
                } else {
                    gb->cpu.remaining_cycles = 4;
                }

                break;

            case EXECUTION:

                if (gb->cpu.ime) { // If theres *at least* one interrupt request
                    // Interruptions not implemented
                }

                Instruction instr;

                uint8_t opcode = read_byte(gb, gb->cpu.pc);
                if (opcode == 0xCB) {
                    opcode = read_byte(gb, gb->cpu.pc+1);
                    instr = cb_instructions[opcode];
                } else {
                    instr = instructions[opcode];
                }

                if (instr.func == NULL) {
                    printf("Unimplemented instruction \"%s\"\n", instructions[opcode].mnemonic);
                    exit(1);
                }
                //printf("A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X SP: %04X PC: 00:%04X (%02X %02X %02X %02X)\n", gb->cpu.A, gb->cpu.F, gb->cpu.B, gb->cpu.C, gb->cpu.D, gb->cpu.E, gb->cpu.H, gb->cpu.L, gb->cpu.sp, gb->cpu.pc, read_byte(gb, gb->cpu.pc),read_byte(gb, gb->cpu.pc+1),read_byte(gb, gb->cpu.pc+2),read_byte(gb, gb->cpu.pc+3));
                gb->cpu.pc += instr.length;
                gb->cpu.remaining_cycles = instr.cycles;
                gb->cpu.remaining_cycles += instr.func(gb);

                break;

        }

    }

    --gb->cpu.remaining_cycles;

}