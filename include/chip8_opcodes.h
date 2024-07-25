#ifndef CHIP8_OPCODES_H
#define CHIP8_OPCODES_H

#include "chip8.h"

/**
 * @brief Clear the display.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_cls(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Return from a subroutine.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_ret(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Jump to location nnn.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_jp(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Call subroutine at nnn.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_call(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Skip the next instruction if Vx equals kk.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_skip_equal_byte(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Skip the next instruction if Vx does not equal kk.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_skip_not_equal_byte(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Skip the next instruction if Vx equals Vy.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_skip_equal(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Set Vx to kk.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_load_byte(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Add kk to Vx.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_add_byte(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Set Vx to Vy.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_load(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Set Vx to Vx OR Vy.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_or(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Set Vx to Vx AND Vy.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_and(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Set Vx to Vx XOR Vy.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_xor(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Add Vy to Vx, set VF to carry.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_add(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Subtract Vy from Vx, set VF to NOT borrow.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_subtract_x(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Shift Vx right by 1 bit.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_divide(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Subtract Vx from Vy, set VF to NOT borrow.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_subtract_y(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Multiply Vx and Vy (interpret as a right shift operation).
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_multiply(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Skip the next instruction if Vx does not equal Vy.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_skip_not_equal(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Set I to nnn.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_set_i(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Jump to location nnn + V0.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_jump(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Set Vx to a random byte AND kk.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_random(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Display an n-byte sprite starting at memory location I at (Vx, Vy), set VF to collision.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_draw(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Skip the next instruction if the key with the value of Vx is pressed.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_skip_key(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Skip the next instruction if the key with the value of Vx is not pressed.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_skip_not_key(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Set Vx to the delay timer value.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_load_delay_timer(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Wait for a key press, store the value of the key in Vx.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_wait_key(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Set the delay timer to Vx.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_set_delay_timer(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Set the sound timer to Vx.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_set_sound_timer(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Add Vx to I.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_add_i(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Set I to the location of the sprite for digit Vx.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_load_font(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Store the BCD representation of Vx in memory locations I, I+1, and I+2.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_load_bcd(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Store registers V0 through Vx in memory starting at location I.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_load_registers(Chip8 *chip8, Opcode *opcode);

/**
 * @brief Read registers V0 through Vx from memory starting at location I.
 *
 * @param chip8 Pointer to the Chip8 state structure.
 * @param opcode Pointer to the Opcode structure containing the instruction.
 */
void chip8_execute_opcode_load_memory(Chip8 *chip8, Opcode *opcode);

#endif /* CHIP8_OPCODES_H */
