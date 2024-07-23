#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

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

typedef struct
{
    uint8_t stack_pointer;              /* Stack pointer */
    uint8_t sound_timer;                /* Sound timer */
    uint8_t delay_timer;                /* Delay timer */
    uint16_t i_register;                /* Index register */
    uint16_t program_counter;           /* Program counter */
    uint8_t ram[RAM_SIZE];              /* RAM memory */
    uint8_t v[REGISTERS_SIZE];          /* V registers */
    uint16_t stack[STACK_SIZE];         /* Stack */
    uint16_t keys;                      /* Keyboard state */
    uint64_t display[DISPLAY_HEIGHT];   /* display */
} Chip8;

typedef struct
{
    uint16_t instruction;
    uint16_t nnn;
    uint8_t n;
    uint8_t x;
    uint8_t y;
    uint8_t kk;
} Opcode;

typedef struct {
    uint16_t opcode_prefix;                                 /* The prefix to match */
    uint16_t mask;                                          /* Mask to isolate the relevant bits */
    void (*handler)(Chip8 *chip8, Opcode *opcode);          /* Handler function */
} OpcodeEntry;

/* Declare the font set */
extern const uint8_t chip8_font_set[FONT_SET_SIZE];

/* Declare the opcode table */
extern const OpcodeEntry opcode_table[OPCODE_AMOUNT];

/* Function to initialize a Chip8 structure */
void chip8_init(Chip8 *chip8);

/* Function to load a program into Chip8's RAM */
void chip8_load_ram(Chip8 *chip8, const uint8_t *program, size_t program_size);

/* Function to get keyboard key state */
uint8_t chip8_get_keyboard_state(Chip8 *chip8, uint8_t key_index);

/* Function to set keyboard key state */
void chip8_set_keyboard_state(Chip8 *chip8, uint8_t key_index, uint8_t state);

/* Function to get display pixel state */
uint8_t chip8_get_display_state(Chip8 *chip8, uint8_t x_pos, uint8_t y_pos);

/* Function to set display pixel state */
void chip8_set_display_state(Chip8 *chip8, uint8_t x_pos, uint8_t y_pos, uint8_t state);

/* Function to fetch opcode */
Opcode chip8_fetch_opcode(Chip8 *chip8);

/* Function to fetch opcode */
int chip8_execute_opcode(Chip8 *chip8, Opcode *opcode);

/* Function to run Chip8 */
void chip8_run(Chip8 *chip8);

#endif /* CHIP8_H */
