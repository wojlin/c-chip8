#include "../include/chip8_opcodes.h"
#include "../include/chip8.h"
#include "../include/keyboard.h"

/* The above code is defining an array called `opcode_table` of type `OpcodeEntry`. Each `OpcodeEntry`
struct contains three elements: an opcode value, a mask value, and a function pointer to a specific
opcode handler function. */
const OpcodeEntry opcode_table[OPCODE_AMOUNT] = {
    { 0x00E0, 0xFFFF, chip8_execute_opcode_cls },                   // 00E0
    { 0x00EE, 0xFFFF, chip8_execute_opcode_ret },                   // 00EE
    { 0x1000, 0xF000, chip8_execute_opcode_jp },                    // 1nnn
    { 0x2000, 0xF000, chip8_execute_opcode_call },                  // 2nnn
    { 0x3000, 0xF000, chip8_execute_opcode_skip_equal_byte },       // 3xkk
    { 0x4000, 0xF000, chip8_execute_opcode_skip_not_equal_byte },   // 4xkk
    { 0x5000, 0xF00F, chip8_execute_opcode_skip_equal },            // 5xy0
    { 0x6000, 0xF000, chip8_execute_opcode_load_byte },             // 6xkk
    { 0x7000, 0xF000, chip8_execute_opcode_add_byte },              // 7xkk
    { 0x8000, 0xF00F, chip8_execute_opcode_load },                  // 8xy0
    { 0x8001, 0xF00F, chip8_execute_opcode_or },                    // 8xy1
    { 0x8002, 0xF00F, chip8_execute_opcode_and },                   // 8xy2
    { 0x8003, 0xF00F, chip8_execute_opcode_xor },                   // 8xy3
    { 0x8004, 0xF00F, chip8_execute_opcode_add },                   // 8xy4
    { 0x8005, 0xF00F, chip8_execute_opcode_subtract_x },            // 8xy5
    { 0x8006, 0xF00F, chip8_execute_opcode_divide },                // 8xy6
    { 0x8007, 0xF00F, chip8_execute_opcode_subtract_y },            // 8xy7
    { 0x800E, 0xF00F, chip8_execute_opcode_multiply },              // 8xyE
    { 0x9000, 0xF00F, chip8_execute_opcode_skip_not_equal },        // 9xy0
    { 0xA000, 0xF000, chip8_execute_opcode_set_i },                 // Annn
    { 0xB000, 0xF000, chip8_execute_opcode_jump },                  // Bnnn
    { 0xC000, 0xF000, chip8_execute_opcode_random },                // Cxkk
    { 0xD000, 0xF000, chip8_execute_opcode_draw },                  // Dxyn
    { 0xE09E, 0xF0FF, chip8_execute_opcode_skip_key },              // Ex9E
    { 0xE0A1, 0xF0FF, chip8_execute_opcode_skip_not_key },          // ExA1
    { 0xF007, 0xF0FF, chip8_execute_opcode_load_delay_timer },      // Fx07
    { 0xF00A, 0xF0FF, chip8_execute_opcode_wait_key },              // Fx0A
    { 0xF015, 0xF0FF, chip8_execute_opcode_set_delay_timer },       // Fx15
    { 0xF018, 0xF0FF, chip8_execute_opcode_set_sound_timer },       // Fx18
    { 0xF01E, 0xF0FF, chip8_execute_opcode_add_i },                 // Fx1E
    { 0xF029, 0xF0FF, chip8_execute_opcode_load_font},              // Fx29
    { 0xF033, 0xF0FF, chip8_execute_opcode_load_bcd },              // Fx33
    { 0xF055, 0xF0FF, chip8_execute_opcode_load_registers},         // Fx55
    { 0xF065, 0xF0FF, chip8_execute_opcode_load_memory },           // Fx65
};

/* clear the display */
void chip8_execute_opcode_cls(Chip8 *chip8, Opcode *opcode)
{
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            chip8_set_display_state(chip8, x, y, 0);
        }
    }
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* return from a subroutine */
void chip8_execute_opcode_ret(Chip8 *chip8, Opcode *opcode)
{
    chip8->stack_pointer = (chip8->stack_pointer - 1) & 0xF;
    chip8->program_counter = chip8->stack[chip8->stack_pointer];
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
    
}

/* Jump to location nnn */
void chip8_execute_opcode_jp(Chip8 *chip8, Opcode *opcode)
{
    chip8->program_counter = opcode->nnn;
}

/* Call subroutine at nnn */
void chip8_execute_opcode_call(Chip8 *chip8, Opcode *opcode)
{
    chip8->stack[chip8->stack_pointer] = chip8->program_counter;
    chip8->stack_pointer = (chip8->stack_pointer + 1) & 0xF;
    chip8->program_counter = opcode->nnn;
}

/* Skip next instruction if Vx = kk */
void chip8_execute_opcode_skip_equal_byte(Chip8 *chip8, Opcode *opcode)
{
    chip8->program_counter += (chip8->v[opcode->x] == opcode->kk) ? 4 : 2;
    chip8->program_counter = chip8->program_counter & 0x0FFF;
}

/* Skip next instruction if Vx != kk */
void chip8_execute_opcode_skip_not_equal_byte(Chip8 *chip8, Opcode *opcode)
{
    chip8->program_counter += (chip8->v[opcode->x] != opcode->kk) ? 4 : 2;
    chip8->program_counter = chip8->program_counter & 0x0FFF;
}

/* Skip next instruction if Vx = Vy */
void chip8_execute_opcode_skip_equal(Chip8 *chip8, Opcode *opcode)
{
    chip8->program_counter += (chip8->v[opcode->x] == chip8->v[opcode->y]) ? 4 : 2;
    chip8->program_counter = chip8->program_counter & 0x0FFF;
}

/* Set Vx = kk */
void chip8_execute_opcode_load_byte(Chip8 *chip8, Opcode *opcode)
{
    chip8->v[opcode->x] = opcode->kk;
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set Vx = Vx + kk */
void chip8_execute_opcode_add_byte(Chip8 *chip8, Opcode *opcode)
{
    chip8->v[opcode->x] = chip8->v[opcode->x] + opcode->kk;
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set Vx = Vy */
void chip8_execute_opcode_load(Chip8 *chip8, Opcode *opcode)
{
    chip8->v[opcode->x] = chip8->v[opcode->y];
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set Vx = Vx OR Vy. */
void chip8_execute_opcode_or(Chip8 *chip8, Opcode *opcode)
{
    chip8->v[opcode->x] = (chip8->v[opcode->x] | chip8->v[opcode->y]);
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set Vx = Vx AND Vy. */
void chip8_execute_opcode_and(Chip8 *chip8, Opcode *opcode)
{
    chip8->v[opcode->x] = (chip8->v[opcode->x] & chip8->v[opcode->y]);
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set Vx = Vx XOR Vy. */
void chip8_execute_opcode_xor(Chip8 *chip8, Opcode *opcode)
{
    chip8->v[opcode->x] = (chip8->v[opcode->x] ^ chip8->v[opcode->y]);
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set Vx = Vx + Vy, set VF = carry. */
void chip8_execute_opcode_add(Chip8 *chip8, Opcode *opcode)
{   
    chip8->v[0x0F] = ((uint16_t)chip8->v[opcode->x] + (uint16_t)chip8->v[opcode->y] > 255) ? 1 : 0;
    chip8->v[opcode->x] += chip8->v[opcode->y];
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set Vx = Vx - Vy, set VF = NOT borrow. */
void chip8_execute_opcode_subtract_x(Chip8 *chip8, Opcode *opcode)
{   
    chip8->v[0x0F] = (chip8->v[opcode->x] > chip8->v[opcode->y]) ? 1 : 0;
    chip8->v[opcode->x] -= chip8->v[opcode->y];
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set Vx = Vx SHR 1. */
void chip8_execute_opcode_divide(Chip8 *chip8, Opcode *opcode)
{   
    chip8->v[0x0F] = (chip8->v[opcode->x] & 0x01);
    chip8->v[opcode->x] >>= 1;
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set Vx = Vy - Vx, set VF = NOT borrow. */
void chip8_execute_opcode_subtract_y(Chip8 *chip8, Opcode *opcode)
{   
    chip8->v[0x0F] = (chip8->v[opcode->y] > chip8->v[opcode->x]) ? 1 : 0;
    chip8->v[opcode->x] = chip8->v[opcode->y] - chip8->v[opcode->x];
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set Vx = Vx SHR 1. */
void chip8_execute_opcode_multiply(Chip8 *chip8, Opcode *opcode)
{   
    chip8->v[0x0F] = (chip8->v[opcode->x] & 0x80) >> 7;
    uint8_t shl = chip8->v[opcode->x] << 1;
    chip8->v[opcode->x] = shl & 0xFF;
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Skip next instruction if Vx != Vy. */
void chip8_execute_opcode_skip_not_equal(Chip8 *chip8, Opcode *opcode)
{   
    chip8->program_counter += (chip8->v[opcode->x] != chip8->v[opcode->y]) ? 4 : 2;
    chip8->program_counter = chip8->program_counter & 0x0FFF;
}

/* Set I = nnn. */
void chip8_execute_opcode_set_i(Chip8 *chip8, Opcode *opcode)
{   
    chip8->i_register = opcode->nnn;
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Jump to location nnn + V0. */
void chip8_execute_opcode_jump(Chip8 *chip8, Opcode *opcode)
{   
    chip8->program_counter = opcode->nnn + chip8->v[0];
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set Vx = random byte AND kk. */
void chip8_execute_opcode_random(Chip8 *chip8, Opcode *opcode)
{   
    uint8_t random_value = rand() % 256;
    chip8->v[opcode->x] = random_value & opcode->kk;
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision. */
void chip8_execute_opcode_draw(Chip8 *chip8, Opcode *opcode)
{   
    uint8_t pos_x = chip8->v[opcode->x];
    uint8_t pos_y = chip8->v[opcode->y];
    uint8_t amount = opcode->n;

    chip8->display_changed = 1;

    uint8_t collision = 0;
    for(int n = 0; n < amount; n++)
    {
        uint8_t value = chip8->ram[chip8->i_register + n];
        for (int i = 7; i >= 0; i--) 
        {
            uint8_t px_pos_x = (pos_x + 7 - i) % 64;
            uint8_t px_pos_y = (pos_y + n) % 32;

            uint8_t is_on = (value >> i) & 1;
            uint8_t current_state = chip8_get_display_state(chip8, px_pos_x, px_pos_y);
            
            if (current_state & is_on) collision = 1;

            chip8_set_display_state(chip8, px_pos_x, px_pos_y, current_state ^ is_on);         
        }
    }

    chip8->v[0x0F] = collision;
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Skip next instruction if key with the value of Vx is pressed. */
void chip8_execute_opcode_skip_key(Chip8 *chip8, Opcode *opcode)
{   
    chip8->program_counter += (chip8_get_keyboard_state(chip8, chip8->v[opcode->x])) ? 4 : 2;
    chip8->program_counter = chip8->program_counter & 0x0FFF;
}

/* Skip next instruction if key with the value of Vx is not pressed. */
void chip8_execute_opcode_skip_not_key(Chip8 *chip8, Opcode *opcode)
{   
    chip8->program_counter += (chip8_get_keyboard_state(chip8, chip8->v[opcode->x])) ? 2 : 4;
    chip8->program_counter = chip8->program_counter & 0x0FFF;
}

/* Set Vx = delay timer value. */
void chip8_execute_opcode_load_delay_timer(Chip8 *chip8, Opcode *opcode)
{   
    chip8->v[opcode->x] = chip8->delay_timer;
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Wait for a key press, store the value of the key in Vx. */
void chip8_execute_opcode_wait_key(Chip8 *chip8, Opcode *opcode)
{   

    for (uint8_t i = 0; i < 16; i++) {
        uint8_t current_state = chip8_get_keyboard_state(chip8, i);
        if (current_state) 
        {
            chip8->v[opcode->x] = i;
            chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
            break;
            return;
        }
    }

}

/* Set delay timer = Vx. */
void chip8_execute_opcode_set_delay_timer(Chip8 *chip8, Opcode *opcode)
{   
    chip8->delay_timer = chip8->v[opcode->x];
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set sound timer = Vx. */
void chip8_execute_opcode_set_sound_timer(Chip8 *chip8, Opcode *opcode)
{   
    chip8->sound_timer = chip8->v[opcode->x];
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set I = I + Vx. */
void chip8_execute_opcode_add_i(Chip8 *chip8, Opcode *opcode)
{   
    chip8->i_register += chip8->v[opcode->x];
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Set I = location of sprite for digit Vx. */
void chip8_execute_opcode_load_font(Chip8 *chip8, Opcode *opcode)
{   
    chip8->i_register = chip8->v[opcode->x] * 5;
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Store BCD representation of Vx in memory locations I, I+1, and I+2. */
void chip8_execute_opcode_load_bcd(Chip8 *chip8, Opcode *opcode)
{   
    uint8_t Vx = chip8->v[opcode->x];
    uint8_t hundreds = Vx / 100;      // Hundreds place
    uint8_t tens = (Vx % 100) / 10;    // Tens place
    uint8_t ones = Vx % 10;          // Ones place

    // Store the digits in memory
    chip8->ram[chip8->i_register] = hundreds;
    chip8->ram[(chip8->i_register + 1)] = tens;
    chip8->ram[(chip8->i_register + 2)] = ones;

    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Store registers V0 through Vx in memory starting at location I. */
void chip8_execute_opcode_load_registers(Chip8 *chip8, Opcode *opcode)
{   
    for(int i = 0; i <= opcode->x; i++)
    {
        chip8->ram[chip8->i_register + i] = chip8->v[i];
    }
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}

/* Read registers V0 through Vx from memory starting at location I. */
void chip8_execute_opcode_load_memory(Chip8 *chip8, Opcode *opcode)
{   
    for(int i = 0; i <= opcode->x; i++)
    {
        chip8->v[i] = chip8->ram[chip8->i_register + i];
    }
    chip8->program_counter = (chip8->program_counter + 2) & 0x0FFF;
}