#include "chip8.h"
#include "stack.h"
#include <string.h>


void decimalToBinary(uint8_t num, uint8_t binary_arr[]);
void swap(uint8_t *a, uint8_t *b);


bool chip8_init(Chip8 *data){
    uint8_t font[80] = 
        { // 0x00 ^ 0xF0  = 0xF0
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

    // data->frameBuffer[0+64*0] = 0xff0000ff;

    return true;
}

bool chip8_load_rom(Chip8 *data, char *path){
    FILE *Fptr = fopen(path, "rb");
    int ch;
    data->pC = PROGRAM_START;
    uint16_t p_end = data->pC;

    if (Fptr == NULL){
        fprintf(stderr, "FILE not found");
        return false;
    }

    while((ch = fgetc(Fptr)) != EOF){
        data->mem[p_end++] = ch;
    }

    for(int i=0;i<8;i++){
        printf("%X ", data->mem[0x250+i]);
    }
    fclose(Fptr);
    return true;
}

void chip8_emulate_cycle(Chip8 *data){
    static int cycle = 1;
    printf("\n-- cycle %d PC:%X --\n", cycle++, data->pC);
    uint16_t opcode = (data->mem[data->pC] << 8) | (data->mem[data->pC + 1]);
    uint16_t nibble[4] = {0};

    //Mask out the individuals nibbles
    nibble[0] = opcode & 0xF000;
    nibble[1] = opcode & 0x0F00;
    nibble[2] = opcode & 0x00F0;
    nibble[3] = opcode & 0x000F;
    data->pC += 2;

    if (opcode == 0x00e0){
        memset(data->frameBuffer, 0, sizeof(data->frameBuffer) / sizeof(data->frameBuffer[0]));
    }
    switch (opcode & 0xF000){
        case 0x1000:
            // set pC to NNN 12 bit addresss
            data->pC = nibble[1] | nibble[2] | nibble[3];
            break;

        case 0x6000:
            //6XNN
            // Simply set the register VX to the value NN.
            data->registers[nibble[1] >> 8] = (uint8_t)nibble[2] | nibble[3];
            break;

        case 0x7000:
            // Add the value NN to VX.
            data->registers[nibble[1] >> 8] += (uint8_t)nibble[2] | nibble[3];
            break;

        case 0xa000:
            // This sets the index register I to the value NNN.
            data->index_register = nibble[1] | nibble[2] | nibble[3];
            printf("\n%x new index register: %x, %d\n", opcode, nibble[1] | nibble[2] | nibble[3], nibble[1] | nibble[2] | nibble[3]);
            break;

        case 0xd000:
            // DXYN
            // the drawing instruction
            // the index register holds an 8 bit sprite thats drawn horizantly
            // drawn by treating 0 bits as transparent, and all the 1 bits will “flip”

            //TODO: This instruction should fill the data->frameBuffer with sprite data
            uint8_t x = data->registers[nibble[1] >> 8];
            uint8_t y = data->registers[nibble[2] >> 4];
            uint8_t sprite_len = nibble[3];
            printf("\nsprite_len = 0x%x , %d\n", nibble[3], nibble[3]);
            x %= 64;
            y %= 32;
            data->registers[0xF] = 0;

            for (uint8_t row=0; row < sprite_len; row++){
                uint8_t byte_data = data->mem[data->index_register + row];
                printf("byte = %x\n", byte_data);
                y++;
                // printf("%x \n", byte_data);
                for (uint8_t col=0; col < 8; col++){
                    uint8_t pixel = (byte_data & (0x80>>col)) >> 7 - col;
                    printf("\ni = %d x:%d y:%d\n", x + col + y*64, x, y);
                    data->frameBuffer[x + col + y*64] ^= pixel * 0xffffffff;
                    if (data->frameBuffer[x + col + y*64] & pixel){
                        data->registers[0xF] = 1;
                    }

                }
            }

          break;

        default:
            break;
    // sleep()
}
    for (int i=0; i<16;i++){
        printf("reg[%x] = %d\n", i, data->registers[i]);
    }
    printf("index register = %x, %d", data->index_register, data->index_register);


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
    right = 8 - 1;
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
