#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include "chip8.h"

// Defines the size of each pixel in the SDL window.
#define PIXEL_SIZE 20

// RGB values for colors used in the SDL display.
#define ZERO_R 23
#define ZERO_G 21
#define ZERO_B 59
#define ONE_R 200
#define ONE_G 172
#define ONE_B 214

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} Display;

/**
 * @brief Initializes the SDL library.
 */
void initSDL();

/**
 * @brief Creates and initializes an SDL display.
 * 
 * @return The initialized Display struct.
 */
Display createDisplay();

/**
 * @brief Cleans up SDL resources.
 * 
 * @param display The Display struct to clean up.
 */
void removeDisplay(Display *display);

/**
 * @brief Renders the display to the terminal if it has changed.
 * 
 * @param chip8 Pointer to the Chip8 emulator instance.
 */
void show_terminal_display(Chip8 *chip8);

/**
 * @brief Renders the display using SDL if it has changed.
 * 
 * @param display Pointer to the Display struct.
 * @param chip8 Pointer to the Chip8 emulator instance.
 */
void show_sdl_display(Display *display, Chip8 *chip8);

/**
 * @brief Clears the terminal screen based on the operating system.
 */
void clear_terminal();

/**
 * @brief beeps the buzzer
 */
void sound_buzzer();

#endif // DISPLAY_H
