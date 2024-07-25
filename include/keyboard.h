#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "chip8.h"

#include <SDL2/SDL.h>
#include <stdint.h>

// Function declarations
int read_keyboard(Chip8 *chip8);
int read_keyboard_sdl(Chip8 *chip8);

#endif // KEYBOARD_H
