#include <stdio.h>
#include <SDL2/SDL.h>
#include "./../includes/gb.h"


#define FRAMERATE 59.722
#define INSTR_PER_FRAME 70224
#define WINDOW_SIZE_MULTIPLIER 1


int main(int argc, char *argv[]) {

    GB gb;


    if (argc < 2) {
        printf("Invalid input\n");
        return -1;
    }

    init_gb(&gb, argv[1]);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error initializing SDL\n");
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Beni Boy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH*WINDOW_SIZE_MULTIPLIER, HEIGHT*WINDOW_SIZE_MULTIPLIER, 0);
    if (!window) {
        fprintf(stderr, "Error creating a window\n");
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    int timerFps;
    SDL_Event e;

    int frames = 1;
    while (frames) {

        timerFps = SDL_GetTicks();

        for (size_t c=0; c<INSTR_PER_FRAME; ++c) {
            tick_cpu(&gb);
            tick_timer(&gb);
            tick_ppu(&gb);
        }

        SDL_UpdateTexture(framebuffer , NULL, gb.ppu.screen, WIDTH * sizeof (uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, framebuffer , NULL, NULL);
        SDL_RenderPresent(renderer);

        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if ( e.type == SDL_QUIT ) {
                frames = 0;
            }
        }

        timerFps = SDL_GetTicks() - timerFps;
        if(timerFps < 1000.0/FRAMERATE) {
            SDL_Delay((1000.0/FRAMERATE) - timerFps); //delay the frame to be in time
        }

    }

    SDL_DestroyTexture(framebuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    free_gb(&gb);

    return 0;
}
