#include "./../includes/cpu.h"


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

    cpu->remaining_cycles = 0x00;
    
}