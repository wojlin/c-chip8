#ifndef CHIP8_OPCODES_H
#define CHIP8_OPCODES_H

#include "chip8.h"

/* clear display */
void chip8_execute_opcode_cls(Chip8 *chip8, Opcode *opcode);

/* return from a subroutine */
void chip8_execute_opcode_ret(Chip8 *chip8, Opcode *opcode);

/* Jump to location nnn */
void chip8_execute_opcode_jp(Chip8 *chip8, Opcode *opcode);

/* Call subroutine at nnn */
void chip8_execute_opcode_call(Chip8 *chip8, Opcode *opcode);

/* Skip next instruction if Vx = kk */
void chip8_execute_opcode_skip_equal_byte(Chip8 *chip8, Opcode *opcode);

/* Skip next instruction if Vx != kk */
void chip8_execute_opcode_skip_not_equal_byte(Chip8 *chip8, Opcode *opcode);

/* Skip next instruction if Vx = Vy */
void chip8_execute_opcode_skip_equal(Chip8 *chip8, Opcode *opcode);

/* Set Vx = kk */
void chip8_execute_opcode_load_byte(Chip8 *chip8, Opcode *opcode);

/* Set Vx = Vx + kk */
void chip8_execute_opcode_add_byte(Chip8 *chip8, Opcode *opcode);

/* Set Vx = Vy */
void chip8_execute_opcode_load(Chip8 *chip8, Opcode *opcode);

/* Set Vx = Vx OR Vy. */
void chip8_execute_opcode_or(Chip8 *chip8, Opcode *opcode);

/* Set Vx = Vx OR Vy. */
void chip8_execute_opcode_and(Chip8 *chip8, Opcode *opcode);

/* Set Vx = Vx OR Vy. */
void chip8_execute_opcode_xor(Chip8 *chip8, Opcode *opcode);

/* Set Vx = Vx + Vy, set VF = carry. */
void chip8_execute_opcode_add(Chip8 *chip8, Opcode *opcode);

/* Set Vx = Vx - Vy, set VF = NOT borrow. */
void chip8_execute_opcode_subtract_x(Chip8 *chip8, Opcode *opcode);

/* Set Vx = Vx SHR 1. */
void chip8_execute_opcode_divide(Chip8 *chip8, Opcode *opcode);

/* Set Vx = Vy - Vx, set VF = NOT borrow. */
void chip8_execute_opcode_subtract_y(Chip8 *chip8, Opcode *opcode);

/* Set Vx = Vx SHR 1. */
void chip8_execute_opcode_multiply(Chip8 *chip8, Opcode *opcode);

/* Skip next instruction if Vx != Vy. */
void chip8_execute_opcode_skip_not_equal(Chip8 *chip8, Opcode *opcode);

/* Set I = nnn. */
void chip8_execute_opcode_set_i(Chip8 *chip8, Opcode *opcode);

/* Jump to location nnn + V0. */
void chip8_execute_opcode_jump(Chip8 *chip8, Opcode *opcode);

/* Set Vx = random byte AND kk. */
void chip8_execute_opcode_random(Chip8 *chip8, Opcode *opcode);

/* Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision. */
void chip8_execute_opcode_draw(Chip8 *chip8, Opcode *opcode);

/* Skip next instruction if key with the value of Vx is pressed. */
void chip8_execute_opcode_skip_key(Chip8 *chip8, Opcode *opcode);

/* Skip next instruction if key with the value of Vx is pressed. */
void chip8_execute_opcode_skip_not_key(Chip8 *chip8, Opcode *opcode);

/* Set Vx = delay timer value. */
void chip8_execute_opcode_load_delay_timer(Chip8 *chip8, Opcode *opcode);

/* Wait for a key press, store the value of the key in Vx. */
void chip8_execute_opcode_wait_key(Chip8 *chip8, Opcode *opcode);

/* Set delay timer = Vx. */
void chip8_execute_opcode_set_delay_timer(Chip8 *chip8, Opcode *opcode);

/* Set sound timer = Vx. */
void chip8_execute_opcode_set_sound_timer(Chip8 *chip8, Opcode *opcode);

/* Set I = I + Vx. */
void chip8_execute_opcode_add_i(Chip8 *chip8, Opcode *opcode);

/* Set I = location of sprite for digit Vx. */
void chip8_execute_opcode_load_font(Chip8 *chip8, Opcode *opcode);

/* Store BCD representation of Vx in memory locations I, I+1, and I+2. */
void chip8_execute_opcode_load_bcd(Chip8 *chip8, Opcode *opcode);

/* Store registers V0 through Vx in memory starting at location I. */
void chip8_execute_opcode_load_registers(Chip8 *chip8, Opcode *opcode);

/* Read registers V0 through Vx from memory starting at location I. */
void chip8_execute_opcode_load_memory(Chip8 *chip8, Opcode *opcode);

#endif /* CHIP8_OPCODES_H */