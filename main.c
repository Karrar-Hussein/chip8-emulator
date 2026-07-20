#include <stdio.h>
#include "stack.h"
#include <stdint.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define MEMSIZE 4096
#define PROGRAM_START 0x200
#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 32


// static SDL_Window *window = NULL;
// static SDL_Renderer *renderer = NULL;

int main(void){
    FILE *fptr;
    uint8_t mem[MEMSIZE];
    uint8_t registers[16];
    uint8_t *index_register;
    Stack *stack = create_stack(16);
    uint8_t font[80] = 
        {
                0xF0, 0x90, 0x90, 0x90, 0xF0,
                0x20, 0x60, 0x20, 0x20, 0x70,
                0xF0, 0x10, 0xF0, 0x80, 0xF0,
                0xF0, 0x10, 0xF0, 0x10, 0xF0,
                0x90, 0x90, 0xF0, 0x10, 0x10,
                0xF0, 0x80, 0xF0, 0x10, 0xF0,
                0xF0, 0x80, 0xF0, 0x90, 0xF0,
                0xF0, 0x10, 0x20, 0x40, 0x40,
                0xF0, 0x90, 0xF0, 0x90, 0xF0,
                0xF0, 0x90, 0xF0, 0x10, 0xF0,
                0xF0, 0x90, 0xF0, 0x90, 0x90,
                0xE0, 0x90, 0xE0, 0x90, 0xE0,
                0xF0, 0x80, 0x80, 0x80, 0xF0,
                0xE0, 0x90, 0x90, 0x90, 0xE0,
                0xF0, 0x80, 0xF0, 0x80, 0xF0,
                0xF0, 0x80, 0xF0, 0x80, 0x80};
    int counter = 0x50;
    for(int i=0; i<80;i++){
        mem[counter++] = font[i];
    }

    uint8_t *pC = &mem[PROGRAM_START];
    uint8_t *pI = &mem[PROGRAM_START];
    uint16_t opcode;
    uint16_t nibble[4];
    int ch;

    // load the program into memory starting at address 200 (512 in decimal). 
    fptr = fopen("IBM_Logo.ch8", "rb");
    while((ch = fgetc(fptr)) != EOF){
        // printf("%.2x ", ch);
        *pI++ = ch;
    }
    fclose(fptr);

    //TODO: Implement the display and the DXYN instrucion
    // // init window
    // SDL_Init(SDL_INIT_VIDEO);
    // window = SDL_CreateWindow("IBM Logo", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    // renderer = SDL_CreateRenderer(window, NULL);
    //
    // //draw and update frame
    // SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    // SDL_RenderClear(renderer);
    // SDL_RenderPresent(renderer);
    // //clean up
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
    //
    while(pC != pI){

        opcode = (*pC << 8) | *(pC + 1);

        //Mask out the individuals nibbles
        nibble[0] = opcode & 0xF000;
        nibble[1] = opcode & 0x0F00;
        nibble[2] = opcode & 0x00F0;
        nibble[3] = opcode & 0x000F;
        pC += 2;

        switch (opcode & 0xF000){
            case 0x0000:
                // clear the display.
                break;
            case 0x1000:
                // set pC to NNN 12 bit addresss
                pC = &mem[nibble[1] | nibble[2] | nibble[3]];
                break;
            case 0x6000:
                // Simply set the register VX to the value NN.
                registers[nibble[1]] = nibble[2] | nibble[4];
                break;
            case 0x7000:
                // Add the value NN to VX.
                registers[nibble[1]] += nibble[2] | nibble[4];
                break;
            case 0xa000:
                // This sets the index register I to the value NNN.
                index_register = &mem[nibble[1] | nibble[2] | nibble[3]];
                break;
            case 0xd000:
                // the drawing instruction
                break;
            default:
                break;
        }
    }


    return 0;
}
