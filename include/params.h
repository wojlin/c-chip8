#ifndef PARAMS_H
#define PARAMS_H

#include "chip8.h"

#include <stdint.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>

/** Error message printed on failure to start the CHIP-8 emulator. */
#define ERROR_MSG "CHIP 8 EMULATOR FAILED TO START\n"

/** Structure to hold command-line arguments. */
typedef struct {
    char *ui;       /**< User interface type (e.g., terminal, window). */
    char *type;     /**< Type of program data (e.g., file, raw). */
    char *data;     /**< Path to file or raw data bytes. */
    int result;     /**< Result status of argument parsing. */
} Arguments;

/** Structure to hold program data and its size. */
typedef struct {
    size_t program_size; /**< Size of the loaded program. */
    uint8_t program[PROGRAM_MEMORY_SIZE]; /**< Memory buffer to hold the program. */
} Data;

/** Check if a file exists and is readable.
 * 
 * @param path Path to the file.
 * @return 1 if the file exists and is readable, 0 otherwise.
 */
int file_exists_and_readable(const char *path);

/** Validate the format of raw data.
 * 
 * @param str Raw data string to validate.
 * @return 1 if the raw data is valid, 0 otherwise.
 */
int is_raw_data_valid(const char *str);

/** Read a binary file into a program buffer.
 * 
 * @param file_path Path to the binary file.
 * @param program Pointer to the buffer where the program will be loaded.
 * @param program_size Pointer to a variable that will receive the program size.
 */
void read_file_to_program(const char* file_path, uint8_t *program, size_t *program_size);

/** Convert raw hexadecimal string to program byte array.
 * 
 * @param hex_string Raw hexadecimal data as a string.
 * @param byte_array Pointer to the buffer where bytes will be stored.
 * @param program_size Pointer to a variable that will receive the program size.
 */
void raw_data_to_program(const char *hex_string, uint8_t *byte_array, size_t *program_size);

/** Handle raw program data and load it into the program buffer.
 * 
 * @param data Raw hexadecimal data as a string.
 * @param program_data Pointer to the Data structure to store the loaded program.
 */
void handle_raw_program_data(const char *data, Data *program_data);

/** Handle file-based program data and load it into the program buffer.
 * 
 * @param data Path to the program file.
 * @param program_data Pointer to the Data structure to store the loaded program.
 */
void handle_file_program_data(const char *data, Data *program_data);

/** Parse command-line arguments.
 * 
 * @param args Pointer to the Arguments structure to populate.
 * @param argc Argument count.
 * @param argv Argument vector.
 */
void get_args(Arguments *args, int argc, char *argv[]);

/** Handle command-line arguments and load program data accordingly.
 * 
 * @param args Pointer to the Arguments structure containing parsed arguments.
 * @param program_data Pointer to the Data structure to store the loaded program.
 */
void handle_args(const Arguments *args, Data *program_data);

#endif // PARAMS_H
