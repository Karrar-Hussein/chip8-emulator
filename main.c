#include <stdio.h>
#include "stack.h"
#include <stdint.h>

#define MEMSIZE 4096
#define PROGRAM_START 0x200

typedef uint8_t byte;
int main(void){
    FILE *fptr;
    byte mem[MEMSIZE];
    uint8_t registers[16];
    Stack *stack = create_stack(16);
    uint16_t num;
    push(stack, 5);
    pop(stack, &num);
    printf("popped item: %d", num);
    // TODO: Load font array into memory.
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
    uint8_t *pC = &mem[PROGRAM_START];
    uint8_t *pI = &mem[PROGRAM_START];
    uint16_t opcode;
    int ch;

    // load the program into memory starting at address 200 (512 in decimal). 
    fptr = fopen("IBM_Logo.ch8", "rb");
    while((ch = fgetc(fptr)) != EOF){
        // printf("%.2x ", ch);
        *pI++ = ch;
    }
    fclose(fptr);

    while(pC != pI){
        // TODO: Set the pC to point at 16 bit instructions.
        // TODO: Divide opcode
        opcode = (*pC << 8) | *(pC + 1);
        pC += 2;
        switch (opcode & 0xF000){
            case 0x0000:
                // clear the display.
                break;
            case 0x1000:
                // set pC to NNN 12 bit addresss
                break;
            case 0x6000:
                // Simply set the register VX to the value NN.
                break;
            case 0x7000:
                // Add the value NN to VX.
                break;
            case 0xa000:
                // This sets the index register I to the value NNN.
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
