#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #define sleep_ms(ms) Sleep(ms)  // Sleep function for Windows
#else
    #include <unistd.h>
    #define sleep_ms(ms) usleep((ms) * 1000)  // usleep takes microseconds on Unix-like systems
#endif

// Define constants for CHIP-8 emulator
#define RAM_SIZE 4096
#define REGISTERS_SIZE 16
#define STACK_SIZE 16
#define MEMORY_READ_START 512
#define PROGRAM_MEMORY_SIZE (RAM_SIZE - MEMORY_READ_START)
#define FONT_SET_START 0
#define FONT_SET_SIZE 80 // 16 characters * 5 bytes each
#define KEYBOARD_SIZE 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define OPCODE_AMOUNT 34
#define CPU_FREQUENCY 500  // 500 Hz for the main loop
#define TIMER_FREQUENCY 60  // 60 Hz for timer updates
#define TIME_PER_TICK_MS (1000 / CPU_FREQUENCY)  // Time per tick in milliseconds
#define TIME_PER_TIMER_TICK_MS (1000 / TIMER_FREQUENCY)  // Timer update interval in milliseconds

/**
 * Structure representing the state of the CHIP-8 emulator.
 */
typedef struct {
    uint8_t stack_pointer;              // Stack pointer
    uint8_t sound_timer;                // Sound timer
    uint8_t delay_timer;                // Delay timer
    uint16_t i_register;                // Index register
    uint16_t program_counter;           // Program counter
    uint8_t ram[RAM_SIZE];              // RAM memory
    uint8_t v[REGISTERS_SIZE];          // V registers
    uint16_t stack[STACK_SIZE];         // Stack
    uint16_t keys;                      // Keyboard state (bitfield)
    uint64_t display[DISPLAY_HEIGHT];   // Display
    clock_t timer;                      // Timer
    uint8_t display_changed;            // Flag for redrawing display only if needed
} Chip8;

/**
 * Structure representing an opcode.
 */
typedef struct {
    uint16_t instruction;  // Full opcode instruction
    uint16_t nnn;          // Address
    uint8_t n;             // 4-bit nibble
    uint8_t x;             // 4-bit x register
    uint8_t y;             // 4-bit y register
    uint8_t kk;            // 8-bit immediate value
} Opcode;

/**
 * Structure for opcode handling.
 */
typedef struct {
    uint16_t opcode_prefix;                       // The prefix to match
    uint16_t mask;                                // Mask to isolate relevant bits
    void (*handler)(Chip8 *chip8, Opcode *opcode);  // Handler function
} OpcodeEntry;

/* Font set used by CHIP-8 */
extern const uint8_t chip8_font_set[FONT_SET_SIZE];

/* Opcode table for CHIP-8 */
extern const OpcodeEntry opcode_table[OPCODE_AMOUNT];

/**
 * Initialize a CHIP-8 structure with default values.
 * 
 * @param chip8 Pointer to the Chip8 structure to initialize.
 */
void chip8_init(Chip8 *chip8);

/**
 * Load a program into CHIP-8's RAM.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @param program Pointer to the program data.
 * @param program_size Size of the program data.
 */
void chip8_load_ram(Chip8 *chip8, const uint8_t *program, size_t program_size);

/**
 * Check if the sound timer has expired and the buzzer should play.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @return 1 if the buzzer should play, 0 otherwise.
 */
uint8_t chip8_should_buzz(Chip8 *chip8);

/**
 * Decrement the delay and sound timers.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 */
void chip8_decrement_timers(Chip8 *chip8);

/**
 * Handle timer updates, including decrementing timers.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 */
void chip8_handle_timer_updates(Chip8 *chip8);

/**
 * Get the state of a keyboard key.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @param key_index Index of the key to check.
 * @return 1 if the key is pressed, 0 otherwise.
 */
uint8_t chip8_get_keyboard_state(Chip8 *chip8, uint8_t key_index);

/**
 * Set the state of a keyboard key.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @param key_index Index of the key to set.
 * @param state State to set (1 for pressed, 0 for not pressed).
 */
void chip8_set_keyboard_state(Chip8 *chip8, uint8_t key_index, uint8_t state);

/**
 * Get the state of a display pixel.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @param x_pos X position of the pixel.
 * @param y_pos Y position of the pixel.
 * @return 1 if the pixel is set, 0 otherwise.
 */
uint8_t chip8_get_display_state(Chip8 *chip8, uint8_t x_pos, uint8_t y_pos);

/**
 * Set the state of a display pixel.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @param x_pos X position of the pixel.
 * @param y_pos Y position of the pixel.
 * @param state State to set (1 for on, 0 for off).
 */
void chip8_set_display_state(Chip8 *chip8, uint8_t x_pos, uint8_t y_pos, uint8_t state);

/**
 * Fetch the current opcode from the CHIP-8 memory.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @return The fetched Opcode.
 */
Opcode chip8_fetch_opcode(Chip8 *chip8);

/**
 * Execute a given opcode.
 * 
 * @param chip8 Pointer to the Chip8 structure.
 * @param opcode Pointer to the Opcode to execute.
 * @return 0 if the opcode was handled, 1 otherwise.
 */
int chip8_execute_opcode(Chip8 *chip8, Opcode *opcode);

/**
 * Wait to maintain a constant CPU frequency.
 */
void chip8_wait_for_next_tick();

#endif /* CHIP8_H */
