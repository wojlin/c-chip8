#include "../include/chip8.h"
#include "../include/chip8_opcodes.h"

/* Define the font set used by Chip8 */
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



/* Initialize a Chip8 structure to default values */
void chip8_init(Chip8 *chip8) {

    srand((unsigned int)time(NULL));

    chip8->stack_pointer = 0;
    chip8->sound_timer = 0;
    chip8->delay_timer = 0;
    chip8->i_register = 0;
    chip8->program_counter = MEMORY_READ_START;

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

/* Load a program into Chip8's RAM */
void chip8_load_ram(Chip8 *chip8, const uint8_t program[PROGRAM_MEMORY_SIZE], size_t program_size) {
    memcpy(chip8->ram + MEMORY_READ_START, program, program_size); 
}

/* decrement timers */
void chip8_decrement_timers(Chip8 *chip8) {
    if (chip8->delay_timer > 0) {
        --chip8->delay_timer;
    }
    if (chip8->sound_timer > 0) {
        --chip8->sound_timer;
    }
}

/* return 1 if the buzzer should play 0 if not */
uint8_t chip8_should_buzz(Chip8 *chip8)
{
    return (chip8->sound_timer > 0) ? 1 : 0;
}

/* handle sound and delay timer */
void chip8_handle_timer_updates(Chip8* chip8) {
    clock_t current_time = clock();
    double elapsed_time = (double)(current_time - chip8->timer) / CLOCKS_PER_SEC * 1000;
    if (elapsed_time >= TIME_PER_TIMER_TICK_MS) {  // 60Hz timer update
        chip8_decrement_timers(chip8);
        chip8->timer = current_time;
    }
}

/* Function to get keyboard key state */
uint8_t chip8_get_keyboard_state(Chip8 *chip8, uint8_t key_index) {
    if (key_index >= KEYBOARD_SIZE) {
        // Invalid key index, return 0
        return 0;
    }
    return (chip8->keys & (1 << key_index)) ? 1 : 0;
}

/* Function to set keyboard key state */
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

/* Function to get display pixel state */
uint8_t chip8_get_display_state(Chip8 *chip8, uint8_t pos_x, uint8_t pos_y) {
    if (pos_x >= DISPLAY_WIDTH || pos_y >= DISPLAY_HEIGHT) {
        // Invalid pixel index, return 0
        return 0;
    }
    return (chip8->display[pos_y] & (1ULL << pos_x)) ? 1 : 0;
}

/* Function to set display pixel state */
void chip8_set_display_state(Chip8 *chip8, uint8_t pos_x, uint8_t pos_y, uint8_t state) {
    if (pos_x >= DISPLAY_WIDTH || pos_y >= DISPLAY_HEIGHT) {
        // Invalid pixel index, return
        return;
    }

    if (state) {
        chip8->display[pos_y] |= (1ULL << pos_x);  // Set the bit
    } else {
        chip8->display[pos_y] &= ~(1ULL << pos_x); // Clear the bit
    }
}

/* Function to fetch opcode */
Opcode chip8_fetch_opcode(Chip8 *chip8)
{
    Opcode opcode;

    uint16_t pc = chip8->program_counter;

    /* fetch instruction */
    opcode.instruction = chip8->ram[pc]  << 8| chip8->ram[pc+1];

    /* decode instruction */
    opcode.nnn = opcode.instruction & 0x0FFF;        /* Lowest 12 bits */
    opcode.n = opcode.instruction & 0x000F;           /* Lowest 4 bits */
    opcode.x = (opcode.instruction & 0x0F00) >> 8;    /* 4 bits from high byte */
    opcode.y = (opcode.instruction & 0x00F0) >> 4;    /* 4 bits from low byte */
    opcode.kk = opcode.instruction & 0x00FF;          /* Lowest 8 bits */

    return opcode;
}

/* Function to fetch opcode */
int chip8_execute_opcode(Chip8 *chip8, Opcode *opcode)
{
    int handled = 1;
    for (int i = 0; i < sizeof(opcode_table) / sizeof(OpcodeEntry); ++i) {
        if ((opcode->instruction & opcode_table[i].mask) == opcode_table[i].opcode_prefix) 
        {
            opcode_table[i].handler(chip8, opcode);
            handled = 0;
            break;
        }
    }

    chip8_handle_timer_updates(chip8);

    return handled;
}

/* Function to run Chip8 */
void chip8_run(Chip8 *chip8)
{   
    while(1)
    {
        Opcode opcode = chip8_fetch_opcode(chip8);
        int result = chip8_execute_opcode(chip8, &opcode);
        if(result){
            break;
        }        
    }   
}