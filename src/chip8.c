#include "../include/chip8.h"
#include "../include/chip8_opcodes.h"

/* Define the font set used by CHIP-8 */
const uint8_t chip8_font_set[FONT_SET_SIZE] = {
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

/**
 * Initialize a CHIP-8 structure with default values.
 * 
 * @param chip8 Pointer to the Chip8 structure to initialize.
 */
void chip8_init(Chip8 *chip8) {
    srand((unsigned int)time(NULL));

    chip8->stack_pointer = 0;
    chip8->sound_timer = 0;
    chip8->delay_timer = 0;
    chip8->i_register = 0;
    chip8->program_counter = MEMORY_READ_START;
    chip8->timer = 0;

    /* Initialize RAM memory to zero */
    memset(chip8->ram, 0, RAM_SIZE);

    /* Initialize V registers to zero */
    memset(chip8->v, 0, REGISTERS_SIZE);

    /* Initialize stack to zero */
    memset(chip8->stack, 0, STACK_SIZE * sizeof(uint16_t));

    /* Initialize display pixels to zero */
    memset(chip8->display, 0, sizeof(chip8->display));

    /* Load the font set into memory */
    memcpy(chip8->ram + FONT_SET_START, chip8_font_set, FONT_SET_SIZE);

    /* Initialize keyboard state to zero */
    chip8->keys = 0;
}

/**
 * Load a program into CHIP-8's RAM.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @param program Pointer to the program data.
 * @param program_size Size of the program data.
 */
void chip8_load_ram(Chip8 *chip8, const uint8_t program[PROGRAM_MEMORY_SIZE], size_t program_size) {
    memcpy(chip8->ram + MEMORY_READ_START, program, program_size); 
    memcpy(chip8->ram + FONT_SET_START, chip8_font_set, FONT_SET_SIZE);
}

/**
 * Decrement the delay and sound timers.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 */
void chip8_decrement_timers(Chip8 *chip8) {
    if (chip8->delay_timer > 0) {
        chip8->delay_timer -= 1;
    }
    if (chip8->sound_timer > 0) {
        chip8->sound_timer -= 1;
    }
}

/**
 * Check if the sound timer has expired and the buzzer should play.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @return 1 if the buzzer should play, 0 otherwise.
 */
uint8_t chip8_should_buzz(Chip8 *chip8) {
    return (chip8->sound_timer > 0) ? 1 : 0;
}

/**
 * Handle timer updates, including decrementing timers.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 */
void chip8_handle_timer_updates(Chip8 *chip8) {
    chip8->timer += 1;
    if (chip8->timer > TIME_PER_TIMER_TICK_MS) {
        chip8_decrement_timers(chip8);
        chip8->timer = 0;
    }
}

/**
 * Get the state of a keyboard key.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @param key_index Index of the key to check.
 * @return 1 if the key is pressed, 0 otherwise.
 */
uint8_t chip8_get_keyboard_state(Chip8 *chip8, uint8_t key_index) {
    if (key_index >= KEYBOARD_SIZE) {
        // Invalid key index, return 0
        return 0;
    }
    return (chip8->keys & (1 << key_index)) ? 1 : 0;
}

/**
 * Set the state of a keyboard key.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @param key_index Index of the key to set.
 * @param state State to set (1 for pressed, 0 for not pressed).
 */
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

/**
 * Get the state of a display pixel.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @param x_pos X position of the pixel.
 * @param y_pos Y position of the pixel.
 * @return 1 if the pixel is set, 0 otherwise.
 */
uint8_t chip8_get_display_state(Chip8 *chip8, uint8_t x_pos, uint8_t y_pos) {
    if (x_pos >= DISPLAY_WIDTH || y_pos >= DISPLAY_HEIGHT) {
        // Invalid pixel index, return 0
        return 0;
    }
    uint64_t mask = (uint64_t)1 << (DISPLAY_WIDTH - 1 - x_pos);
    return (chip8->display[y_pos] & mask) ? 1 : 0;
}

/**
 * Set the state of a display pixel.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @param x_pos X position of the pixel.
 * @param y_pos Y position of the pixel.
 * @param state State to set (1 for on, 0 for off).
 */
void chip8_set_display_state(Chip8 *chip8, uint8_t x_pos, uint8_t y_pos, uint8_t state) {
    if (x_pos >= DISPLAY_WIDTH || y_pos >= DISPLAY_HEIGHT) {
        // Invalid pixel index, return
        return;
    }

    uint64_t mask = (uint64_t)1 << (DISPLAY_WIDTH - 1 - x_pos);

    if (state) {
        chip8->display[y_pos] |= mask;  // Set the bit
    } else {
        chip8->display[y_pos] &= ~mask; // Clear the bit
    }
}

/**
 * Fetch the current opcode from CHIP-8 memory.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @return The fetched Opcode.
 */
Opcode chip8_fetch_opcode(Chip8 *chip8) {
    Opcode opcode;

    uint16_t pc = chip8->program_counter;

    /* Fetch instruction */
    opcode.instruction = (chip8->ram[pc] << 8) | chip8->ram[pc + 1];

    /* Decode instruction */
    opcode.nnn = opcode.instruction & 0x0FFF;        // Lowest 12 bits
    opcode.n = opcode.instruction & 0x000F;           // Lowest 4 bits
    opcode.x = (opcode.instruction & 0x0F00) >> 8;    // 4 bits from high byte
    opcode.y = (opcode.instruction & 0x00F0) >> 4;    // 4 bits from low byte
    opcode.kk = opcode.instruction & 0x00FF;          // Lowest 8 bits

    return opcode;
}

/**
 * Execute a given opcode.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @param opcode Pointer to the Opcode to execute.
 * @return 0 if the opcode was handled, 1 otherwise.
 */
int chip8_execute_opcode(Chip8 *chip8, Opcode *opcode) {
    chip8_wait_for_next_tick();
    chip8_handle_timer_updates(chip8);

    int handled = 1;
    for (int i = 0; i < OPCODE_AMOUNT; ++i) {
        if ((opcode->instruction & opcode_table[i].mask) == opcode_table[i].opcode_prefix) {
            opcode_table[i].handler(chip8, opcode);
            handled = 0;
            break;
        }
    }
    return handled;
}

/**
 * Wait to maintain a constant CPU frequency.
 */
void chip8_wait_for_next_tick() {
    sleep_ms(TIME_PER_TICK_MS);
}
