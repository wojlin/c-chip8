#include "../include/chip8.h"

#include <stdio.h>


void print_display(Chip8 *chip8)
{
    printf("/////////////////////////// display ////////////////////////////\n");
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            printf("%c", (chip8_get_display_state(chip8, x, y) ? '#' : '.'));
        }
        printf("\n");
    }
}

void print_keyboard(Chip8 *chip8)
{
    printf("/////////////////////////// keyboard ////////////////////////////\n");
    for (int i = 0; i < KEYBOARD_SIZE; i++) {    
        printf("0x%X : %c\n", i, (chip8_get_keyboard_state(chip8, i) ? '1' : '0'));  
    }
}

int main()
{
    Chip8 chip8;

    const uint8_t program[4] = {0x60, 0x05, 0x61, 0x08};

    chip8_init(&chip8);
    chip8_load_ram(&chip8, program);


    chip8_set_keyboard_state(&chip8, 0, 1);
    chip8_set_keyboard_state(&chip8, 3, 1);

    chip8_set_display_state(&chip8, 0, 0, 1);
    chip8_set_display_state(&chip8, 32, 16, 1);

    print_keyboard(&chip8);
    print_display(&chip8);

    return 0;
}