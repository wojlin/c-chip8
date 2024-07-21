#include "../include/chip8.h"

/* Define the font set used by Chip8 */
const uint8_t font_set[FONT_SET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

/* Initialize a Chip8 structure to default values */
void chip8_init(Chip8 *chip8) {
    chip8->stack_pointer = 0;
    chip8->sound_timer = 0;
    chip8->delay_timer = 0;
    chip8->i_register = 0;
    chip8->program_counter = MEMORY_READ_START;

    /* Initialize RAM memory to zero */
    memset(chip8->ram, 0, RAM_SIZE);

    /* Initialize V registers to zero */
    memset(chip8->v, 0, REGISTERS_SIZE);

    /* Initialize stack to zero */
    memset(chip8->stack, 0, STACK_SIZE * sizeof(uint16_t));

    /* Initialize display pixels to zero */
    memset(chip8->display, 0, sizeof(chip8->display));

    /* Load the font set into memory */
    memcpy(chip8->ram + FONT_SET_START, font_set, FONT_SET_SIZE);

    /* Initialize keyboard state to zero */
    chip8->keys = 0;
}

/* Load a program into Chip8's RAM */
void chip8_load_ram(Chip8 *chip8, const uint8_t program[PROGRAM_MEMORY_SIZE]) {
    memcpy(chip8->ram + MEMORY_READ_START, program, PROGRAM_MEMORY_SIZE);
}

/* Function to get keyboard key state */
uint8_t chip8_get_keyboard_state(Chip8 *chip8, uint8_t key_index) {
    if (key_index >= KEYBOARD_SIZE) {
        // Invalid key index, return 0
        return 0;
    }
    return (chip8->keys & (1 << key_index)) ? 1 : 0;
}

/* Function to set keyboard key state */
void chip8_set_keyboard_state(Chip8 *chip8, uint8_t key_index, uint8_t state) {
    if (key_index >= KEYBOARD_SIZE) {
        // Invalid key index
        return;
    }

    if (state) {
        chip8->keys |= (1 << key_index);  // Set the bit
    } else {
        chip8->keys &= ~(1 << key_index); // Clear the bit
    }
}

/* Function to get display pixel state */
uint8_t chip8_get_display_state(Chip8 *chip8, uint8_t pos_x, uint8_t pos_y) {
    if (pos_x >= DISPLAY_WIDTH || pos_y >= DISPLAY_HEIGHT) {
        // Invalid pixel index, return 0
        return 0;
    }
    return (chip8->display[pos_y] & (1ULL << pos_x)) ? 1 : 0;
}

/* Function to set display pixel state */
void chip8_set_display_state(Chip8 *chip8, uint8_t pos_x, uint8_t pos_y, uint8_t state) {
    if (pos_x >= DISPLAY_WIDTH || pos_y >= DISPLAY_HEIGHT) {
        // Invalid pixel index, return
        return;
    }

    if (state) {
        chip8->display[pos_y] |= (1ULL << pos_x);  // Set the bit
    } else {
        chip8->display[pos_y] &= ~(1ULL << pos_x); // Clear the bit
    }
}