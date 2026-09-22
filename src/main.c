#include "gb.h"
#include <stdio.h>
#include <SDL2/SDL.h>

#define FRAMERATE 59.722
#define WINDOW_SIZE_MULTIPLIER 6


// Maps a keyboard key to its Game Boy button. Returns 0 if the key isn't bound.
static int map_key(SDL_Keycode sym, gb_key *key) {

    switch (sym) {
        case SDLK_UP:    *key = GB_KEY_UP;     return 1;
        case SDLK_DOWN:  *key = GB_KEY_DOWN;   return 1;
        case SDLK_LEFT:  *key = GB_KEY_LEFT;   return 1;
        case SDLK_RIGHT: *key = GB_KEY_RIGHT;  return 1;
        case SDLK_a:     *key = GB_KEY_A;      return 1;
        case SDLK_d:     *key = GB_KEY_B;      return 1;
        case SDLK_q:     *key = GB_KEY_START;  return 1;
        case SDLK_e:     *key = GB_KEY_SELECT; return 1;
        default:         return 0;
    }

}

int main(int argc, char *argv[]) {

    gb_console gb = {0};

    if (argc != 2) {
        fprintf(stderr, "Use ./beni-boy ROM_Name\n");
        return 1;
    }

    if (init_gb(&gb, argv[1])) {
        return 1;
    }


    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error initializing SDL\n");
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Beni Boy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GB_SCREEN_WIDTH*WINDOW_SIZE_MULTIPLIER, GB_SCREEN_HEIGHT*WINDOW_SIZE_MULTIPLIER, 0);
    if (!window) {
        fprintf(stderr, "Error creating a window\n");
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating a renderer\n");
        return 1;
    }
    SDL_Texture *framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, GB_SCREEN_WIDTH, GB_SCREEN_HEIGHT);
    if (!framebuffer) {
        fprintf(stderr, "Error creating the framebuffer texture\n");
        return 1;
    }
    int frame_start_ms;
    SDL_Event e;
    gb_key key;

    int running = 1;

    while (running) {

        frame_start_ms = SDL_GetTicks();

        run_frame(&gb);


        SDL_UpdateTexture(framebuffer , NULL, gb.ppu.lcd, GB_SCREEN_WIDTH * sizeof (uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, framebuffer , NULL, NULL);
        SDL_RenderPresent(renderer);

        //Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit
            if (e.type == SDL_QUIT) {
                running = 0;
            }

            //User presses a key
            else if (e.type == SDL_KEYDOWN) {
                if (map_key(e.key.keysym.sym, &key)) {
                    press_key(&gb, key);
                }

            } else if (e.type == SDL_KEYUP) {
                if (map_key(e.key.keysym.sym, &key)) {
                    release_key(&gb, key);
                }
            }
        }


        frame_start_ms = SDL_GetTicks() - frame_start_ms;
        if (frame_start_ms < 1000.0/FRAMERATE) {
            SDL_Delay((1000.0/FRAMERATE) - frame_start_ms); //delay the frame to be in time
        }


    }

    SDL_DestroyTexture(framebuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    free_gb(&gb);

    SDL_Quit();

    return 0;
}
