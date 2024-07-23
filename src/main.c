#include "../include/chip8.h"
#include "../include/chip8_opcodes.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

void print_display(Chip8 *chip8)
{
    printf("/////////////////////////// display ////////////////////////////\n");
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            printf("%c", (chip8_get_display_state(chip8, x, y) ? '#' : '.'));
        }
        printf("\n");
    }
}

void print_keyboard(Chip8 *chip8)
{
    printf("/////////////////////////// keyboard ////////////////////////////\n");
    for (int i = 0; i < KEYBOARD_SIZE; i++) {    
        printf("0x%X : %c\n", i, (chip8_get_keyboard_state(chip8, i) ? '1' : '0'));  
    }
}

void print_opcode(Opcode *opcode)
{
    printf("//////////////////////////// opcode /////////////////////////////\n");
    printf("Instruction: 0x%04X\n", opcode->instruction);
    printf("nnn: 0x%03X\n", opcode->nnn);
    printf("n: 0x%01X\n", opcode->n);
    printf("x: 0x%01X\n", opcode->x);
    printf("y: 0x%01X\n", opcode->y);
    printf("kk: 0x%02X\n", opcode->kk);
}

void clear_terminal()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


typedef struct
{
    char *ui;
    char *type;
    char *data;
    int result;
} Arguments;

Arguments getArgs(int argc, char *argv[]) {
    int opt;
    char *usage = "CHIP 8 EMULATOR FAILED TO START\nUsage: %s --ui <terminal>/<sdl> --type <file>/<raw> --data <path to file>/<bytes>\n";

    Arguments a = {NULL, NULL, NULL, 1};

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
                a.ui = optarg;
                break;
            case 't':
                a.type = optarg;
                break;
            case 'd':
                a.data = optarg;
                break;
            default:
                fprintf(stderr, usage , argv[0]);
                return a;
        }
    }

    // Display parsed arguments
    if (a.ui == NULL || a.type == NULL || a.data == NULL) {
       fprintf(stderr, usage , argv[0]);
       return a;
    }

    a.result = 0;

    return a;
}

int main(int argc, char *argv[])
{

    Arguments args = getArgs(argc, argv);
    if(args.result == 1)
    {
        return 1;
    }
    printf(args.ui);
    printf("\n");
    printf(args.type);
    printf("\n");
    printf(args.data);
    printf("\n");

    Chip8 chip8;

    const uint8_t program[16] = {0xb2, 0x06, 0x66, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x60, 0x1e, 0x61, 0x0a, 0xa2, 0x02, 0xd0, 0x16};

    chip8_init(&chip8);
    chip8_load_ram(&chip8, program, sizeof(program));

    uint8_t result = 0;
    while(!result)
    {
        Opcode opcode = chip8_fetch_opcode(&chip8);

        result = chip8_execute_opcode(&chip8, &opcode);

        clear_terminal();

        print_opcode(&opcode);
        print_display(&chip8);
        
    }

    //chip8_run(&chip8);

    //chip8_set_keyboard_state(&chip8, 0, 1);
    //chip8_set_keyboard_state(&chip8, 3, 1);

    //chip8_set_display_state(&chip8, 0, 0, 1);
    //chip8_set_display_state(&chip8, 32, 16, 1);

    //print_keyboard(&chip8);
    //print_display(&chip8);

    return 0;
}