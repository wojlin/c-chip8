#ifndef UTILS_H
#define UTILS_H

#include "chip8.h"
#include "params.h"

/**
 * Print the launch options for the CHIP-8 emulator.
 * 
 * @param args Pointer to the Arguments structure containing launch options.
 */
void print_launch_options(const Arguments *args);

/**
 * Print the current timers for the CHIP-8 emulator.
 * 
 * @param chip8 Pointer to the Chip8 structure containing timer values.
 */
void print_timers(Chip8 *chip8);

/**
 * Print the current state of the keyboard for the CHIP-8 emulator.
 * 
 * @param chip8 Pointer to the Chip8 structure containing keyboard state.
 */
void print_keyboard(Chip8 *chip8);

/**
 * Print details of the current opcode for the CHIP-8 emulator.
 * 
 * @param opcode Pointer to the Opcode structure containing opcode details.
 */
void print_opcode(const Opcode *opcode);

#endif // UTILS_H
