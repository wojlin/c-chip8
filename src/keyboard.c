#include "keyboard.h"

// Function to map SDL keys to CHIP-8 key indices
static uint8_t sdl_key_to_chip8_key(SDL_Keycode key) {
    switch (key) {
        case SDLK_1: return 0x1;
        case SDLK_2: return 0x2;
        case SDLK_3: return 0x3;
        case SDLK_4: return 0xC;
        case SDLK_q: return 0x4;
        case SDLK_w: return 0x5;
        case SDLK_e: return 0x6;
        case SDLK_r: return 0xD;
        case SDLK_a: return 0x7;
        case SDLK_s: return 0x8;
        case SDLK_d: return 0x9;
        case SDLK_f: return 0xE;
        case SDLK_z: return 0xA;
        case SDLK_x: return 0x0;
        case SDLK_c: return 0xB;
        case SDLK_v: return 0xF;
        default: return 0xFF; // Invalid key
    }
}

int read_keyboard_sdl(Chip8 *chip8) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    // Check ESC key for exiting
    uint8_t esc_state = state[SDL_GetScancodeFromKey(SDLK_ESCAPE)] ? 1 : 0;
    if (esc_state) {
        return 1; // Return 1 to indicate that ESC was pressed
    }

    // Iterate over all CHIP-8 keys
    for (int i = 0; i < KEYBOARD_SIZE; ++i) {
        // Map CHIP-8 key index to SDL keycode
        SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN;
        switch (i) {
            case 0x1: scancode = SDL_SCANCODE_1; break;
            case 0x2: scancode = SDL_SCANCODE_2; break;
            case 0x3: scancode = SDL_SCANCODE_3; break;
            case 0xC: scancode = SDL_SCANCODE_4; break;
            case 0x4: scancode = SDL_SCANCODE_Q; break;
            case 0x5: scancode = SDL_SCANCODE_W; break;
            case 0x6: scancode = SDL_SCANCODE_E; break;
            case 0xD: scancode = SDL_SCANCODE_R; break;
            case 0x7: scancode = SDL_SCANCODE_A; break;
            case 0x8: scancode = SDL_SCANCODE_S; break;
            case 0x9: scancode = SDL_SCANCODE_D; break;
            case 0xE: scancode = SDL_SCANCODE_F; break;
            case 0xA: scancode = SDL_SCANCODE_Z; break;
            case 0x0: scancode = SDL_SCANCODE_X; break;
            case 0xB: scancode = SDL_SCANCODE_C; break;
            case 0xF: scancode = SDL_SCANCODE_V; break;
            default: continue; // Skip invalid key indices
        }

        // Update the state of the key
        uint8_t new_state = state[scancode] ? 1 : 0;
        chip8_set_keyboard_state(chip8, i, new_state);
    }

    return 0; // Return 0 to indicate normal operation
}



uint8_t map_key_to_index(char key) {
    switch (key) {
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 12;
        case 'q': return 4;
        case 'w': return 5;
        case 'e': return 6;
        case 'r': return 13;
        case 'a': return 7;
        case 's': return 8;
        case 'd': return 9;
        case 'f': return 14;
        case 'z': return 10;
        case 'x': return 0;
        case 'c': return 11;
        case 'v': return 15;
        default: return 16; // Invalid key index
    }
}

#ifdef _WIN32
    #include <conio.h>
    uint8_t wait_for_keypress(Chip8 *chip8) {
        while (1) {
            if (_kbhit()) {
                char key = _getch();
                uint8_t key_index = map_key_to_index(key);
                if (key_index < 16) {
                    chip8_set_keyboard_state(chip8, key_index, 1); // Key down
                    return key_index;
                }
            }
        }
    }

    int read_keyboard(Chip8 *chip8) {
        for (int i = 0; i < 16; i++) {
            chip8_set_keyboard_state(chip8, i, 0);  // Assume all keys are up
        }
        if (_kbhit()) {
            char key = _getch();
            if (key == 27) { // ESC key
                return 1;
            }
            uint8_t key_index = map_key_to_index(key);
            if (key_index < 16) {
                chip8_set_keyboard_state(chip8, key_index, 1);  // Key down
            }
        }
        return 0;
    }

#else
    #include <termios.h>
#include <unistd.h>
#include <fcntl.h>

    int kbhit(void) {
        struct termios oldt, newt;
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if (ch != EOF) {
            ungetc(ch, stdin);
            return 1;
        }

        return 0;
    }

    int read_keyboard(Chip8 *chip8) {
        for (int i = 0; i < 16; i++) {
            chip8_set_keyboard_state(chip8, i, 0);  // Assume all keys are up
        }
        if (kbhit()) {
            char key = getchar();  
            if (key == 27) { // ESC key
            return 1;
            }
            uint8_t key_index = map_key_to_index(key);
            if (key_index < 16) {
                chip8_set_keyboard_state(chip8, key_index, 1);  // Key down
            }
        }
        return 0;
    }
#endif
