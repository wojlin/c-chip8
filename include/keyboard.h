#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "chip8.h"

#include <stdint.h>  // For uint8_t

// Function declarations
uint8_t map_key_to_index(char key);

#ifdef _WIN32
#include <conio.h>

// Declaration of platform-specific functions for Windows
uint8_t wait_for_keypress(Chip8 *chip8);
void read_keyboard(Chip8 *chip8);

#else

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Declaration of platform-specific functions for Unix-like systems
int kbhit(void);
uint8_t wait_for_keypress(Chip8 *chip8);
void read_keyboard(Chip8 *chip8);

#endif // _WIN32

#endif // KEYBOARD_H
