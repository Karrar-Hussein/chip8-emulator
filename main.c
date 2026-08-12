#include <stdio.h>
#include "stack.h"
#include <stdint.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define MEMSIZE 4096
#define PROGRAM_START 0x200
#define WINDOW_WIDTH 64 * 10
#define WINDOW_HEIGHT 32 * 10
#define FRAMERATE 1000 / 30
#define SPRITE_LEN 8


void decimalToBinary(uint8_t num, uint8_t binary_arr[]);
void swap(uint8_t *a, uint8_t *b);

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

int main(void){
    FILE *fptr;
    uint8_t mem[MEMSIZE];
    uint8_t registers[16];
    uint8_t *index_register;
    // Stack *stack = create_stack(16);
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
    uint16_t nibble[4] = {0};
    int ch;

    // load the program into memory starting at address 200 (512 in decimal). 
    fptr = fopen("Splash.ch8", "rb");
    while((ch = fgetc(fptr)) != EOF){
        *pI++ = ch;
    }
    fclose(fptr);

    //TODO: Implement the display and the DXYN instrucion

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("IBM Logo", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, NULL);

   while(pC != pI){

        opcode = (*pC << 8) | *(pC + 1);
        // printf("%x ", opcode);

        //Mask out the individuals nibbles
        nibble[0] = opcode & 0xF000;
                    // FFFF & 0F000 = 
        nibble[1] = opcode & 0x0F00;
        nibble[2] = opcode & 0x00F0;
        nibble[3] = opcode & 0x000F;
        pC += 2;

        switch (opcode & 0xF000){
            case 0x0000:
               // clear the display.
                // SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
                // SDL_RenderClear(renderer);
                break;
            case 0x1000:
                // set pC to NNN 12 bit addresss
                //  0x0F00 | 0x00F0 | 0x000F = 0FFF
                //
                pC = &mem[nibble[1] | nibble[2] | nibble[3]];
                break;
            case 0x6000:
                //6XNN
                //6101 = reg[1] = 00000001 = 1
                //6010 = reg[0] = 
                // Simply set the register VX to the value NN.
                nibble[1] >>= 8;
                registers[nibble[1]] = (uint8_t)nibble[2] | nibble[3];
                // printf("opcode: %x\nreg[%d] = %d , %d\n", opcode, nibble[1], registers[nibble[1]], (uint8_t)nibble[2] | nibble[3]);
                break;
            case 0x7000:
                // Add the value NN to VX.
                registers[nibble[1]] += (uint8_t)nibble[2] | nibble[3];
                break;
            case 0xa000:
                // This sets the index register I to the value NNN.
                index_register = &mem[nibble[1] | nibble[2] | nibble[3]];
                break;
            case 0xd000:
                // DXYN 
                // the drawing instruction
                // the index register holds an 8 bit sprite thats drawn horizantly
                // drawn by treating 0 bits as transparent, and all the 1 bits will “flip”

                uint8_t frameBuffer[SPRITE_LEN] = {0};
                SDL_FRect rects[8];
                nibble[1] >>= 8;
                nibble[2] >>= 4;
                float x = registers[nibble[1]];
                float y = registers[nibble[2]];
                // fprintf(stderr, "(%f, %f) ", x, y);

                decimalToBinary(*index_register, frameBuffer);

                for (int i=0; i<SPRITE_LEN; i++){
                    rects[i].x = x++;
                    rects[i].y = y;
                    rects[i].w = 10.0;
                    rects[i].h = 10.0;
                    // printf("%d", frameBuffer[i]);
                }


                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);


                SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
                SDL_RenderRects(renderer, rects, 8);

                SDL_RenderPresent(renderer);
                break;
            default:
                break;
        }
        // for (int i=0; i<16;i++){
        //     printf("reg[%d] = %d\n", i, registers[i]);
        // }
        SDL_Delay(1000 / 700);
    }

    // SDL_Delay(2000);
    //
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void decimalToBinary(uint8_t num, uint8_t binary_arr[]){
    int i = 0;
    uint8_t remainder = 0;
    uint8_t divisor = 2;
    int left, right;
    printf("num: %3.d  ", num);

    while (num > 0){
        remainder = num % divisor;
        num = num / divisor;
        binary_arr[i] = remainder;
        i++;
    }
    left = 0;
    right = SPRITE_LEN - 1;
    while(left < right){
        swap(&binary_arr[left], &binary_arr[right]);
        left++;
        right--;
    }
}

void swap(uint8_t *a, uint8_t *b){
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}
