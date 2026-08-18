#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "chip8.h"


static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

int main(void){
    Chip8 chip8 = {{0}, {0}, 0, 0, 0};

    chip8_init(&chip8);

    chip8_load_rom(&chip8, "Splash.ch8");

    while(run){
        //sdl logic here...
        chip8_emulate_cycle(&chip8);
    }
}
