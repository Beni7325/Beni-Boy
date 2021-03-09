#ifndef PIXEL_FIFOS_H
#define PIXEL_FIFOS_H

#include <stdint.h>


#define FIFO_SIZE 16 // Placeholder


typedef enum {
    BACKGROUND, WINDOW
} Fetcher_Mode;


typedef enum {
    BG, S0, S1
} Palette;


typedef struct {
    uint8_t color;
    Palette palette;
    uint8_t background_priority;
} Pixel_Info;


typedef struct {

    Fetcher_Mode mode;

    Pixel_Info fifo[FIFO_SIZE + 1];
    uint8_t head;
    uint8_t tail;
    uint8_t size;

} Background_Fifo;

#endif /* PIXEL_FIFOS_H */
