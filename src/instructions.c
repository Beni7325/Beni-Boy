#include <stdlib.h>
#include "./../includes/instructions.h"


// ######################   Auxiliary functions   #####################

void push(GB *gb, uint16_t data) {
    gb->cpu.sp -= 2;
    write_word(gb, gb->cpu.sp, data);
}

uint16_t pop(GB *gb) {
    uint16_t tmp = read_word(gb, gb->cpu.sp);
    gb->cpu.sp += 2;
    return tmp;
}

// ###################   Misc/control instructions   ##################

uint32_t nop(GB *gb) {
    return 0;
}

uint32_t ei(GB *gb) {
    // TODO ime bug
    gb->cpu.set_ime = 1;
    return 0;
}

uint32_t di(GB *gb) {
    gb->cpu.ime = 0;
    return 0;
}

uint32_t halt(GB *gb) {
    /*if (gb->cpu.int_flag & 0x1F & gb->mem.interr_enable_reg) {
        // TODO: HALT BUG
    } else {
        gb->cpu.pc -= 1;
    }*/
    gb->cpu.state = HALT;
    return 0;
}


// ##########################   Jumps/Calls   #########################

uint32_t jp_a16(GB *gb) {
    gb->cpu.pc = read_word(gb, gb->cpu.pc-2);
    return 0;
}

uint32_t jp_z_a16(GB *gb) {
    if (gb->cpu.z_flag) {
        gb->cpu.pc = read_word(gb, gb->cpu.pc-2);
        return 4;
    }
    return 0;
}

uint32_t jp_nz_a16(GB *gb) {
    if (!gb->cpu.z_flag) {
        gb->cpu.pc = read_word(gb, gb->cpu.pc-2);
        return 4;
    }
    return 0;
}

uint32_t jp_c_a16(GB *gb) {
    if (gb->cpu.c_flag) {
        gb->cpu.pc = read_word(gb, gb->cpu.pc-2);
        return 4;
    }
    return 0;
}

uint32_t jp_nc_a16(GB *gb) {
    if (!gb->cpu.c_flag) {
        gb->cpu.pc = read_word(gb, gb->cpu.pc-2);
        return 4;
    }
    return 0;
}

uint32_t jp_hl(GB *gb) {
    gb->cpu.pc = gb->cpu.HL;
    return 0;
}

uint32_t jr_r8(GB *gb) {
    gb->cpu.pc += (int8_t)read_byte(gb, gb->cpu.pc-1);
    return 0;
}

uint32_t jr_z_r8(GB *gb) {
    if (gb->cpu.z_flag) {
        gb->cpu.pc += (int8_t)read_byte(gb, gb->cpu.pc-1);
        return 4;
    }
    return 0;
}

uint32_t jr_nz_r8(GB *gb) {
    if (!gb->cpu.z_flag) {
        gb->cpu.pc += (int8_t)read_byte(gb, gb->cpu.pc-1);
        return 4;
    }
    return 0;
}

uint32_t jr_c_r8(GB *gb) {
    if (gb->cpu.c_flag) {
        gb->cpu.pc += (int8_t)read_byte(gb, gb->cpu.pc-1);
        return 4;
    }
    return 0;
}

uint32_t jr_nc_r8(GB *gb) {
    if (!gb->cpu.c_flag) {
        gb->cpu.pc += (int8_t)read_byte(gb, gb->cpu.pc-1);
        return 4;
    }
    return 0;
}

uint32_t call_a16(GB *gb) {
    push(gb, gb->cpu.pc);
    gb->cpu.pc = read_word(gb, gb->cpu.pc-2);
    return 0;
}

uint32_t call_z_a16(GB *gb) {
    if (gb->cpu.z_flag) {
        push(gb, gb->cpu.pc);
        gb->cpu.pc = read_word(gb, gb->cpu.pc-2);
        return 12;
    }
    return 0;
}

uint32_t call_nz_a16(GB *gb) {
    if (!gb->cpu.z_flag) {
        push(gb, gb->cpu.pc);
        gb->cpu.pc = read_word(gb, gb->cpu.pc-2);
        return 12;
    }
    return 0;
}

uint32_t call_c_a16(GB *gb) {
    if (gb->cpu.c_flag) {
        push(gb, gb->cpu.pc);
        gb->cpu.pc = read_word(gb, gb->cpu.pc-2);
        return 12;
    }
    return 0;
}

uint32_t call_nc_a16(GB *gb) {
    if (!gb->cpu.c_flag) {
        push(gb, gb->cpu.pc);
        gb->cpu.pc = read_word(gb, gb->cpu.pc-2);
        return 12;
    }
    return 0;
}

uint32_t ret(GB *gb) {
    gb->cpu.pc = pop(gb);
    return 0;
}

uint32_t ret_z(GB *gb) {
    if (gb->cpu.z_flag) {
        gb->cpu.pc = pop(gb);
        return 12;
    }
    return 0;
}

uint32_t ret_nz(GB *gb) {
    if (!gb->cpu.z_flag) {
        gb->cpu.pc = pop(gb);
        return 12;
    }
    return 0;
}

uint32_t ret_c(GB *gb) {
    if (gb->cpu.c_flag) {
        gb->cpu.pc = pop(gb);
        return 12;
    }
    return 0;
}

uint32_t ret_nc(GB *gb) {
    if (!gb->cpu.c_flag) {
        gb->cpu.pc = pop(gb);
        return 12;
    }
    return 0;
}

uint32_t reti(GB *gb) {
    gb->cpu.pc = pop(gb);
    // TODO ime BUG;
    gb->cpu.set_ime = 1;
    return 0;
}

uint32_t rst_00h(GB *gb) {
    push(gb, gb->cpu.pc);
    gb->cpu.pc = 0x00;
    return 0;
}

uint32_t rst_08h(GB *gb) {
    push(gb, gb->cpu.pc);
    gb->cpu.pc = 0x08;
    return 0;
}

uint32_t rst_10h(GB *gb) {
    push(gb, gb->cpu.pc);
    gb->cpu.pc = 0x10;
    return 0;
}

uint32_t rst_18h(GB *gb) {
    push(gb, gb->cpu.pc);
    gb->cpu.pc = 0x18;
    return 0;
}

uint32_t rst_20h(GB *gb) {
    push(gb, gb->cpu.pc);
    gb->cpu.pc = 0x28;
    return 0;
}

uint32_t rst_28h(GB *gb) {
    push(gb, gb->cpu.pc);
    gb->cpu.pc = 0x28;
    return 0;
}

uint32_t rst_30h(GB *gb) {
    push(gb, gb->cpu.pc);
    gb->cpu.pc = 0x30;
    return 0;
}

uint32_t rst_38h(GB *gb) {
    push(gb, gb->cpu.pc);
    gb->cpu.pc = 0x38;
    return 0;
}


// #####################   8bit load/store/move   #####################

uint32_t ld_a_a(GB *gb) {
    return 0;
}

uint32_t ld_a_b(GB *gb) {
    gb->cpu.A = gb->cpu.B;
    return 0;
}

uint32_t ld_a_c(GB *gb) {
    gb->cpu.A = gb->cpu.C;
    return 0;
}

uint32_t ld_a_d(GB *gb) {
    gb->cpu.A = gb->cpu.D;
    return 0;
}

uint32_t ld_a_e(GB *gb) {
    gb->cpu.A = gb->cpu.E;
    return 0;
}

uint32_t ld_a_h(GB *gb) {
    gb->cpu.A = gb->cpu.H;
    return 0;
}

uint32_t ld_a_l(GB *gb) {
    gb->cpu.A = gb->cpu.L;
    return 0;
}

uint32_t ld_a_d8(GB *gb) {
    gb->cpu.A = read_byte(gb, gb->cpu.pc-1);
    return 0;
}

uint32_t ld_a_bc(GB *gb) {
    gb->cpu.A = read_byte(gb, gb->cpu.BC);
    return 0;
}

uint32_t ld_a_de(GB *gb) {
    gb->cpu.A = read_byte(gb, gb->cpu.DE);
    return 0;
}

uint32_t ld_a_hl(GB *gb) {
    gb->cpu.A = read_byte(gb, gb->cpu.HL);
    return 0;
}

uint32_t ld_a_a16(GB *gb) {
    gb->cpu.A = read_byte(gb, read_word(gb, gb->cpu.pc-2));
    return 0;
}

uint32_t ld_a_hlplus(GB *gb) {
    gb->cpu.A = read_byte(gb, gb->cpu.HL++);
    return 0;
}

uint32_t ld_a_hlminus(GB *gb) {
    gb->cpu.A = read_byte(gb, gb->cpu.HL--);
    return 0;
}

uint32_t ld_a_c_plus_ff00(GB *gb) {
    gb->cpu.A = read_byte(gb, gb->cpu.C | 0xFF00);
    return 0;
}

uint32_t ld_c_plus_ff00_a(GB *gb) {
    write_byte(gb, gb->cpu.C | 0xFF00, gb->cpu.A);
    return 0;
}

uint32_t ld_b_a(GB *gb) {
    gb->cpu.B = gb->cpu.A;
    return 0;
}

uint32_t ld_b_b(GB *gb) {
    return 0;
}

uint32_t ld_b_c(GB *gb) {
    gb->cpu.B = gb->cpu.C;
    return 0;
}

uint32_t ld_b_d(GB *gb) {
    gb->cpu.B = gb->cpu.D;
    return 0;
}

uint32_t ld_b_e(GB *gb) {
    gb->cpu.B = gb->cpu.E;
    return 0;
}

uint32_t ld_b_h(GB *gb) {
    gb->cpu.B = gb->cpu.H;
    return 0;
}

uint32_t ld_b_l(GB *gb) {
    gb->cpu.B = gb->cpu.L;
    return 0;
}

uint32_t ld_b_d8(GB *gb) {
    gb->cpu.B = read_byte(gb, gb->cpu.pc-1);
    return 0;
}

uint32_t ld_b_hl(GB *gb) {
    gb->cpu.B = read_byte(gb, gb->cpu.HL);
    return 0;
}

uint32_t ld_c_a(GB *gb) {
    gb->cpu.C = gb->cpu.A;
    return 0;
}

uint32_t ld_c_b(GB *gb) {
    gb->cpu.C = gb->cpu.B;
    return 0;
}

uint32_t ld_c_c(GB *gb) {
    return 0;
}

uint32_t ld_c_d(GB *gb) {
    gb->cpu.C = gb->cpu.D;
    return 0;
}

uint32_t ld_c_e(GB *gb) {
    gb->cpu.C = gb->cpu.E;
    return 0;
}

uint32_t ld_c_h(GB *gb) {
    gb->cpu.C = gb->cpu.H;
    return 0;
}

uint32_t ld_c_l(GB *gb) {
    gb->cpu.C = gb->cpu.L;
    return 0;
}

uint32_t ld_c_d8(GB *gb) {
    gb->cpu.C = read_byte(gb, gb->cpu.pc-1);
    return 0;
}

uint32_t ld_c_hl(GB *gb) {
    gb->cpu.C = read_byte(gb, gb->cpu.HL);
    return 0;
}

uint32_t ld_d_a(GB *gb) {
    gb->cpu.D = gb->cpu.A;
    return 0;
}

uint32_t ld_d_b(GB *gb) {
    gb->cpu.D = gb->cpu.B;
    return 0;
}

uint32_t ld_d_c(GB *gb) {
    gb->cpu.D = gb->cpu.C;
    return 0;
}

uint32_t ld_d_d(GB *gb) {
    return 0;
}

uint32_t ld_d_e(GB *gb) {
    gb->cpu.D = gb->cpu.E;
    return 0;
}

uint32_t ld_d_h(GB *gb) {
    gb->cpu.D = gb->cpu.H;
    return 0;
}

uint32_t ld_d_l(GB *gb) {
    gb->cpu.D = gb->cpu.L;
    return 0;
}

uint32_t ld_d_d8(GB *gb) {
    gb->cpu.D = read_byte(gb, gb->cpu.pc-1);
    return 0;
}

uint32_t ld_d_hl(GB *gb) {
    gb->cpu.D = read_byte(gb, gb->cpu.HL);
    return 0;
}

uint32_t ld_e_a(GB *gb) {
    gb->cpu.E = gb->cpu.A;
    return 0;
}

uint32_t ld_e_b(GB *gb) {
    gb->cpu.E = gb->cpu.B;
    return 0;
}

uint32_t ld_e_c(GB *gb) {
    gb->cpu.E = gb->cpu.C;
    return 0;
}

uint32_t ld_e_d(GB *gb) {
    gb->cpu.E = gb->cpu.D;
    return 0;
}

uint32_t ld_e_e(GB *gb) {
    return 0;
}

uint32_t ld_e_h(GB *gb) {
    gb->cpu.E = gb->cpu.H;
    return 0;
}

uint32_t ld_e_l(GB *gb) {
    gb->cpu.E = gb->cpu.L;
    return 0;
}

uint32_t ld_e_d8(GB *gb) {
    gb->cpu.E = read_byte(gb, gb->cpu.pc-1);
    return 0;
}

uint32_t ld_e_hl(GB *gb) {
    gb->cpu.E = read_byte(gb, gb->cpu.HL);
    return 0;
}

uint32_t ld_h_a(GB *gb) {
    gb->cpu.H = gb->cpu.A;
    return 0;
}

uint32_t ld_h_b(GB *gb) {
    gb->cpu.H = gb->cpu.B;
    return 0;
}

uint32_t ld_h_c(GB *gb) {
    gb->cpu.H = gb->cpu.C;
    return 0;
}

uint32_t ld_h_d(GB *gb) {
    gb->cpu.H = gb->cpu.D;
    return 0;
}

uint32_t ld_h_e(GB *gb) {
    gb->cpu.H = gb->cpu.E;
    return 0;
}

uint32_t ld_h_h(GB *gb) {
    return 0;
}

uint32_t ld_h_l(GB *gb) {
    gb->cpu.H = gb->cpu.L;
    return 0;
}

uint32_t ld_h_d8(GB *gb) {
    gb->cpu.H = read_byte(gb, gb->cpu.pc-1);
    return 0;
}

uint32_t ld_h_hl(GB *gb) {
    gb->cpu.H = read_byte(gb, gb->cpu.HL);
    return 0;
}

uint32_t ld_l_a(GB *gb) {
    gb->cpu.L = gb->cpu.A;
    return 0;
}

uint32_t ld_l_b(GB *gb) {
    gb->cpu.L = gb->cpu.B;
    return 0;
}

uint32_t ld_l_c(GB *gb) {
    gb->cpu.L = gb->cpu.C;
    return 0;
}

uint32_t ld_l_d(GB *gb) {
    gb->cpu.L = gb->cpu.D;
    return 0;
}

uint32_t ld_l_e(GB *gb) {
    gb->cpu.L = gb->cpu.E;
    return 0;
}

uint32_t ld_l_h(GB *gb) {
    gb->cpu.L = gb->cpu.H;
    return 0;
}

uint32_t ld_l_l(GB *gb) {
    return 0;
}

uint32_t ld_l_d8(GB *gb) {
    gb->cpu.L = read_byte(gb, gb->cpu.pc-1);
    return 0;
}

uint32_t ld_l_hl(GB *gb) {
    gb->cpu.L = read_byte(gb, gb->cpu.HL);
    return 0;
}

uint32_t ld_bc_a(GB *gb) {
    write_byte(gb, gb->cpu.BC, gb->cpu.A);
    return 0;
}

uint32_t ld_de_a(GB *gb) {
    write_byte(gb, gb->cpu.DE, gb->cpu.A);
    return 0;
}

uint32_t ld_hl_a(GB *gb) {
    write_byte(gb, gb->cpu.HL, gb->cpu.A);
    return 0;
}

uint32_t ld_hl_b(GB *gb) {
    write_byte(gb, gb->cpu.HL, gb->cpu.B);
    return 0;
}

uint32_t ld_hl_c(GB *gb) {
    write_byte(gb, gb->cpu.HL, gb->cpu.C);
    return 0;
}

uint32_t ld_hl_d(GB *gb) {
    write_byte(gb, gb->cpu.HL, gb->cpu.D);
    return 0;
}

uint32_t ld_hl_e(GB *gb) {
    write_byte(gb, gb->cpu.HL, gb->cpu.E);
    return 0;
}

uint32_t ld_hl_h(GB *gb) {
    write_byte(gb, gb->cpu.HL, gb->cpu.H);
    return 0;
}

uint32_t ld_hl_l(GB *gb) {
    write_byte(gb, gb->cpu.HL, gb->cpu.L);
    return 0;
}

uint32_t ld_hl_d8(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.pc-1));
    return 0;
}

uint32_t ld_hlplus_a(GB *gb) {
    write_byte(gb, gb->cpu.HL++, gb->cpu.A);
    return 0;
}

uint32_t ld_hlminus_a(GB *gb) {
    write_byte(gb, gb->cpu.HL--, gb->cpu.A);
    return 0;
}

uint32_t ld_a16_a(GB *gb) {
    write_byte(gb, read_word(gb, gb->cpu.pc-2), gb->cpu.A);
    return 0;
}

uint32_t ldh_a8_plus_ff00_a(GB *gb) {
    write_byte(gb, read_byte(gb, gb->cpu.pc-1) | 0xFF00, gb->cpu.A);
    return 0;
}

uint32_t ldh_a_a8_plus_ff00(GB *gb) {
    gb->cpu.A = read_byte(gb, read_byte(gb, gb->cpu.pc-1) | 0xFF00);
    return 0;
}


// ####################   16bit load/store/move   #####################

uint32_t ld_bc_d16(GB *gb) {
    gb->cpu.BC = read_word(gb, gb->cpu.pc-2);
    return 0;
}

uint32_t ld_de_d16(GB *gb) {
    gb->cpu.DE = read_word(gb, gb->cpu.pc-2);
    return 0;
}

uint32_t ld_hl_d16(GB *gb) {
    gb->cpu.HL = read_word(gb, gb->cpu.pc-2);
    return 0;
}

uint32_t ld_sp_d16(GB *gb) {
    gb->cpu.sp = read_word(gb, gb->cpu.pc-2);
    return 0;
}

uint32_t ld_sp_hl(GB *gb) {
    gb->cpu.sp = gb->cpu.HL;
    return 0;
}

uint32_t ld_a16_sp(GB *gb) {
    write_word(gb, read_word(gb, gb->cpu.pc-2), gb->cpu.sp);
    return 0;
}

uint32_t ld_hl_sp_r8(GB *gb) {
    int8_t r8 = (int8_t)read_byte(gb, gb->cpu.pc-1);
    uint16_t sp_plus_r8 = gb->cpu.sp + r8;

    gb->cpu.HL = sp_plus_r8;

    gb->cpu.z_flag = 0;
    gb->cpu.n_flag = 0;
    if (r8 >= 0) {
        gb->cpu.c_flag = ( ((gb->cpu.sp & 0xFF) + r8) > 0xFF );
        gb->cpu.h_flag = ( ((gb->cpu.sp & 0xF) + (r8 & 0xF)) > 0xF );
    } else {
        gb->cpu.c_flag = ( (sp_plus_r8 & 0xFF) <= (gb->cpu.sp & 0xFF) );
        gb->cpu.h_flag = ( (sp_plus_r8 & 0xF) <= (gb->cpu.sp & 0xF) );
    }

    return 0;
}

uint32_t push_af(GB *gb) {
    push(gb, gb->cpu.AF);
    return 0;
}

uint32_t push_bc(GB *gb) {
    push(gb, gb->cpu.BC);
    return 0;
}

uint32_t push_de(GB *gb) {
    push(gb, gb->cpu.DE);
    return 0;
}

uint32_t push_hl(GB *gb) {
    push(gb, gb->cpu.HL);
    return 0;
}

uint32_t pop_af(GB *gb) {
    gb->cpu.AF = pop(gb) & 0xFFF0;
    return 0;
}

uint32_t pop_bc(GB *gb) {
    gb->cpu.BC = pop(gb);
    return 0;
}

uint32_t pop_de(GB *gb) {
    gb->cpu.DE = pop(gb);
    return 0;
}

uint32_t pop_hl(GB *gb) {
    gb->cpu.HL = pop(gb);
    return 0;
}


// ##################   8bit arithmetical/logical   ###################

static inline void inc_x(GB *gb, uint8_t *reg) {
    gb->cpu.h_flag = ( ((*reg & 0xF) + 1) > 0xF );
    gb->cpu.n_flag = 0;
    gb->cpu.z_flag = ( ++(*reg) == 0x00 );
}

uint32_t inc_a(GB *gb) {
    inc_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t inc_b(GB *gb) {
    inc_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t inc_c(GB *gb) {
    inc_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t inc_d(GB *gb) {
    inc_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t inc_e(GB *gb) {
    inc_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t inc_h(GB *gb) {
    inc_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t inc_l(GB *gb) {
    inc_x(gb, &gb->cpu.L);
    return 0;
}


static inline void dec_x(GB *gb, uint8_t *reg) {
    gb->cpu.h_flag = ( 1 > (*reg & 0xF) );
    gb->cpu.n_flag = 1;
    gb->cpu.z_flag = ( --(*reg) == 0x00 );
}
uint32_t dec_a(GB *gb) {
    dec_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t dec_b(GB *gb) {
    dec_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t dec_c(GB *gb) {
    dec_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t dec_d(GB *gb) {
    dec_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t dec_e(GB *gb) {
    dec_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t dec_h(GB *gb) {
    dec_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t dec_l(GB *gb) {
    dec_x(gb, &gb->cpu.L);
    return 0;
}


uint32_t inc_at_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);

    gb->cpu.h_flag = ( ((byte & 0xF) + 1) == 0x10 );
    gb->cpu.n_flag = 0;
    write_byte(gb, gb->cpu.HL, ++byte);
    gb->cpu.z_flag = ( byte == 0x00 );

    return 0;
}


uint32_t dec_at_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);

    gb->cpu.h_flag = ( 1 > (byte & 0xF) );
    gb->cpu.n_flag = 1;
    write_byte(gb, gb->cpu.HL, --byte);
    gb->cpu.z_flag = ( byte == 0x00 );

    return 0;
}


static inline void cp_x(GB *gb, uint8_t *reg) {
    gb->cpu.z_flag = (*reg == gb->cpu.A);
    gb->cpu.n_flag = 1;
    gb->cpu.h_flag = ( (*reg & 0xF) > (gb->cpu.A & 0xF) );
    gb->cpu.c_flag = (*reg > gb->cpu.A);
}

uint32_t cp_a(GB *gb) {
    cp_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t cp_b(GB *gb) {
    cp_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t cp_c(GB *gb) {
    cp_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t cp_d(GB *gb) {
    cp_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t cp_e(GB *gb) {
    cp_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t cp_h(GB *gb) {
    cp_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t cp_l(GB *gb) {
    cp_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t cp_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    cp_x(gb, &byte);
    return 0;
}

uint32_t cp_d8(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.pc-1);
    cp_x(gb, &byte);
    return 0;
}


static inline void and_x(GB *gb, uint8_t *reg) {
    gb->cpu.A &= *reg;
    gb->cpu.z_flag = (gb->cpu.A == 0);
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 1;
    gb->cpu.c_flag = 0;
}

uint32_t and_a(GB *gb) {
    and_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t and_b(GB *gb) {
    and_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t and_c(GB *gb) {
    and_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t and_d(GB *gb) {
    and_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t and_e(GB *gb) {
    and_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t and_h(GB *gb) {
    and_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t and_l(GB *gb) {
    and_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t and_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    and_x(gb, &byte);
    return 0;
}

uint32_t and_d8(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.pc-1);
    and_x(gb, &byte);
    return 0;
}


static inline void or_x(GB *gb, uint8_t *reg) {
    gb->cpu.A |= *reg;
    gb->cpu.z_flag = gb->cpu.A == 0;
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = 0;
}

uint32_t or_a(GB *gb) {
    or_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t or_b(GB *gb) {
    or_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t or_c(GB *gb) {
    or_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t or_d(GB *gb) {
    or_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t or_e(GB *gb) {
    or_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t or_h(GB *gb) {
    or_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t or_l(GB *gb) {
    or_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t or_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    or_x(gb, &byte);
    return 0;
}

uint32_t or_d8(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.pc-1);
    or_x(gb, &byte);
    return 0;
}


static inline void xor_x(GB *gb, uint8_t *reg) {
    gb->cpu.A ^= *reg;
    gb->cpu.z_flag = gb->cpu.A == 0;
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = 0;
}

uint32_t xor_a(GB *gb) {
    xor_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t xor_b(GB *gb) {
    xor_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t xor_c(GB *gb) {
    xor_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t xor_d(GB *gb) {
    xor_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t xor_e(GB *gb) {
    xor_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t xor_h(GB *gb) {
    xor_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t xor_l(GB *gb) {
    xor_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t xor_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    xor_x(gb, &byte);
    return 0;
}

uint32_t xor_d8(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.pc-1);
    xor_x(gb, &byte);
    return 0;
}



static inline void add_a_x(GB *gb, uint8_t *reg) {
    uint16_t tmp = gb->cpu.A + *reg;
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = ( ((gb->cpu.A & 0xF) + (*reg & 0xF) ) > 0xF );
    gb->cpu.c_flag = (tmp > 0xFF);
    gb->cpu.A = tmp & 0xFF;
    gb->cpu.z_flag = (gb->cpu.A == 0);
}

uint32_t add_a_a(GB *gb) {
    add_a_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t add_a_b(GB *gb) {
    add_a_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t add_a_c(GB *gb) {
    add_a_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t add_a_d(GB *gb) {
    add_a_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t add_a_e(GB *gb) {
    add_a_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t add_a_h(GB *gb) {
    add_a_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t add_a_l(GB *gb) {
    add_a_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t add_a_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    add_a_x(gb, &byte);
    return 0;
}

uint32_t add_a_d8(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.pc-1);
    add_a_x(gb, &byte);
    return 0;
}


static inline void adc_a_x(GB *gb, uint8_t *reg){
    uint16_t tmp = gb->cpu.A + *reg + gb->cpu.c_flag;
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = ( ((gb->cpu.A & 0xF) + (*reg & 0xF) + gb->cpu.c_flag ) > 0xF );
    gb->cpu.c_flag = (tmp > 0xFF);
    gb->cpu.A = tmp & 0xFF;
    gb->cpu.z_flag = (gb->cpu.A == 0);
}

uint32_t adc_a_a(GB *gb) {
    adc_a_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t adc_a_b(GB *gb) {
    adc_a_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t adc_a_c(GB *gb) {
    adc_a_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t adc_a_d(GB *gb) {
    adc_a_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t adc_a_e(GB *gb) {
    adc_a_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t adc_a_h(GB *gb) {
    adc_a_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t adc_a_l(GB *gb) {
    adc_a_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t adc_a_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    adc_a_x(gb, &byte);
    return 0;
}

uint32_t adc_a_d8(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.pc-1);
    adc_a_x(gb, &byte);
    return 0;
}


static inline void sub_a_x(GB *gb, uint8_t *reg) {
    uint16_t tmp = gb->cpu.A - *reg;
    gb->cpu.n_flag = 1;
    gb->cpu.h_flag = ( (*reg & 0xF) > (gb->cpu.A & 0xF) );
    gb->cpu.c_flag = (*reg > gb->cpu.A);
    gb->cpu.A = tmp & 0xFF;
    gb->cpu.z_flag = (gb->cpu.A == 0);
}

uint32_t sub_a(GB *gb) {
    sub_a_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t sub_b(GB *gb) {
    sub_a_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t sub_c(GB *gb) {
    sub_a_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t sub_d(GB *gb) {
    sub_a_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t sub_e(GB *gb) {
    sub_a_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t sub_h(GB *gb) {
    sub_a_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t sub_l(GB *gb) {
    sub_a_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t sub_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    sub_a_x(gb, &byte);
    return 0;
}

uint32_t sub_d8(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.pc-1);
    sub_a_x(gb, &byte);
    return 0;
}


static inline void sbc_a_x(GB *gb, uint8_t *reg) {
    uint8_t old_c_flag = gb->cpu.c_flag;
    gb->cpu.n_flag = 1;
    gb->cpu.h_flag = ( ( (*reg & 0xF) + gb->cpu.c_flag) > (gb->cpu.A & 0xF) );
    gb->cpu.c_flag = (((uint16_t)*reg + gb->cpu.c_flag) > (uint16_t)gb->cpu.A);
    gb->cpu.A = gb->cpu.A - (*reg + old_c_flag);
    gb->cpu.z_flag = (gb->cpu.A == 0);
}

uint32_t sbc_a_a(GB *gb) {
    sbc_a_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t sbc_a_b(GB *gb) {
    sbc_a_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t sbc_a_c(GB *gb) {
    sbc_a_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t sbc_a_d(GB *gb) {
    sbc_a_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t sbc_a_e(GB *gb) {
    sbc_a_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t sbc_a_h(GB *gb) {
    sbc_a_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t sbc_a_l(GB *gb) {
    sbc_a_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t sbc_a_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    sbc_a_x(gb, &byte);
    return 0;
}

uint32_t sbc_a_d8(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.pc-1);
    sbc_a_x(gb, &byte);
    return 0;
}


uint32_t daa(GB *gb) {
    if (!gb->cpu.n_flag) {
        if (gb->cpu.c_flag || (gb->cpu.A > 0x99)) {
            gb->cpu.A += 0x60;
            gb->cpu.c_flag = 1;
        }
        if (gb->cpu.h_flag || ((gb->cpu.A & 0xF) > 0x09)) {
            gb->cpu.A += 0x6;
        }
    } else {
        if (gb->cpu.c_flag) {
            gb->cpu.A -= 0x60;
        }
        if (gb->cpu.h_flag) {
            gb->cpu.A -= 0x6;
        }

    }
    gb->cpu.z_flag = (gb->cpu.A == 0);
    gb->cpu.h_flag = 0;
    return 0;
}


uint32_t scf(GB *gb) {
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = 1;
    return 0;
}


uint32_t cpl(GB *gb) {
    gb->cpu.n_flag = 1;
    gb->cpu.h_flag = 1;
    gb->cpu.A = ~gb->cpu.A;
    return 0;
}


uint32_t ccf(GB *gb) {
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = !gb->cpu.c_flag;
    return 0;
}


// ##################   16bit arithmetical/logical   ##################

uint32_t inc_bc(GB *gb) {
    gb->cpu.BC += 1;
    return 0;
}

uint32_t inc_de(GB *gb) {
    gb->cpu.DE += 1;
    return 0;
}

uint32_t inc_hl(GB *gb) {
    gb->cpu.HL += 1;
    return 0;
}

uint32_t inc_sp(GB *gb) {
    gb->cpu.sp += 1;
    return 0;
}

uint32_t dec_bc(GB *gb) {
    gb->cpu.BC -= 1;
    return 0;
}

uint32_t dec_de(GB *gb) {
    gb->cpu.DE -= 1;
    return 0;
}

uint32_t dec_hl(GB *gb) {
    gb->cpu.HL -= 1;
    return 0;
}

uint32_t dec_sp(GB *gb) {
    gb->cpu.sp -= 1;
    return 0;
}


static inline void add_hl_x(GB *gb, uint16_t *reg) {
    gb->cpu.c_flag = ( ((uint32_t)gb->cpu.HL  + (uint32_t)*reg) > 0xFFFF );
    gb->cpu.h_flag = ( ((gb->cpu.HL & 0xFFF) + (*reg & 0xFFF)) > 0xFFF );
    gb->cpu.n_flag = 0;
    gb->cpu.HL += *reg;
}

uint32_t add_hl_bc(GB *gb) {
    add_hl_x(gb, &gb->cpu.BC);
    return 0;
}

uint32_t add_hl_de(GB *gb) {
    add_hl_x(gb, &gb->cpu.DE);
    return 0;
}

uint32_t add_hl_hl(GB *gb) {
    add_hl_x(gb, &gb->cpu.HL);
    return 0;
}

uint32_t add_hl_sp(GB *gb) {
    add_hl_x(gb, &gb->cpu.sp);
    return 0;
}


uint32_t add_sp_r8(GB *gb) {
    int8_t r8 = (int8_t)read_byte(gb, gb->cpu.pc-1);
    uint16_t sp_plus_r8 = gb->cpu.sp + r8;

    gb->cpu.n_flag = 0;
    gb->cpu.z_flag = 0;
    if (r8 >= 0) {
        gb->cpu.c_flag = ( ((gb->cpu.sp & 0xFF) + r8) > 0xFF );
        gb->cpu.h_flag = ( ((gb->cpu.sp & 0xF) + (r8 & 0xF)) > 0xF );
    } else {
        gb->cpu.c_flag = ( (sp_plus_r8 & 0xFF) <= (gb->cpu.sp & 0xFF) );
        gb->cpu.h_flag = ( (sp_plus_r8 & 0xF) <= (gb->cpu.sp & 0xF) );
    }

    gb->cpu.sp = sp_plus_r8;

    return 0;
}


// ############   8bit rotations/shifts/bit instructions   ############

static inline void bit_n_x(GB *gb, uint8_t bit_no, uint8_t *reg) {
    gb->cpu.z_flag = !(*reg & (0x01 << bit_no));
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 1;
}

uint32_t bit_0_a(GB *gb) {
    bit_n_x(gb, 0, &gb->cpu.A);
    return 0;
}

uint32_t bit_0_b(GB *gb) {
    bit_n_x(gb, 0, &gb->cpu.B);
    return 0;
}

uint32_t bit_0_c(GB *gb) {
    bit_n_x(gb, 0, &gb->cpu.C);
    return 0;
}

uint32_t bit_0_d(GB *gb) {
    bit_n_x(gb, 0, &gb->cpu.D);
    return 0;
}

uint32_t bit_0_e(GB *gb) {
    bit_n_x(gb, 0, &gb->cpu.E);
    return 0;
}

uint32_t bit_0_h(GB *gb) {
    bit_n_x(gb, 0, &gb->cpu.H);
    return 0;
}

uint32_t bit_0_l(GB *gb) {
    bit_n_x(gb, 0, &gb->cpu.L);
    return 0;
}

uint32_t bit_0_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    bit_n_x(gb, 0, &byte);
    return 0;
}

uint32_t bit_1_a(GB *gb) {
    bit_n_x(gb, 1, &gb->cpu.A);
    return 0;
}

uint32_t bit_1_b(GB *gb) {
    bit_n_x(gb, 1, &gb->cpu.B);
    return 0;
}

uint32_t bit_1_c(GB *gb) {
    bit_n_x(gb, 1, &gb->cpu.C);
    return 0;
}

uint32_t bit_1_d(GB *gb) {
    bit_n_x(gb, 1, &gb->cpu.D);
    return 0;
}

uint32_t bit_1_e(GB *gb) {
    bit_n_x(gb, 1, &gb->cpu.E);
    return 0;
}

uint32_t bit_1_h(GB *gb) {
    bit_n_x(gb, 1, &gb->cpu.H);
    return 0;
}

uint32_t bit_1_l(GB *gb) {
    bit_n_x(gb, 1, &gb->cpu.L);
    return 0;
}

uint32_t bit_1_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    bit_n_x(gb, 1, &byte);
    return 0;
}

uint32_t bit_2_a(GB *gb) {
    bit_n_x(gb, 2, &gb->cpu.A);
    return 0;
}

uint32_t bit_2_b(GB *gb) {
    bit_n_x(gb, 2, &gb->cpu.B);
    return 0;
}

uint32_t bit_2_c(GB *gb) {
    bit_n_x(gb, 2, &gb->cpu.C);
    return 0;
}

uint32_t bit_2_d(GB *gb) {
    bit_n_x(gb, 2, &gb->cpu.D);
    return 0;
}

uint32_t bit_2_e(GB *gb) {
    bit_n_x(gb, 2, &gb->cpu.E);
    return 0;
}

uint32_t bit_2_h(GB *gb) {
    bit_n_x(gb, 2, &gb->cpu.H);
    return 0;
}

uint32_t bit_2_l(GB *gb) {
    bit_n_x(gb, 2, &gb->cpu.L);
    return 0;
}

uint32_t bit_2_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    bit_n_x(gb, 2, &byte);
    return 0;
}

uint32_t bit_3_a(GB *gb) {
    bit_n_x(gb, 3, &gb->cpu.A);
    return 0;
}

uint32_t bit_3_b(GB *gb) {
    bit_n_x(gb, 3, &gb->cpu.B);
    return 0;
}

uint32_t bit_3_c(GB *gb) {
    bit_n_x(gb, 3, &gb->cpu.C);
    return 0;
}

uint32_t bit_3_d(GB *gb) {
    bit_n_x(gb, 3, &gb->cpu.D);
    return 0;
}

uint32_t bit_3_e(GB *gb) {
    bit_n_x(gb, 3, &gb->cpu.E);
    return 0;
}

uint32_t bit_3_h(GB *gb) {
    bit_n_x(gb, 3, &gb->cpu.H);
    return 0;
}

uint32_t bit_3_l(GB *gb) {
    bit_n_x(gb, 3, &gb->cpu.L);
    return 0;
}

uint32_t bit_3_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    bit_n_x(gb, 3, &byte);
    return 0;
}

uint32_t bit_4_a(GB *gb) {
    bit_n_x(gb, 4, &gb->cpu.A);
    return 0;
}

uint32_t bit_4_b(GB *gb) {
    bit_n_x(gb, 4, &gb->cpu.B);
    return 0;
}

uint32_t bit_4_c(GB *gb) {
    bit_n_x(gb, 4, &gb->cpu.C);
    return 0;
}

uint32_t bit_4_d(GB *gb) {
    bit_n_x(gb, 4, &gb->cpu.D);
    return 0;
}

uint32_t bit_4_e(GB *gb) {
    bit_n_x(gb, 4, &gb->cpu.E);
    return 0;
}

uint32_t bit_4_h(GB *gb) {
    bit_n_x(gb, 4, &gb->cpu.H);
    return 0;
}

uint32_t bit_4_l(GB *gb) {
    bit_n_x(gb, 4, &gb->cpu.L);
    return 0;
}

uint32_t bit_4_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    bit_n_x(gb, 4, &byte);
    return 0;
}

uint32_t bit_5_a(GB *gb) {
    bit_n_x(gb, 5, &gb->cpu.A);
    return 0;
}

uint32_t bit_5_b(GB *gb) {
    bit_n_x(gb, 5, &gb->cpu.B);
    return 0;
}

uint32_t bit_5_c(GB *gb) {
    bit_n_x(gb, 5, &gb->cpu.C);
    return 0;
}

uint32_t bit_5_d(GB *gb) {
    bit_n_x(gb, 5, &gb->cpu.D);
    return 0;
}

uint32_t bit_5_e(GB *gb) {
    bit_n_x(gb, 5, &gb->cpu.E);
    return 0;
}

uint32_t bit_5_h(GB *gb) {
    bit_n_x(gb, 5, &gb->cpu.H);
    return 0;
}

uint32_t bit_5_l(GB *gb) {
    bit_n_x(gb, 5, &gb->cpu.L);
    return 0;
}

uint32_t bit_5_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    bit_n_x(gb, 5, &byte);
    return 0;
}

uint32_t bit_6_a(GB *gb) {
    bit_n_x(gb, 6, &gb->cpu.A);
    return 0;
}

uint32_t bit_6_b(GB *gb) {
    bit_n_x(gb, 6, &gb->cpu.B);
    return 0;
}

uint32_t bit_6_c(GB *gb) {
    bit_n_x(gb, 6, &gb->cpu.C);
    return 0;
}

uint32_t bit_6_d(GB *gb) {
    bit_n_x(gb, 6, &gb->cpu.D);
    return 0;
}

uint32_t bit_6_e(GB *gb) {
    bit_n_x(gb, 6, &gb->cpu.E);
    return 0;
}

uint32_t bit_6_h(GB *gb) {
    bit_n_x(gb, 6, &gb->cpu.H);
    return 0;
}

uint32_t bit_6_l(GB *gb) {
    bit_n_x(gb, 6, &gb->cpu.L);
    return 0;
}

uint32_t bit_6_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    bit_n_x(gb, 6, &byte);
    return 0;
}

uint32_t bit_7_a(GB *gb) {
    bit_n_x(gb, 7, &gb->cpu.A);
    return 0;
}

uint32_t bit_7_b(GB *gb) {
    bit_n_x(gb, 7, &gb->cpu.B);
    return 0;
}

uint32_t bit_7_c(GB *gb) {
    bit_n_x(gb, 7, &gb->cpu.C);
    return 0;
}

uint32_t bit_7_d(GB *gb) {
    bit_n_x(gb, 7, &gb->cpu.D);
    return 0;
}

uint32_t bit_7_e(GB *gb) {
    bit_n_x(gb, 7, &gb->cpu.E);
    return 0;
}

uint32_t bit_7_h(GB *gb) {
    bit_n_x(gb, 7, &gb->cpu.H);
    return 0;
}

uint32_t bit_7_l(GB *gb) {
    bit_n_x(gb, 7, &gb->cpu.L);
    return 0;
}

uint32_t bit_7_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    bit_n_x(gb, 7, &byte);
    return 0;
}


uint32_t res_0_a(GB *gb) {
    gb->cpu.A &= 0xFE;
    return 0;
}

uint32_t res_0_b(GB *gb) {
    gb->cpu.B &= 0xFE;
    return 0;
}

uint32_t res_0_c(GB *gb) {
    gb->cpu.C &= 0xFE;
    return 0;
}

uint32_t res_0_d(GB *gb) {
    gb->cpu.D &= 0xFE;
    return 0;
}

uint32_t res_0_e(GB *gb) {
    gb->cpu.E &= 0xFE;
    return 0;
}

uint32_t res_0_h(GB *gb) {
    gb->cpu.H &= 0xFE;
    return 0;
}

uint32_t res_0_l(GB *gb) {
    gb->cpu.L &= 0xFE;
    return 0;
}

uint32_t res_0_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) & 0xFE);
    return 0;
}

uint32_t res_1_a(GB *gb) {
    gb->cpu.A &= 0xFD;
    return 0;
}

uint32_t res_1_b(GB *gb) {
    gb->cpu.B &= 0xFD;
    return 0;
}

uint32_t res_1_c(GB *gb) {
    gb->cpu.C &= 0xFD;
    return 0;
}

uint32_t res_1_d(GB *gb) {
    gb->cpu.D &= 0xFD;
    return 0;
}

uint32_t res_1_e(GB *gb) {
    gb->cpu.E &= 0xFD;
    return 0;
}

uint32_t res_1_h(GB *gb) {
    gb->cpu.H &= 0xFD;
    return 0;
}

uint32_t res_1_l(GB *gb) {
    gb->cpu.L &= 0xFD;
    return 0;
}

uint32_t res_1_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) & 0xFD);
    return 0;
}

uint32_t res_2_a(GB *gb) {
    gb->cpu.A &= 0xFB;
    return 0;
}

uint32_t res_2_b(GB *gb) {
    gb->cpu.B &= 0xFB;
    return 0;
}

uint32_t res_2_c(GB *gb) {
    gb->cpu.C &= 0xFB;
    return 0;
}

uint32_t res_2_d(GB *gb) {
    gb->cpu.D &= 0xFB;
    return 0;
}

uint32_t res_2_e(GB *gb) {
    gb->cpu.E &= 0xFB;
    return 0;
}

uint32_t res_2_h(GB *gb) {
    gb->cpu.H &= 0xFB;
    return 0;
}

uint32_t res_2_l(GB *gb) {
    gb->cpu.L &= 0xFB;
    return 0;
}

uint32_t res_2_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) & 0xFB);
    return 0;
}

uint32_t res_3_a(GB *gb) {
    gb->cpu.A &= 0xF7;
    return 0;
}

uint32_t res_3_b(GB *gb) {
    gb->cpu.B &= 0xF7;
    return 0;
}

uint32_t res_3_c(GB *gb) {
    gb->cpu.C &= 0xF7;
    return 0;
}

uint32_t res_3_d(GB *gb) {
    gb->cpu.D &= 0xF7;
    return 0;
}

uint32_t res_3_e(GB *gb) {
    gb->cpu.E &= 0xF7;
    return 0;
}

uint32_t res_3_h(GB *gb) {
    gb->cpu.H &= 0xF7;
    return 0;
}

uint32_t res_3_l(GB *gb) {
    gb->cpu.L &= 0xF7;
    return 0;
}

uint32_t res_3_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) & 0xF7);
    return 0;
}

uint32_t res_4_a(GB *gb) {
    gb->cpu.A &= 0xEF;
    return 0;
}

uint32_t res_4_b(GB *gb) {
    gb->cpu.B &= 0xEF;
    return 0;
}

uint32_t res_4_c(GB *gb) {
    gb->cpu.C &= 0xEF;
    return 0;
}

uint32_t res_4_d(GB *gb) {
    gb->cpu.D &= 0xEF;
    return 0;
}

uint32_t res_4_e(GB *gb) {
    gb->cpu.E &= 0xEF;
    return 0;
}

uint32_t res_4_h(GB *gb) {
    gb->cpu.H &= 0xEF;
    return 0;
}

uint32_t res_4_l(GB *gb) {
    gb->cpu.L &= 0xEF;
    return 0;
}

uint32_t res_4_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) & 0xEF);
    return 0;
}

uint32_t res_5_a(GB *gb) {
    gb->cpu.A &= 0xDF;
    return 0;
}

uint32_t res_5_b(GB *gb) {
    gb->cpu.B &= 0xDF;
    return 0;
}

uint32_t res_5_c(GB *gb) {
    gb->cpu.C &= 0xDF;
    return 0;
}

uint32_t res_5_d(GB *gb) {
    gb->cpu.D &= 0xDF;
    return 0;
}

uint32_t res_5_e(GB *gb) {
    gb->cpu.E &= 0xDF;
    return 0;
}

uint32_t res_5_h(GB *gb) {
    gb->cpu.H &= 0xDF;
    return 0;
}

uint32_t res_5_l(GB *gb) {
    gb->cpu.L &= 0xDF;
    return 0;
}

uint32_t res_5_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) & 0xDF);
    return 0;
}

uint32_t res_6_a(GB *gb) {
    gb->cpu.A &= 0xBF;
    return 0;
}

uint32_t res_6_b(GB *gb) {
    gb->cpu.B &= 0xBF;
    return 0;
}

uint32_t res_6_c(GB *gb) {
    gb->cpu.C &= 0xBF;
    return 0;
}

uint32_t res_6_d(GB *gb) {
    gb->cpu.D &= 0xBF;
    return 0;
}

uint32_t res_6_e(GB *gb) {
    gb->cpu.E &= 0xBF;
    return 0;
}

uint32_t res_6_h(GB *gb) {
    gb->cpu.H &= 0xBF;
    return 0;
}

uint32_t res_6_l(GB *gb) {
    gb->cpu.L &= 0xBF;
    return 0;
}

uint32_t res_6_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) & 0xBF);
    return 0;
}

uint32_t res_7_a(GB *gb) {
    gb->cpu.A &= 0x7F;
    return 0;
}

uint32_t res_7_b(GB *gb) {
    gb->cpu.B &= 0x7F;
    return 0;
}

uint32_t res_7_c(GB *gb) {
    gb->cpu.C &= 0x7F;
    return 0;
}

uint32_t res_7_d(GB *gb) {
    gb->cpu.D &= 0x7F;
    return 0;
}

uint32_t res_7_e(GB *gb) {
    gb->cpu.E &= 0x7F;
    return 0;
}

uint32_t res_7_h(GB *gb) {
    gb->cpu.H &= 0x7F;
    return 0;
}

uint32_t res_7_l(GB *gb) {
    gb->cpu.L &= 0x7F;
    return 0;
}

uint32_t res_7_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) & 0x7F);
    return 0;
}

uint32_t set_0_a(GB *gb) {
    gb->cpu.A |= 0x01;
    return 0;
}

uint32_t set_0_b(GB *gb) {
    gb->cpu.B |= 0x01;
    return 0;
}

uint32_t set_0_c(GB *gb) {
    gb->cpu.C |= 0x01;
    return 0;
}

uint32_t set_0_d(GB *gb) {
    gb->cpu.D |= 0x01;
    return 0;
}

uint32_t set_0_e(GB *gb) {
    gb->cpu.E |= 0x01;
    return 0;
}

uint32_t set_0_h(GB *gb) {
    gb->cpu.H |= 0x01;
    return 0;
}

uint32_t set_0_l(GB *gb) {
    gb->cpu.L |= 0x01;
    return 0;
}

uint32_t set_0_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) | 0x01);
    return 0;
}

uint32_t set_1_a(GB *gb) {
    gb->cpu.A |= 0x02;
    return 0;
}

uint32_t set_1_b(GB *gb) {
    gb->cpu.B |= 0x02;
    return 0;
}

uint32_t set_1_c(GB *gb) {
    gb->cpu.C |= 0x02;
    return 0;
}

uint32_t set_1_d(GB *gb) {
    gb->cpu.D |= 0x02;
    return 0;
}

uint32_t set_1_e(GB *gb) {
    gb->cpu.E |= 0x02;
    return 0;
}

uint32_t set_1_h(GB *gb) {
    gb->cpu.H |= 0x02;
    return 0;
}

uint32_t set_1_l(GB *gb) {
    gb->cpu.L |= 0x02;
    return 0;
}

uint32_t set_1_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) | 0x02);
    return 0;
}

uint32_t set_2_a(GB *gb) {
    gb->cpu.A |= 0x04;
    return 0;
}

uint32_t set_2_b(GB *gb) {
    gb->cpu.B |= 0x04;
    return 0;
}

uint32_t set_2_c(GB *gb) {
    gb->cpu.C |= 0x04;
    return 0;
}

uint32_t set_2_d(GB *gb) {
    gb->cpu.D |= 0x04;
    return 0;
}

uint32_t set_2_e(GB *gb) {
    gb->cpu.E |= 0x04;
    return 0;
}

uint32_t set_2_h(GB *gb) {
    gb->cpu.H |= 0x04;
    return 0;
}

uint32_t set_2_l(GB *gb) {
    gb->cpu.L |= 0x04;
    return 0;
}

uint32_t set_2_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) | 0x04);
    return 0;
}

uint32_t set_3_a(GB *gb) {
    gb->cpu.A |= 0x08;
    return 0;
}

uint32_t set_3_b(GB *gb) {
    gb->cpu.B |= 0x08;
    return 0;
}

uint32_t set_3_c(GB *gb) {
    gb->cpu.C |= 0x08;
    return 0;
}

uint32_t set_3_d(GB *gb) {
    gb->cpu.D |= 0x08;
    return 0;
}

uint32_t set_3_e(GB *gb) {
    gb->cpu.E |= 0x08;
    return 0;
}

uint32_t set_3_h(GB *gb) {
    gb->cpu.H |= 0x08;
    return 0;
}

uint32_t set_3_l(GB *gb) {
    gb->cpu.L |= 0x08;
    return 0;
}

uint32_t set_3_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) | 0x08);
    return 0;
}

uint32_t set_4_a(GB *gb) {
    gb->cpu.A |= 0x10;
    return 0;
}

uint32_t set_4_b(GB *gb) {
    gb->cpu.B |= 0x10;
    return 0;
}

uint32_t set_4_c(GB *gb) {
    gb->cpu.C |= 0x10;
    return 0;
}

uint32_t set_4_d(GB *gb) {
    gb->cpu.D |= 0x10;
    return 0;
}

uint32_t set_4_e(GB *gb) {
    gb->cpu.E |= 0x10;
    return 0;
}

uint32_t set_4_h(GB *gb) {
    gb->cpu.H |= 0x10;
    return 0;
}

uint32_t set_4_l(GB *gb) {
    gb->cpu.L |= 0x10;
    return 0;
}

uint32_t set_4_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) | 0x10);
    return 0;
}

uint32_t set_5_a(GB *gb) {
    gb->cpu.A |= 0x20;
    return 0;
}

uint32_t set_5_b(GB *gb) {
    gb->cpu.B |= 0x20;
    return 0;
}

uint32_t set_5_c(GB *gb) {
    gb->cpu.C |= 0x20;
    return 0;
}

uint32_t set_5_d(GB *gb) {
    gb->cpu.D |= 0x20;
    return 0;
}

uint32_t set_5_e(GB *gb) {
    gb->cpu.E |= 0x20;
    return 0;
}

uint32_t set_5_h(GB *gb) {
    gb->cpu.H |= 0x20;
    return 0;
}

uint32_t set_5_l(GB *gb) {
    gb->cpu.L |= 0x20;
    return 0;
}

uint32_t set_5_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) | 0x20);
    return 0;
}

uint32_t set_6_a(GB *gb) {
    gb->cpu.A |= 0x40;
    return 0;
}

uint32_t set_6_b(GB *gb) {
    gb->cpu.B |= 0x40;
    return 0;
}

uint32_t set_6_c(GB *gb) {
    gb->cpu.C |= 0x40;
    return 0;
}

uint32_t set_6_d(GB *gb) {
    gb->cpu.D |= 0x40;
    return 0;
}

uint32_t set_6_e(GB *gb) {
    gb->cpu.E |= 0x40;
    return 0;
}

uint32_t set_6_h(GB *gb) {
    gb->cpu.H |= 0x40;
    return 0;
}

uint32_t set_6_l(GB *gb) {
    gb->cpu.L |= 0x40;
    return 0;
}

uint32_t set_6_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) | 0x40);
    return 0;
}

uint32_t set_7_a(GB *gb) {
    gb->cpu.A |= 0x80;
    return 0;
}

uint32_t set_7_b(GB *gb) {
    gb->cpu.B |= 0x80;
    return 0;
}

uint32_t set_7_c(GB *gb) {
    gb->cpu.C |= 0x80;
    return 0;
}

uint32_t set_7_d(GB *gb) {
    gb->cpu.D |= 0x80;
    return 0;
}

uint32_t set_7_e(GB *gb) {
    gb->cpu.E |= 0x80;
    return 0;
}

uint32_t set_7_h(GB *gb) {
    gb->cpu.H |= 0x80;
    return 0;
}

uint32_t set_7_l(GB *gb) {
    gb->cpu.L |= 0x80;
    return 0;
}

uint32_t set_7_hl(GB *gb) {
    write_byte(gb, gb->cpu.HL, read_byte(gb, gb->cpu.HL) | 0x80);
    return 0;
}


static inline void rlc_x(GB *gb, uint8_t *reg) {
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = (*reg & 0x80) >> 7;
    *reg <<= 1;
    *reg |= gb->cpu.c_flag;
    gb->cpu.z_flag = (*reg == 0);
}

uint32_t rlc_a(GB *gb) {
    rlc_x(gb, &gb->cpu.A);    
    return 0;
}

uint32_t rlc_b(GB *gb) {
    rlc_x(gb, &gb->cpu.B);    
    return 0;
}

uint32_t rlc_c(GB *gb) {
    rlc_x(gb, &gb->cpu.C);    
    return 0;
}

uint32_t rlc_d(GB *gb) {
    rlc_x(gb, &gb->cpu.D);    
    return 0;
}

uint32_t rlc_e(GB *gb) {
    rlc_x(gb, &gb->cpu.E);    
    return 0;
}

uint32_t rlc_h(GB *gb) {
    rlc_x(gb, &gb->cpu.H);    
    return 0;
}

uint32_t rlc_l(GB *gb) {
    rlc_x(gb, &gb->cpu.L);    
    return 0;
}

uint32_t rlc_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    rlc_x(gb, &byte);
    write_byte(gb, gb->cpu.HL, byte);  
    return 0;
}


uint32_t rlca(GB *gb) {
    gb->cpu.z_flag = 0;
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = (gb->cpu.A & 0x80) >> 7;

    gb->cpu.A <<= 1;
    gb->cpu.A |= gb->cpu.c_flag;
    
    return 0;
}


static inline void rrc_x(GB *gb, uint8_t *reg) {
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = (*reg & 0x01);
    *reg >>= 1;
    *reg |= gb->cpu.c_flag << 7;
    gb->cpu.z_flag = (*reg == 0);
}

uint32_t rrc_a(GB *gb) {
    rrc_x(gb, &gb->cpu.A);    
    return 0;
}

uint32_t rrc_b(GB *gb) {
    rrc_x(gb, &gb->cpu.B);    
    return 0;
}

uint32_t rrc_c(GB *gb) {
    rrc_x(gb, &gb->cpu.C);    
    return 0;
}

uint32_t rrc_d(GB *gb) {
    rrc_x(gb, &gb->cpu.D);    
    return 0;
}

uint32_t rrc_e(GB *gb) {
    rrc_x(gb, &gb->cpu.E);    
    return 0;
}

uint32_t rrc_h(GB *gb) {
    rrc_x(gb, &gb->cpu.H);    
    return 0;
}

uint32_t rrc_l(GB *gb) {
    rrc_x(gb, &gb->cpu.L);    
    return 0;
}

uint32_t rrc_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    rrc_x(gb, &byte); 
    write_byte(gb, gb->cpu.HL, byte);   
    return 0;
}


uint32_t rrca(GB *gb) {
    gb->cpu.z_flag = 0;
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = (gb->cpu.A & 0x01);

    gb->cpu.A >>= 1;
    gb->cpu.A |= gb->cpu.c_flag << 7;
    
    return 0;
}


static inline void rl_x(GB *gb, uint8_t *reg) {
    uint8_t carry_flag = gb->cpu.c_flag;
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = (*reg & 0x80) >> 7;
    *reg <<= 1;
    *reg |= carry_flag;
    gb->cpu.z_flag = (*reg == 0);
}

uint32_t rl_a(GB *gb) {
    rl_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t rl_b(GB *gb) {
    rl_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t rl_c(GB *gb) {
    rl_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t rl_d(GB *gb) {
    rl_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t rl_e(GB *gb) {
    rl_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t rl_h(GB *gb) {
    rl_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t rl_l(GB *gb) {
    rl_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t rl_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    rl_x(gb, &byte);
    write_byte(gb, gb->cpu.HL, byte);
    return 0;
}


uint32_t rla(GB *gb) {
    uint8_t carry_flag = gb->cpu.c_flag;
    gb->cpu.z_flag = 0;
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = (gb->cpu.A & 0x80) >> 7;

    gb->cpu.A <<= 1;
    gb->cpu.A |= carry_flag;
    
    return 0;
}


static inline void rr_x(GB *gb, uint8_t *reg) {
    uint8_t carry_flag = gb->cpu.c_flag;
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = (*reg & 0x01);
    *reg >>= 1;
    *reg |= carry_flag << 7;
    gb->cpu.z_flag = (*reg == 0);
}

uint32_t rr_a(GB *gb) {
    rr_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t rr_b(GB *gb) {
    rr_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t rr_c(GB *gb) {
    rr_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t rr_d(GB *gb) {
    rr_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t rr_e(GB *gb) {
    rr_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t rr_h(GB *gb) {
    rr_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t rr_l(GB *gb) {
    rr_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t rr_hl(GB *gb) {
    uint8_t carry_flag = gb->cpu.c_flag, byte = read_byte(gb, gb->cpu.HL);
    rr_x(gb, &byte);
    write_byte(gb, gb->cpu.HL, byte);
    return 0;
}


uint32_t rra(GB *gb) {
    uint8_t carry_flag = gb->cpu.c_flag;
    gb->cpu.z_flag = 0;
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = (gb->cpu.A & 0x01);

    gb->cpu.A >>= 1;
    gb->cpu.A |= carry_flag << 7;
    
    return 0;
}


static inline void sla_x(GB *gb, uint8_t *res) {
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = (*res & 0x80) >> 7;
    *res <<= 1;
    gb->cpu.z_flag = (*res == 0);
}

uint32_t sla_a(GB *gb) {
    sla_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t sla_b(GB *gb) {
    sla_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t sla_c(GB *gb) {
    sla_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t sla_d(GB *gb) {
    sla_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t sla_e(GB *gb) {
    sla_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t sla_h(GB *gb) {
    sla_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t sla_l(GB *gb) {
    sla_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t sla_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    sla_x(gb, &byte);
    write_byte(gb, gb->cpu.HL, byte);
    return 0;
}


static inline void sra_x(GB *gb, uint8_t *res) {
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = (*res & 0x01);
    *res = (int8_t)*res >> 1;
    gb->cpu.z_flag = (*res == 0);
}

uint32_t sra_a(GB *gb) {
    sra_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t sra_b(GB *gb) {
    sra_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t sra_c(GB *gb) {
    sra_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t sra_d(GB *gb) {
    sra_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t sra_e(GB *gb) {
    sra_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t sra_h(GB *gb) {
    sra_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t sra_l(GB *gb) {
    sra_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t sra_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    sra_x(gb, &byte);
    write_byte(gb, gb->cpu.HL, byte);
    return 0;
}


static inline void srl_x(GB *gb, uint8_t *res) {
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = (*res & 0x01);
    *res >>= 1;
    gb->cpu.z_flag = (*res == 0);
}

uint32_t srl_a(GB *gb) {
    srl_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t srl_b(GB *gb) {
    srl_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t srl_c(GB *gb) {
    srl_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t srl_d(GB *gb) {
    srl_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t srl_e(GB *gb) {
    srl_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t srl_h(GB *gb) {
    srl_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t srl_l(GB *gb) {
    srl_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t srl_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    srl_x(gb, &byte);
    write_byte(gb, gb->cpu.HL, byte);
    return 0;
}


static inline void swap_x(GB *gb, uint8_t *reg) {
    uint8_t tmp = *reg & 0xF0;
    *reg <<= 4;
    *reg |= (tmp >> 4);
    gb->cpu.n_flag = 0;
    gb->cpu.h_flag = 0;
    gb->cpu.c_flag = 0;
    gb->cpu.z_flag = (*reg == 0);
}

uint32_t swap_a(GB *gb) {
    swap_x(gb, &gb->cpu.A);
    return 0;
}

uint32_t swap_b(GB *gb) {
    swap_x(gb, &gb->cpu.B);
    return 0;
}

uint32_t swap_c(GB *gb) {
    swap_x(gb, &gb->cpu.C);
    return 0;
}

uint32_t swap_d(GB *gb) {
    swap_x(gb, &gb->cpu.D);
    return 0;
}

uint32_t swap_e(GB *gb) {
    swap_x(gb, &gb->cpu.E);
    return 0;
}

uint32_t swap_h(GB *gb) {
    swap_x(gb, &gb->cpu.H);
    return 0;
}

uint32_t swap_l(GB *gb) {
    swap_x(gb, &gb->cpu.L);
    return 0;
}

uint32_t swap_hl(GB *gb) {
    uint8_t byte = read_byte(gb, gb->cpu.HL);
    swap_x(gb, &byte);
    write_byte(gb, gb->cpu.HL, byte);
    return 0;
}


const Instruction instructions[256] = {

/*0x0*/ {"NOP", 1, 4, nop},                 {"LD BC,d16", 3, 12, ld_bc_d16},    {"LD (BC),A", 1, 8, ld_bc_a},       {"INC BC", 1, 8, inc_bc},
        {"INC B", 1, 4, inc_b},             {"DEC B", 1, 4, dec_b},             {"LD B,d8", 2, 8, ld_b_d8},         {"RLCA", 1, 4, rlca},
        {"LD (a16),SP", 3, 20, ld_a16_sp},  {"ADD HL,BC", 1, 8, add_hl_bc},     {"LD A,(BC)", 1, 8, ld_a_bc},       {"DEC BC", 1, 8, dec_bc},
        {"INC C", 1, 4, inc_c},             {"DEC C", 1, 4, dec_c},             {"LD C,d8", 2, 8, ld_c_d8},         {"RRCA", 1, 4, rrca},

/*0x1*/ {"STOP 0", 2, 4, NULL},             {"LD DE,d16", 3, 12, ld_de_d16},    {"LD (DE),A", 1, 8, ld_de_a},       {"INC DE", 1, 8, inc_de},
        {"INC D", 1, 4, inc_d},             {"DEC D", 1, 4, dec_d},             {"LD D,d8", 2, 8, ld_d_d8},         {"RLA", 1, 4, rla},
        {"JR r8", 2, 12, jr_r8},            {"ADD HL,DE", 1, 8, add_hl_de},     {"LD A,(DE)", 1, 8, ld_a_de},       {"DEC DE", 1, 8, dec_de},
        {"INC E", 1, 4, inc_e},             {"DEC E", 1, 4, dec_e},             {"LD E,d8", 2, 8, ld_e_d8},         {"RRA", 1, 4, rra},

/*0x2*/ {"JR NZ,r8", 2, 8, jr_nz_r8},       {"LD HL,d16", 3, 12, ld_hl_d16},    {"LD (HL+),A", 1, 8, ld_hlplus_a},  {"INC HL", 1, 8, inc_hl},
        {"INC H", 1, 4, inc_h},             {"DEC H", 1, 4, dec_h},             {"LD H,d8", 2, 8, ld_h_d8},         {"DAA", 1, 4, daa},
        {"JR Z,r8", 2, 8, jr_z_r8},         {"ADD HL, HL", 1, 8, add_hl_hl},    {"LD A,(HL+)", 1, 8, ld_a_hlplus},  {"DEC HL", 1, 8, dec_hl},
        {"INC L", 1, 4, inc_l},             {"DEC L", 1, 4, dec_l},             {"LD L,d8", 2, 8, ld_l_d8},         {"CPL", 1, 4, cpl},

/*0x3*/ {"JR NC,r8", 2, 8, jr_nc_r8},       {"LD SP,d16", 3, 12, ld_sp_d16},    {"LD (HL-),A", 1, 8, ld_hlminus_a}, {"INC SP", 1, 8, inc_sp},
        {"INC (HL)", 1, 12, inc_at_hl},     {"DEC (HL)", 1, 12, dec_at_hl},     {"LD (HL),d8", 2, 12, ld_hl_d8},    {"SCF", 1, 4, scf},
        {"JR C,r8", 2, 8, jr_c_r8},         {"ADD HL,SP", 1, 8, add_hl_sp},     {"LD A,(HL-)", 1, 8, ld_a_hlminus}, {"DEC SP", 1, 8, dec_sp},
        {"INC A", 1, 4, inc_a},             {"DEC A", 1, 4, dec_a},             {"LD A,d8", 2, 8, ld_a_d8},         {"CCF", 1, 4, ccf},

/*0x4*/ {"LD B,B", 1, 4, ld_b_b},           {"LD B,C", 1, 4, ld_b_c},           {"LD B,D", 1, 4, ld_b_d},           {"LD B,E", 1, 4, ld_b_e},
        {"LD B,H", 1, 4, ld_b_h},           {"LD B,L", 1, 4, ld_b_l},           {"LD B,(HL)", 1, 8, ld_b_hl},       {"LD B,A", 1, 4, ld_b_a},
        {"LD C,B", 1, 4, ld_c_b},           {"LD C,C", 1, 4, ld_c_c},           {"LD C,D", 1, 4, ld_c_d},           {"LD C,E", 1, 4, ld_c_e},
        {"LD C,H", 1, 4, ld_c_h},           {"LD C,L", 1, 4, ld_c_l},           {"LD C,(HL)", 1, 8, ld_c_hl},       {"LD C,A", 1, 4, ld_c_a},

/*0x5*/ {"LD D,B", 1, 4, ld_d_b},           {"LD D,C", 1, 4, ld_d_c},           {"LD D,D", 1, 4, ld_d_d},           {"LD D,E", 1, 4, ld_d_e},
        {"LD D,H", 1, 4, ld_d_h},           {"LD D,L", 1, 4, ld_d_l},           {"LD D,(HL)", 1, 8, ld_d_hl},       {"LD D,A", 1, 4, ld_d_a},
        {"LD E,B", 1, 4, ld_e_b},           {"LD E,C", 1, 4, ld_e_c},           {"LD E,D", 1, 4, ld_e_d},           {"LD E,E", 1, 4, ld_e_e},
        {"LD E,H", 1, 4, ld_e_h},           {"LD E,L", 1, 4, ld_e_l},           {"LD E,(HL)", 1, 8, ld_e_hl},       {"LD E,A", 1, 4, ld_e_a},

/*0x6*/ {"LD H,B", 1, 4, ld_h_b},           {"LD H,C", 1, 4, ld_h_c},           {"LD H,D", 1, 4, ld_h_d},           {"LD H,E", 1, 4, ld_h_e},
        {"LD H,H", 1, 4, ld_h_h},           {"LD H,L", 1, 4, ld_h_l},           {"LD H,(HL)", 1, 8, ld_h_hl},       {"LD H,A", 1, 4, ld_h_a},
        {"LD L,B", 1, 4, ld_l_b},           {"LD L,C", 1, 4, ld_l_c},           {"LD L,D", 1, 4, ld_l_d},           {"LD L,E", 1, 4, ld_l_e},
        {"LD L,H", 1, 4, ld_l_h},           {"LD L,L", 1, 4, ld_l_l},           {"LD L,(HL)", 1, 8, ld_l_hl},       {"LD L,A", 1, 4, ld_l_a},

/*0x7*/ {"LD (HL),B", 1, 8, ld_hl_b},       {"LD (HL),C", 1, 8, ld_hl_c},       {"LD (HL),D", 1, 8, ld_hl_d},       {"LD (HL),E", 1, 8, ld_hl_e},
        {"LD (HL),H", 1, 8, ld_hl_h},       {"LD (HL),L", 1, 8, ld_hl_l},       {"HALT", 1, 4, halt},               {"LD (HL),A", 1, 8, ld_hl_a},
        {"LD A,B", 1, 4, ld_a_b},           {"LD A,C", 1, 4, ld_a_c},           {"LD A,D", 1, 4, ld_a_d},           {"LD A,E", 1, 4, ld_a_e},
        {"LD A,H", 1, 4, ld_a_h},           {"LD A,L", 1, 4, ld_a_l},           {"LD A,(HL)", 1, 8, ld_a_hl},       {"LD A,A", 1, 4, ld_a_a},

/*0x8*/ {"ADD A,B", 1, 4, add_a_b},         {"ADD A,C", 1, 4, add_a_c},         {"ADD A,D", 1, 4, add_a_d},         {"ADD A,E", 1, 4, add_a_e},
        {"ADD A,H", 1, 4, add_a_h},         {"ADD A,L", 1, 4, add_a_l},         {"ADD A,(HL)", 1, 8, add_a_hl},     {"ADD A,A", 1, 4, add_a_a},
        {"ADC A,B", 1, 4, adc_a_b},         {"ADC A,C", 1, 4, adc_a_c},         {"ADC A,D", 1, 4, adc_a_d},         {"ADC A,E", 1, 4, adc_a_e},
        {"ADC A,H", 1, 4, adc_a_h},         {"ADC A,L", 1, 4, adc_a_l},         {"ADC A,(HL)", 1, 8, adc_a_hl},     {"ADC A,A", 1, 4, adc_a_a},
        
/*0x9*/ {"SUB B", 1, 4, sub_b},             {"SUB C", 1, 4, sub_c},             {"SUB D", 1, 4, sub_d},             {"SUB E", 1, 4, sub_e},
        {"SUB H", 1, 4, sub_h},             {"SUB L", 1, 4, sub_l},             {"SUB (HL)", 1, 8, sub_hl},         {"SUB A", 1, 4, sub_a},
        {"SBC A,B", 1, 4, sbc_a_b},         {"SBC A,C", 1, 4, sbc_a_c},         {"SBC A,D", 1, 4, sbc_a_d},         {"SBC A,E", 1, 4, sbc_a_e},
        {"SBC A,H", 1, 4, sbc_a_h},         {"SBC A,L", 1, 4, sbc_a_l},         {"SBC A,(HL)", 1, 8, sbc_a_hl},     {"SBC A,A", 1, 4, sbc_a_a},

/*0xA*/ {"AND B", 1, 4, and_b},             {"AND C", 1, 4, and_c},             {"AND D", 1, 4, and_d},             {"AND E", 1, 4, and_e},
        {"AND H", 1, 4, and_h},             {"AND L", 1, 4, and_l},             {"AND (HL)", 1, 8, and_hl},         {"AND A", 1, 4, and_a},
        {"XOR B", 1, 4, xor_b},             {"XOR C", 1, 4, xor_c},             {"XOR D", 1, 4, xor_d},             {"XOR E", 1, 4, xor_e},
        {"XOR H", 1, 4, xor_h},             {"XOR L", 1, 4, xor_l},             {"XOR (HL)", 1, 8, xor_hl},         {"XOR A", 1, 4, xor_a},
        
/*0xB*/ {"OR B", 1, 4, or_b},               {"OR C", 1, 4, or_c},               {"OR D", 1, 4, or_d},               {"OR E", 1, 4, or_e},
        {"OR H", 1, 4, or_h},               {"OR L", 1, 4, or_l},               {"OR (HL)", 1, 8, or_hl},           {"OR A", 1, 4, or_a},
        {"CP B", 1, 4, cp_b},               {"CP C", 1, 4, cp_c},               {"CP D", 1, 4, cp_d},               {"CP E", 1, 4, cp_e},
        {"CP H", 1, 4, cp_h},               {"CP L", 1, 4, cp_l},               {"CP (HL)", 1, 8, cp_hl},           {"CP A", 1, 4, cp_a},

/*0xC*/ {"RET NZ", 1, 8, ret_nz},           {"POP BC", 1, 12, pop_bc},          {"JP NZ,a16", 3, 12, jp_nz_a16},    {"JP a16", 3, 16, jp_a16},
        {"CALL NZ,a16", 3, 12, call_nz_a16},{"PUSH BC", 1, 16, push_bc},        {"ADD A,d8", 2, 8, add_a_d8},       {"RST 00H", 1, 16, rst_00h},
        {"RET Z", 1, 8, ret_z},             {"RET", 1, 16, ret},                {"JP Z,a16", 3, 12, jp_z_a16},      {"Prefix CB", 2, 4, NULL},
        {"CALL z,a16", 3, 12, call_z_a16},  {"CALL a16", 3, 24, call_a16},      {"ADC A,d8", 2, 8, adc_a_d8},       {"RST 08H", 1, 16, rst_08h},

/*0xD*/ {"RET NC", 1, 8, ret_nc},           {"POP DE", 1, 12, pop_de},          {"JP NC,a16", 3, 12, jp_nc_a16},    {"Illegal 0xD3C", 1, 1, NULL},
        {"CALL NC,a16", 3, 12, call_nc_a16},{"PUSH DE", 1, 16, push_de},        {"SUB d8", 2, 8, sub_d8},           {"RST 10H", 1, 16, rst_10h},
        {"RET C", 1, 8, ret_c},             {"RETI", 1, 16, reti},              {"JP C,a16", 3, 12, jp_c_a16},      {"Illegal 0xDB", 1, 1, NULL},
        {"CALL C,a16", 3, 12, call_c_a16},  {"Illegal 0xDD", 1, 1, NULL},       {"SBC A,d8", 2, 8, sbc_a_d8},       {"RST 18H", 1, 16, rst_18h},

/*0xE*/ {"LDH (a8),A", 2, 12, ldh_a8_plus_ff00_a}, {"POP HL", 1, 12, pop_hl},   {"LD (C),A", 1, 8, ld_c_plus_ff00_a},{"Illegal 0xE3", 1, 1, NULL},
        {"Illegal 0xE4", 1, 1, NULL},       {"PUSH HL", 1, 16, push_hl},        {"AND d8", 2, 8, and_d8},           {"RST 20H", 1, 16, rst_20h},
        {"ADD SP,r8", 2, 16, add_sp_r8},    {"JP (HL)", 1, 4, jp_hl},           {"LD (a16),A", 3, 16, ld_a16_a},    {"Illegal 0xEB", 1, 1, NULL},
        {"Illegal 0xEC", 1, 1, NULL},       {"Illegal 0xED", 1, 1, NULL},       {"XOR d8", 2, 8, xor_d8},           {"RST 28H", 1, 16, rst_28h},

/*0xF*/ {"LDH A,(a8)", 2, 12, ldh_a_a8_plus_ff00}, {"POP AF", 1, 12, pop_af},   {"LD A,(C)", 1, 8, ld_a_c_plus_ff00},{"DI", 1, 4, di},
        {"Illegal 0xF4", 1, 1, NULL},       {"PUSH AF", 1, 16, push_af},        {"OR d8", 2, 8, or_d8},             {"RST 30H", 1, 16, rst_30h},
        {"LD HL,SP+r8", 2, 12, ld_hl_sp_r8},{"LD SP,HL", 1, 8, ld_sp_hl},       {"LD A,(a16)", 3, 16, ld_a_a16},    {"EI", 1, 4, ei},
        {"Illegal 0xFC", 1, 1, NULL},       {"Illegal 0xFD", 1, 1, NULL},       {"CP d8", 2, 8, cp_d8},             {"RST 38H", 1, 16, rst_38h}
};

const Instruction cb_instructions[256] = {

/*0x0*/ {"RLC B", 2, 8, rlc_b},     {"RLC C", 2, 8, rlc_c},     {"RLC D", 2, 8, rlc_d},             {"RLC E", 2, 8, rlc_e},
        {"RLC H", 2, 8, rlc_h},     {"RLC L", 2, 8, rlc_l},     {"RLC (HL)", 2, 16, rlc_hl},        {"RLC A", 2, 8, rlc_a},
        {"RRC B", 2, 8, rrc_b},     {"RRC C", 2, 8, rrc_c},     {"RRC D", 2, 8, rrc_d},             {"RRC E", 2, 8, rrc_e},
        {"RRC H", 2, 8, rrc_h},     {"RRC L", 2, 8, rrc_l},     {"RRC (HL)", 2, 16, rrc_hl},        {"RRC A", 2, 8, rrc_a},

/*0x1*/ {"RL B", 2, 8, rl_b},       {"RL C", 2, 8, rl_c},       {"RL D", 2, 8, rl_d},               {"RL E", 2, 8, rl_e},
        {"RL H", 2, 8, rl_h},       {"RL L", 2, 8, rl_l},       {"RL (HL)", 2, 16, rl_hl},          {"RL A", 2, 8, rl_a},
        {"RR B", 2, 8, rr_b},       {"RR C", 2, 8, rr_c},       {"RR D", 2, 8, rr_d},               {"RR E", 2, 8, rr_e},
        {"RR H", 2, 8, rr_h},       {"RR L", 2, 8, rr_l},       {"RR (HL)", 2, 16, rr_hl},          {"RR A", 2, 8, rr_a},

/*0x2*/ {"SLA B", 2, 8, sla_b},     {"SLA C", 2, 8, sla_c},     {"SLA D", 2, 8, sla_d},             {"SLA E", 2, 8, sla_e},
        {"SLA H", 2, 8, sla_h},     {"SLA L", 2, 8, sla_l},     {"SLA (HL)", 2, 16, sla_hl},        {"SLA A", 2, 8, sla_a},
        {"SRA B", 2, 8, sra_b},     {"SRA C", 2, 8, sra_c},     {"SRA D", 2, 8, sra_d},             {"SRA E", 2, 8, sra_e},
        {"SRA H", 2, 8, sra_h},     {"SRA L", 2, 8, sra_l},     {"SRA (HL)", 2, 16, sra_hl},        {"SRA A", 2, 8, sra_a},

/*0x3*/ {"SWAP B", 2, 8, swap_b},   {"SWAP C", 2, 8, swap_c},   {"SWAP D", 2, 8, swap_d},           {"SWAP E", 2, 8, swap_e},
        {"SWAP H", 2, 8, swap_h},   {"SWAP L", 2, 8, swap_l},   {"SWAP (HL)", 2, 16, swap_hl},      {"SWAP A", 2, 8, swap_a},
        {"SRL B", 2, 8, srl_b},     {"SRL C", 2, 8, srl_c},     {"SRL D", 2, 8, srl_d},             {"SRL E", 2, 8, srl_e},
        {"SRL H", 2, 8, srl_h},     {"SRL L", 2, 8, srl_l},     {"SRL (HL)", 2, 16, srl_hl},        {"SRL A", 2, 8, srl_a},

/*0x4*/ {"BIT 0,B", 2, 8, bit_0_b}, {"BIT 0,C", 2, 8, bit_0_c}, {"BIT 0,D", 2, 8, bit_0_d},         {"BIT 0,E", 2, 8, bit_0_e},
        {"BIT 0,H", 2, 8, bit_0_h}, {"BIT 0,L", 2, 8, bit_0_l}, {"BIT 0,(HL)", 2, 12, bit_0_hl},    {"BIT 0,A", 2, 8, bit_0_a},
        {"BIT 1,B", 2, 8, bit_1_b}, {"BIT 1,C", 2, 8, bit_1_c}, {"BIT 1,D", 2, 8, bit_1_d},         {"BIT 1,E", 2, 8, bit_1_e},
        {"BIT 1,H", 2, 8, bit_1_h}, {"BIT 1,L", 2, 8, bit_1_l}, {"BIT 1,(HL)", 2, 12, bit_1_hl},    {"BIT 1,A", 2, 8, bit_1_a},

/*0x5*/ {"BIT 2,B", 2, 8, bit_2_b}, {"BIT 2,C", 2, 8, bit_2_c}, {"BIT 2,D", 2, 8, bit_2_d},         {"BIT 2,E", 2, 8, bit_2_e},
        {"BIT 2,H", 2, 8, bit_2_h}, {"BIT 2,L", 2, 8, bit_2_l}, {"BIT 2,(HL)", 2, 12, bit_2_hl},    {"BIT 2,A", 2, 8, bit_2_a},
        {"BIT 3,B", 2, 8, bit_3_b}, {"BIT 3,C", 2, 8, bit_3_c}, {"BIT 3,D", 2, 8, bit_3_d},         {"BIT 3,E", 2, 8, bit_3_e},
        {"BIT 3,H", 2, 8, bit_3_h}, {"BIT 3,L", 2, 8, bit_3_l}, {"BIT 3,(HL)", 2, 12, bit_3_hl},    {"BIT 3,A", 2, 8, bit_3_a},

/*0x6*/ {"BIT 4,B", 2, 8, bit_4_b}, {"BIT 4,C", 2, 8, bit_4_c}, {"BIT 4,D", 2, 8, bit_4_d},         {"BIT 4,E", 2, 8, bit_4_e},
        {"BIT 4,H", 2, 8, bit_4_h}, {"BIT 4,L", 2, 8, bit_4_l}, {"BIT 4,(HL)", 2, 12, bit_4_hl},    {"BIT 4,A", 2, 8, bit_4_a},
        {"BIT 5,B", 2, 8, bit_5_b}, {"BIT 5,C", 2, 8, bit_5_c}, {"BIT 5,D", 2, 8, bit_5_d},         {"BIT 5,E", 2, 8, bit_5_e},
        {"BIT 5,H", 2, 8, bit_5_h}, {"BIT 5,L", 2, 8, bit_5_l}, {"BIT 5,(HL)", 2, 12, bit_5_hl},    {"BIT 5,A", 2, 8, bit_5_a},

/*0x7*/ {"BIT 6,B", 2, 8, bit_6_b}, {"BIT 6,C", 2, 8, bit_6_c}, {"BIT 6,D", 2, 8, bit_6_d},         {"BIT 6,E", 2, 8, bit_6_e},
        {"BIT 6,H", 2, 8, bit_6_h}, {"BIT 6,L", 2, 8, bit_6_l}, {"BIT 6,(HL)", 2, 12, bit_6_hl},    {"BIT 6,A", 2, 8, bit_6_a},
        {"BIT 7,B", 2, 8, bit_7_b}, {"BIT 7,C", 2, 8, bit_7_c}, {"BIT 7,D", 2, 8, bit_7_d},         {"BIT 7,E", 2, 8, bit_7_e},
        {"BIT 7,H", 2, 8, bit_7_h}, {"BIT 7,L", 2, 8, bit_7_l}, {"BIT 7,(HL)", 2, 12, bit_7_hl},    {"BIT 7,A", 2, 8, bit_7_a},

/*0x8*/ {"RES 0,B", 2, 8, res_0_b}, {"RES 0,C", 2, 8, res_0_c}, {"RES 0,D", 2, 8, res_0_d},         {"RES 0,E", 2, 8, res_0_e},
        {"RES 0,H", 2, 8, res_0_h}, {"RES 0,L", 2, 8, res_0_l}, {"RES 0,(HL)", 2, 16, res_0_hl},    {"RES 0,A", 2, 8, res_0_a},
        {"RES 1,B", 2, 8, res_1_b}, {"RES 1,C", 2, 8, res_1_c}, {"RES 1,D", 2, 8, res_1_d},         {"RES 1,E", 2, 8, res_1_e},
        {"RES 1,H", 2, 8, res_1_h}, {"RES 1,L", 2, 8, res_1_l}, {"RES 1,(HL)", 2, 16, res_1_hl},    {"RES 1,A", 2, 8, res_1_a},

/*0x9*/ {"RES 2,B", 2, 8, res_2_b}, {"RES 2,C", 2, 8, res_2_c}, {"RES 2,D", 2, 8, res_2_d},         {"RES 2,E", 2, 8, res_2_e},
        {"RES 2,H", 2, 8, res_2_h}, {"RES 2,L", 2, 8, res_2_l}, {"RES 2,(HL)", 2, 16, res_2_hl},    {"RES 2,A", 2, 8, res_2_a},
        {"RES 3,B", 2, 8, res_3_b}, {"RES 3,C", 2, 8, res_3_c}, {"RES 3,D", 2, 8, res_3_d},         {"RES 3,E", 2, 8, res_3_e},
        {"RES 3,H", 2, 8, res_3_h}, {"RES 3,L", 2, 8, res_3_l}, {"RES 3,(HL)", 2, 16, res_3_hl},    {"RES 3,A", 2, 8, res_3_a},

/*0xA*/ {"RES 4,B", 2, 8, res_4_b}, {"RES 4,C", 2, 8, res_4_c}, {"RES 4,D", 2, 8, res_4_d},         {"RES 4,E", 2, 8, res_4_e},
        {"RES 4,H", 2, 8, res_4_h}, {"RES 4,L", 2, 8, res_4_l}, {"RES 4,(HL)", 2, 16, res_4_hl},    {"RES 4,A", 2, 8, res_4_a},
        {"RES 5,B", 2, 8, res_5_b}, {"RES 5,C", 2, 8, res_5_c}, {"RES 5,D", 2, 8, res_5_d},         {"RES 5,E", 2, 8, res_5_e},
        {"RES 5,H", 2, 8, res_5_h}, {"RES 5,L", 2, 8, res_5_l}, {"RES 5,(HL)", 2, 16, res_5_hl},    {"RES 5,A", 2, 8, res_5_a},

/*0xB*/ {"RES 6,B", 2, 8, res_6_b}, {"RES 6,C", 2, 8, res_6_c}, {"RES 6,D", 2, 8, res_6_d},         {"RES 6,E", 2, 8, res_6_e},
        {"RES 6,H", 2, 8, res_6_h}, {"RES 6,L", 2, 8, res_6_l}, {"RES 6,(HL)", 2, 16, res_6_hl},    {"RES 6,A", 2, 8, res_6_a},
        {"RES 7,B", 2, 8, res_7_b}, {"RES 7,C", 2, 8, res_7_c}, {"RES 7,D", 2, 8, res_7_d},         {"RES 7,E", 2, 8, res_7_e},
        {"RES 7,H", 2, 8, res_7_h}, {"RES 7,L", 2, 8, res_7_l}, {"RES 7,(HL)", 2, 16, res_7_hl},    {"RES 7,A", 2, 8, res_7_a},

/*0xC*/ {"SET 0,B", 2, 8, set_0_b}, {"SET 0,C", 2, 8, set_0_c}, {"SET 0,D", 2, 8, set_0_d},         {"SET 0,E", 2, 8, set_0_e},
        {"SET 0,H", 2, 8, set_0_h}, {"SET 0,L", 2, 8, set_0_l}, {"SET 0,(HL)", 2, 16, set_0_hl},    {"SET 0,A", 2, 8, set_0_a},
        {"SET 1,B", 2, 8, set_1_b}, {"SET 1,C", 2, 8, set_1_c}, {"SET 1,D", 2, 8, set_1_d},         {"SET 1,E", 2, 8, set_1_e},
        {"SET 1,H", 2, 8, set_1_h}, {"SET 1,L", 2, 8, set_1_l}, {"SET 1,(HL)", 2, 16, set_1_hl},    {"SET 1,A", 2, 8, set_1_a},

/*0xD*/ {"SET 2,B", 2, 8, set_2_b}, {"SET 2,C", 2, 8, set_2_c}, {"SET 2,D", 2, 8, set_2_d},         {"SET 2,E", 2, 8, set_2_e},
        {"SET 2,H", 2, 8, set_2_h}, {"SET 2,L", 2, 8, set_2_l}, {"SET 2,(HL)", 2, 16, set_2_hl},    {"SET 2,A", 2, 8, set_2_a},
        {"SET 3,B", 2, 8, set_3_b}, {"SET 3,C", 2, 8, set_3_c}, {"SET 3,D", 2, 8, set_3_d},         {"SET 3,E", 2, 8, set_3_e},
        {"SET 3,H", 2, 8, set_3_h}, {"SET 3,L", 2, 8, set_3_l}, {"SET 3,(HL)", 2, 16, set_3_hl},    {"SET 3,A", 2, 8, set_3_a},

/*0xE*/ {"SET 4,B", 2, 8, set_4_b}, {"SET 4,C", 2, 8, set_4_c}, {"SET 4,D", 2, 8, set_4_d},         {"SET 4,E", 2, 8, set_4_e},
        {"SET 4,H", 2, 8, set_4_h}, {"SET 4,L", 2, 8, set_4_l}, {"SET 4,(HL)", 2, 16, set_4_hl},    {"SET 4,A", 2, 8, set_4_a},
        {"SET 5,B", 2, 8, set_5_b}, {"SET 5,C", 2, 8, set_5_c}, {"SET 5,D", 2, 8, set_5_d},         {"SET 5,E", 2, 8, set_5_e},
        {"SET 5,H", 2, 8, set_5_h}, {"SET 5,L", 2, 8, set_5_l}, {"SET 5,(HL)", 2, 16, set_5_hl},    {"SET 5,A", 2, 8, set_5_a},

/*0xF*/ {"SET 6,B", 2, 8, set_6_b}, {"SET 6,C", 2, 8, set_6_c}, {"SET 6,D", 2, 8, set_6_d},         {"SET 6,E", 2, 8, set_6_e},
        {"SET 6,H", 2, 8, set_6_h}, {"SET 6,L", 2, 8, set_6_l}, {"SET 6,(HL)", 2, 16, set_6_hl},    {"SET 6,A", 2, 8, set_6_a},
        {"SET 7,B", 2, 8, set_7_b}, {"SET 7,C", 2, 8, set_7_c}, {"SET 7,D", 2, 8, set_7_d},         {"SET 7,E", 2, 8, set_7_e},
        {"SET 7,H", 2, 8, set_7_h}, {"SET 7,L", 2, 8, set_7_l}, {"SET 7,(HL)", 2, 16, set_7_hl},    {"SET 7,A", 2, 8, set_7_a},
};
