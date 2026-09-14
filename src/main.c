#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "chip8.h"

#define FPS_INTERVAL 1000 / 30

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

void quit();


int main(void){
    Chip8 chip8 = {{0}, {0}, 0, 0, 0};
    SDL_Texture *pTexture;
    char run = 1;

    if(!SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "SDL was failed to initialize");
        return EXIT_FAILURE;
    }

    if(!SDL_CreateWindowAndRenderer("Splash", 64*10, 32*10, 0, &window, &renderer)){
        fprintf(stderr, "SDL Renderer failed to initialize");
        return EXIT_FAILURE;
    }

    chip8_init(&chip8);

    //TODO: grab Rom Path from command line arguments
    chip8_load_rom(&chip8, "roms/IBM_Logo.ch8");

    pTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    SDL_SetTextureScaleMode(pTexture, SDL_SCALEMODE_NEAREST);

    while(run){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                quit();
                run = 0;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN){
                if (event.key.key == SDLK_UP){
                    chip8_emulate_cycle(&chip8);
                }

            }
        }


        // printf("buffer: %x ", chip8.frameBuffer[48]);
        // ABGR8888
        SDL_UpdateTexture(pTexture, NULL, chip8.frameBuffer, 64*4);
        // for (int i=0;i<64*32;i++){
            // printf("%d ", chip8.frameBuffer[i]);
        // }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);


        SDL_RenderTexture(renderer, pTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(FPS_INTERVAL);
    }

    return EXIT_SUCCESS;
}

void quit(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}


