#include "../include/params.h"

/**
 * Convert raw hexadecimal string to program byte array.
 * 
 * @param hex_string Raw hexadecimal data as a string.
 * @param byte_array Pointer to the buffer where bytes will be stored.
 * @param program_size Pointer to a variable that will receive the program size.
 */
void raw_data_to_program(const char *hex_string, uint8_t *byte_array, size_t *program_size) {
    size_t length = strlen(hex_string);
    size_t byte_array_size = 0;
    const char *pos = hex_string;

    for (size_t i = 0; i < length / 3 + 1 && byte_array_size < PROGRAM_MEMORY_SIZE; i++) {
        if (sscanf(pos, "%2hhx", &byte_array[byte_array_size]) == 1) {
            byte_array_size++;
        }
        pos += 3; // Move to the next byte
    }
    *program_size = byte_array_size;
}

/**
 * Check if a file exists and is readable.
 * 
 * @param path Path to the file.
 * @return 1 if the file exists and is readable, 0 otherwise.
 */
int file_exists_and_readable(const char *path) {
    FILE *file = fopen(path, "r");
    if (file) {
        fclose(file);
        return 1;
    } else {
        return 0;
    }
}

/**
 * Validate the format of raw data.
 * 
 * @param str Raw data string to validate.
 * @return 1 if the raw data is valid, 0 otherwise.
 */
int is_raw_data_valid(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }

    int len = strlen(str);
    if (len % 3 != 2) {
        return 0;
    }

    for (int i = 0; i < len; i++) {
        if (i % 3 == 2) {
            if (str[i] != ' ') {
                return 0;
            }
        } else {
            if (!isxdigit(str[i])) {
                return 0;
            }
        }
    }

    return str[len - 1] != ' ';
}

/**
 * Read a binary file into a program buffer.
 * 
 * @param file_path Path to the binary file.
 * @param program Pointer to the buffer where the program will be loaded.
 * @param program_size Pointer to a variable that will receive the program size.
 */
void read_file_to_program(const char* file_path, uint8_t *program, size_t *program_size) {
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("Failed to open file");
        *program_size = 0;
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size > PROGRAM_MEMORY_SIZE) {
        file_size = PROGRAM_MEMORY_SIZE;
    }

    *program_size = fread(program, 1, file_size, file);
    fclose(file);

    if (*program_size < PROGRAM_MEMORY_SIZE) {
        memset(program + *program_size, 0, PROGRAM_MEMORY_SIZE - *program_size);
    }
}

/**
 * Parse command-line arguments.
 * 
 * @param args Pointer to the Arguments structure to populate.
 * @param argc Argument count.
 * @param argv Argument vector.
 */
void get_args(Arguments *args, int argc, char *argv[]) {
    int opt;
    char *usage = ERROR_MSG "\nUsage: %s --ui <terminal>/<window> --type <file>/<raw> --data <path to file>/<bytes>\n";

    static struct option long_options[] = {
        {"ui", required_argument, 0, 'u'},
        {"type", required_argument, 0, 't'},
        {"data", required_argument, 0, 'd'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "u:t:d:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'u':
                args->ui = optarg;
                break;
            case 't':
                args->type = optarg;
                break;
            case 'd':
                args->data = optarg;
                break;
            default:
                fprintf(stderr, usage, argv[0]);
                exit(1);
        }
    }

    if (args->ui == NULL || args->type == NULL || args->data == NULL) {
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }

    args->result = 0;
}

/**
 * Handle command-line arguments and load program data accordingly.
 * 
 * @param args Pointer to the Arguments structure containing parsed arguments.
 * @param program_data Pointer to the Data structure to store the loaded program.
 */
void handle_args(const Arguments *args, Data *program_data) {
    if (args->result == 1) {
        exit(1);
    }
    if (strstr(args->type, "raw") != NULL) {
        handle_raw_program_data(args->data, program_data);
    } else if (strstr(args->type, "file") != NULL) {
        handle_file_program_data(args->data, program_data);
    } else {
        fprintf(stderr, ERROR_MSG);
        fprintf(stderr, "Invalid type option. Usage: --type <file>/<raw>\n");
        exit(1);
    }
}

/**
 * Handle raw program data and load it into the program buffer.
 * 
 * @param data Raw hexadecimal data as a string.
 * @param program_data Pointer to the Data structure to store the loaded program.
 */
void handle_raw_program_data(const char *data, Data *program_data) {
    if (is_raw_data_valid(data)) {
        raw_data_to_program(data, program_data->program, &program_data->program_size);
    } else {
        fprintf(stderr, ERROR_MSG);
        fprintf(stderr, "Invalid raw data\n");
        exit(1);
    }
}

/**
 * Handle file-based program data and load it into the program buffer.
 * 
 * @param data Path to the program file.
 * @param program_data Pointer to the Data structure to store the loaded program.
 */
void handle_file_program_data(const char *data, Data *program_data) {
    if (file_exists_and_readable(data)) {
        read_file_to_program(data, program_data->program, &program_data->program_size);
    } else {
        fprintf(stderr, ERROR_MSG);
        fprintf(stderr, "Invalid file path\n");
        exit(1);
    }
}
