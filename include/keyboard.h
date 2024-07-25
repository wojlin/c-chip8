#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "chip8.h"
#include <SDL2/SDL.h>
#include <stdint.h>

/**
 * Reads the keyboard state and updates the CHIP-8 keyboard state.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @return 1 if the ESC key was pressed, 0 otherwise.
 */
int read_keyboard(Chip8 *chip8);

/**
 * Reads the keyboard state using SDL and updates the CHIP-8 keyboard state.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @return 1 if the ESC key was pressed, 0 otherwise.
 */
int read_keyboard_sdl(Chip8 *chip8);

/**
 * Waits for a key press and returns the corresponding CHIP-8 key index.
 * 
 * @return The CHIP-8 key index or 0xFF if an invalid key was pressed.
 */
uint8_t wait_for_key_sdl();

/**
 * Waits for a key press and returns the corresponding CHIP-8 key index.
 * 
 * @return The CHIP-8 key index or 0xFF if an invalid key was pressed.
 */
uint8_t wait_for_key();

#endif // KEYBOARD_H
