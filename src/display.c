#include "../include/display.h"

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)  // Sleep function on Windows
void sound_buzzer() {
    Beep(1000, 100);  // Frequency 1000 Hz, Duration 100 ms
}
#else
#define sleep_ms(ms) usleep((ms) * 1000)  // usleep takes microseconds on Unix-like systems
void sound_buzzer() {
    printf("\a");  // Fallback to ASCII Bell character
    fflush(stdout); 
}
#endif

// Clears the terminal screen based on the operating system.
void clear_terminal() {
    #ifdef _WIN32
        system("cls"); // Windows
    #else
        int r = system("clear"); // Unix-like systems
    #endif
}

// Renders the display to the terminal if it has changed.
void show_terminal_display(Chip8 *chip8) {
    if (chip8->display_changed) {
        clear_terminal();

        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            for (int x = 0; x < DISPLAY_WIDTH; x++) {
                // Display '#' for pixels that are on and '.' for pixels that are off.
                printf("%c", (chip8_get_display_state(chip8, x, y) ? '#' : '.'));
            }
            printf("\n");
        }

        chip8->display_changed = 0;
    }
}

// Renders the display using SDL if it has changed.
void show_sdl_display(Display *display, Chip8 *chip8) {
    if (chip8->display_changed) {
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            for (int x = 0; x < DISPLAY_WIDTH; x++) {
                // Set draw color based on pixel state.
                SDL_SetRenderDrawColor(display->renderer,
                                       chip8_get_display_state(chip8, x, y) ? ONE_R : ZERO_R,
                                       chip8_get_display_state(chip8, x, y) ? ONE_G : ZERO_G,
                                       chip8_get_display_state(chip8, x, y) ? ONE_B : ZERO_B,
                                       SDL_ALPHA_OPAQUE);

                // Render each pixel.
                SDL_Rect rect = { x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE };
                SDL_RenderFillRect(display->renderer, &rect);
            }
        }

        SDL_RenderPresent(display->renderer); // Update the display
        chip8->display_changed = 0;
    }
}

// Initializes SDL library.
void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

// Creates and initializes an SDL display.
Display createDisplay() {
    Display display;

    // Create a window
    display.window = SDL_CreateWindow("CHIP8 EMULATOR",
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      DISPLAY_WIDTH * PIXEL_SIZE,
                                      DISPLAY_HEIGHT * PIXEL_SIZE,
                                      SDL_WINDOW_SHOWN);
    if (display.window == NULL) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Create a renderer
    display.renderer = SDL_CreateRenderer(display.window, -1, SDL_RENDERER_ACCELERATED);
    if (display.renderer == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(display.window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    return display;
}

// Cleans up SDL resources.
void removeDisplay(Display *display) {
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);
    SDL_Quit();
}
