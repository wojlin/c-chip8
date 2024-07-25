#include "../include/utils.h"
#include <stdio.h>

/**
 * Print the launch options for the CHIP-8 emulator.
 * 
 * @param args Pointer to the Arguments structure containing launch options.
 */
void print_launch_options(const Arguments *args) {
    printf("/////////////////////// Launch Options ///////////////////////\n");
    printf("User Interface: %s\n", args->ui);
    printf("Data Type: %s\n", args->type);
    printf("Data Path/Bytes: %s\n", args->data);
}

/**
 * Print the current timers for the CHIP-8 emulator.
 * 
 * @param chip8 Pointer to the Chip8 structure containing timer values.
 */
void print_timers(Chip8 *chip8) {
    printf("/////////////////////////// Timers ///////////////////////////\n");
    printf("Sound Timer: %u   ", chip8->sound_timer);  
    printf("Delay Timer: %u\n", chip8->delay_timer);  
}

/**
 * Print the current state of the keyboard for the CHIP-8 emulator.
 * 
 * @param chip8 Pointer to the Chip8 structure containing keyboard state.
 */
void print_keyboard(Chip8 *chip8) {
    printf("/////////////////////////// Keyboard //////////////////////////\n");
    for (int i = 0; i < KEYBOARD_SIZE; i++) {
        printf("0x%X : %c\n", i, (chip8_get_keyboard_state(chip8, i) ? '1' : '0'));  
    }
}

/**
 * Print details of the current opcode for the CHIP-8 emulator.
 * 
 * @param opcode Pointer to the Opcode structure containing opcode details.
 */
void print_opcode(const Opcode *opcode) {
    printf("//////////////////////////// Opcode /////////////////////////////\n");
    printf("Instruction: 0x%04X\n", opcode->instruction);
    printf("nnn: 0x%03X\n", opcode->nnn);
    printf("n: 0x%01X\n", opcode->n);
    printf("x: 0x%01X\n", opcode->x);
    printf("y: 0x%01X\n", opcode->y);
    printf("kk: 0x%02X\n", opcode->kk);
}
