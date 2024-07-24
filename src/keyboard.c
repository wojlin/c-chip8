#include "keyboard.h"

// Function definitions

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

    void read_keyboard(Chip8 *chip8) {
        for (int i = 0; i < 16; i++) {
            chip8_set_keyboard_state(chip8, i, 0);  // Assume all keys are up
        }
        if (_kbhit()) {
            char key = _getch();
            uint8_t key_index = map_key_to_index(key);
            if (key_index < 16) {
                chip8_set_keyboard_state(chip8, key_index, 1);  // Key down
            }
        }
    }

#else

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

    uint8_t wait_for_keypress(Chip8 *chip8) {
        while (1) {
            if (kbhit()) {
                char key = getchar();
                uint8_t key_index = map_key_to_index(key);
                if (key_index < 16) {
                    chip8_set_keyboard_state(chip8, key_index, 1); // Key down
                    return key_index;
                }
            }
        }
    }

    void read_keyboard(Chip8 *chip8) {
        for (int i = 0; i < 16; i++) {
            chip8_set_keyboard_state(chip8, i, 0);  // Assume all keys are up
        }
        if (kbhit()) {
            char key = getchar();  
            uint8_t key_index = map_key_to_index(key);
            if (key_index < 16) {
                chip8_set_keyboard_state(chip8, key_index, 1);  // Key down
            }
        }
    }
#endif
