#ifndef CPU_H
#define CPU_H

#include <stdint.h>


typedef struct {

    union {
        struct {
            union {
                uint8_t F;
                struct {
                    uint8_t not_used : 4;
                    uint8_t c_flag : 1;
                    uint8_t h_flag : 1;
                    uint8_t n_flag : 1;
                    uint8_t z_flag : 1;
                };
            };
            uint8_t A;
        };
        uint16_t AF;
    };

    union {
        struct {
            uint8_t C;
            uint8_t B;
        };
        uint16_t BC;
    };

    union {
        struct {
            uint8_t E;
            uint8_t D;
        };
        uint16_t DE;
    };

    union {
        struct {
            uint8_t L;
            uint8_t H;
        };
        uint16_t HL;
    };

    uint16_t sp;
    uint16_t pc;

    uint8_t remaining_cycles;

} Cpu;


void init_cpu(Cpu *cpu);

#endif /* CPU_H */
