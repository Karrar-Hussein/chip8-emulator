#include "stack.h"
#include <stdint.h>


void decimalToBinary(uint8_t num, uint8_t binary_arr[]);

bool chip8_init(Chip8 *data){
    data->mem = {0};
    data->data->registers = {0};
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
                0xF0, 0x80, 0xF0, 0x80, 0x80
        };

    //load font into memory
    int counter = 0x50 ;
    for(int i=0; i<80;i++){
        data->mem[counter++] = font[i];
    }

    return true;
}

bool chip8_load_rom(Chip8 *data, char *path){
    FILE *fptr;
    int ch = 7;
    int p_end = data->pC;

    if (!fptr = fopen(path, "rb")){
        return false;
    }

    while((ch = fgetc(fptr)) != EOF){
        data->mem[data->p_end++] = ch;
    }

    fclose(fptr);
    return true
}

void chip8_emulate_cycle(Chip8 *data){
    opcode = (data->mem[data->pC] << 8) | (data->mem[data->pC + 1]));
    uint8_t nibble[4];

    //Mask out the individuals nibbles
    nibble[0] = opcode & 0xF000;
    nibble[1] = opcode & 0x0F00;
    nibble[2] = opcode & 0x00F0;
    nibble[3] = opcode & 0x000F;
    data->pC += 2;

    switch (opcode & 0xF000){
        case 0x0000:
            // clear the frameBuffer.
            break;

        case 0x1000:
            // set pC to NNN 12 bit addresss
            data->pC = nibble[1] | nibble[2] | nibble[3];
            break;

        case 0x6000:
            //6XNN
            // Simply set the register VX to the value NN.
            nibble[1] >>= 8;
            data->registers[nibble[1]] = (uint8_t)nibble[2] | nibble[3];
            break;

        case 0x7000:
            // Add the value NN to VX.
            data->registers[nibble[1]] += (uint8_t)nibble[2] | nibble[3];
            break;

        case 0xa000:
            // This sets the index register I to the value NNN.
            data->index_register = nibble[1] | nibble[2] | nibble[3];
            break;

        case 0xd000:
            // DXYN
            // the drawing instruction
            // the index register holds an 8 bit sprite thats drawn horizantly
            // drawn by treating 0 bits as transparent, and all the 1 bits will “flip”

            //TODO: This instruction should fill the frameBuffer with sprite data

          break;

        default:
            break;
}

}

void decimalToBinary(uint8_t num, uint8_t binary_arr[]){
    int i = 0;
    uint8_t remainder = 0;
    uint8_t divisor = 2;
    int left, right;

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
