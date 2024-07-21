#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <string.h>

#define RAM_SIZE 4096
#define REGISTERS_SIZE 16
#define STACK_SIZE 16
#define MEMORY_READ_START 512
#define PROGRAM_MEMORY_SIZE (RAM_SIZE - MEMORY_READ_START)
#define FONT_SET_START 0x000
#define FONT_SET_SIZE 80 // 16 characters * 5 bytes each
#define KEYBOARD_SIZE 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

typedef struct
{
    uint8_t stack_pointer;              /* Stack pointer */
    uint8_t sound_timer;                /* Sound timer */
    uint8_t delay_timer;                /* Delay timer */
    uint16_t i_register;                /* Index register */
    uint16_t program_counter;           /* Program counter */
    uint8_t ram[RAM_SIZE];              /* RAM memory */
    uint8_t v[REGISTERS_SIZE];          /* V registers */
    uint16_t stack[STACK_SIZE];         /* Stack */
    uint16_t keys;                      /* Keyboard state */
    uint64_t display[DISPLAY_HEIGHT];   /* display */
} Chip8;

/* Declare the font set */
extern const uint8_t font_set[FONT_SET_SIZE];

/* Function to initialize a Chip8 structure */
void chip8_init(Chip8 *chip8);

/* Function to load a program into Chip8's RAM */
void chip8_load_ram(Chip8 *chip8, const uint8_t *program);

/* Function to get keyboard key state */
uint8_t chip8_get_keyboard_state(Chip8 *chip8, uint8_t key_index);

/* Function to set keyboard key state */
void chip8_set_keyboard_state(Chip8 *chip8, uint8_t key_index, uint8_t state);

/* Function to get display pixel state */
uint8_t chip8_get_display_state(Chip8 *chip8, uint8_t x_pos, uint8_t y_pos);

/* Function to set display pixel state */
void chip8_set_display_state(Chip8 *chip8, uint8_t x_pos, uint8_t y_pos, uint8_t state);

#endif /* CHIP8_H */
