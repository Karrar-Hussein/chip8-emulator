#ifndef CHIP8_H
#define CHIP8_H

#include <stdbool.h>

#define MEMSIZE 4096
#define PROGRAM_START 0x200


typedef struct {
    uint8_t mem[MEMSIZE];
    uint8_t registers[16];
    uint8_t index_register;
    uint8_t pC;
    uint8_t frameBuffer[64*32];

} Chip8;

bool chip8_init(Chip8 data);
bool chip8_load_rom(Chip8 data, char *path);
void chip8_emulate_cycle(Chip8 data);


#endif
