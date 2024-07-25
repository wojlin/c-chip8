#include "../include/chip8.h"
#include "../include/chip8_opcodes.h"
#include "../include/keyboard.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <SDL2/SDL.h>

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)  // Sleep function on Windows
void sound_buzzer() {
    Beep(1000, 100);  // Frequency 1000 Hz, Duration 100 ms
}
#else
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/kd.h>  // For KIOCSOUND and KDMKTONE
#define sleep_ms(ms) usleep((ms) * 1000)  // usleep takes microseconds on Unix-like systems
void sound_buzzer() {
    printf("\a");  // Fallback to ASCII Bell character
    fflush(stdout); 
}
#endif


#define ERROR_MSG "CHIP 8 EMULATOR FAILED TO START\n"
#define PIXEL_SIZE 20 // Size of each pixel in the window
#define ZERO_R 0
#define ZERO_G 0
#define ZERO_B 0
#define ONE_R 255
#define ONE_G 255
#define ONE_B 255

typedef struct {
    char *ui;
    char *type;
    char *data;
    int result;
} Arguments;

typedef struct {
    size_t program_size;
    uint8_t program[PROGRAM_MEMORY_SIZE];
} Data;

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} Display;


// Function Prototypes
void print_launch_options(const Arguments *args);
void print_display(Chip8 *chip8);
void print_timers(Chip8 *chip8);
void print_keyboard(Chip8 *chip8);
void print_opcode(const Opcode *opcode);
void clear_terminal();
void hex_string_to_byte_array(const char *hex_string, uint8_t *byte_array, size_t *program_size);
int file_exists_and_readable(const char *path);
int is_valid_format(const char *str);
void read_file_to_program(const char* file_path, uint8_t *program, size_t *program_size);
void get_args(Arguments *args, int argc, char *argv[]);
void handle_args(const Arguments *args, Data *program_data);
void handle_raw_data(const char *data, Data *program_data);
void handle_file_data(const char *data, Data *program_data);
int run_app(int argc, char *argv[]);
Display createDisplay();
void removeDisplay(Display * display);
void drawDisplay(Display * display, Chip8 *chip8);
void initSDL();

// Function Definitions

void print_launch_options(const Arguments *args) {
    printf("/////////////////////// launch options ///////////////////////////\n");
    printf("ui: %s\n", args->ui);
    printf("type: %s\n", args->type);
    printf("data: %s\n", args->data);
}

void print_display(Chip8 *chip8) {
    if(chip8->display_changed)
    {
        clear_terminal();
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            for (int x = 0; x < DISPLAY_WIDTH; x++) {
                printf("%c", (chip8_get_display_state(chip8, x, y) ? '#' : '.'));
            }
            printf("\n");
        }
        chip8->display_changed = 0;
    } 
}

void print_timers(Chip8 *chip8) {
    printf("/////////////////////////// timers ////////////////////////////\n");
    printf("sound timer: %u   ", chip8->sound_timer);  
    printf("delay timer: %u ", chip8->delay_timer);  
    printf("\n");  
}

void print_keyboard(Chip8 *chip8) {
    printf("/////////////////////////// keyboard ////////////////////////////\n");
    for (int i = 0; i < KEYBOARD_SIZE; i++) {
        printf("0x%X : %c\n", i, (chip8_get_keyboard_state(chip8, i) ? '1' : '0'));  
    }
}

void print_opcode(const Opcode *opcode) {
    printf("//////////////////////////// opcode /////////////////////////////\n");
    printf("Instruction: 0x%04X\n", opcode->instruction);
    printf("nnn: 0x%03X\n", opcode->nnn);
    printf("n: 0x%01X\n", opcode->n);
    printf("x: 0x%01X\n", opcode->x);
    printf("y: 0x%01X\n", opcode->y);
    printf("kk: 0x%02X\n", opcode->kk);
}

void clear_terminal() {
    #ifdef _WIN32
        system("cls");
    #else
        int res = system("clear");
    #endif
}

void hex_string_to_byte_array(const char *hex_string, uint8_t *byte_array, size_t *program_size) {
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

int file_exists_and_readable(const char *path) {
    FILE *file = fopen(path, "r");
    if (file) {
        fclose(file);
        return 1;
    } else {
        return 0;
    }
}

int is_valid_format(const char *str) {
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

    if (str[len - 1] == ' ') {
        return 0;
    }

    return 1;
}

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

void handle_args(const Arguments *args, Data *program_data) {
    if (args->result == 1) {
        exit(1);
    }
    if (strstr(args->type, "raw") != NULL) {
        handle_raw_data(args->data, program_data);
    } else if (strstr(args->type, "file") != NULL) {
        handle_file_data(args->data, program_data);
    } else {
        printf(ERROR_MSG);
        printf("invalid type option. usage: --type <file>/<raw>\n");
        exit(1);
    }
}

void handle_raw_data(const char *data, Data *program_data) {
    if (is_valid_format(data)) {
        hex_string_to_byte_array(data, program_data->program, &program_data->program_size);
    } else {
        printf(ERROR_MSG);
        printf("invalid raw data\n");
        exit(1);
    }
}

void handle_file_data(const char *data, Data *program_data) {
    if (file_exists_and_readable(data)) {
        read_file_to_program(data, program_data->program, &program_data->program_size);
    } else {
        printf(ERROR_MSG);
        printf("invalid file path\n");
        exit(1);
    }
}


void initSDL()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
}

Display createDisplay() {
    Display display;

    // Create a window
    display.window = SDL_CreateWindow("CHIP8 EMULATOR",
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      DISPLAY_WIDTH * PIXEL_SIZE, DISPLAY_HEIGHT * PIXEL_SIZE,
                                      SDL_WINDOW_SHOWN);
    if (display.window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    // Create a renderer
    display.renderer = SDL_CreateRenderer(display.window, -1, SDL_RENDERER_ACCELERATED);
    if (display.renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(display.window);
        SDL_Quit();
        exit(1);
    }

    return display;
}

void removeDisplay(Display * display)
{
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);
    SDL_Quit();
}

void drawDisplay(Display *display, Chip8 *chip8) {
    if(chip8->display_changed)
    {
        for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
            for (int x = 0; x < DISPLAY_WIDTH; ++x) {
                if (chip8->display[y] & ((uint64_t)1 << (DISPLAY_WIDTH - 1 - x))) {
                    SDL_SetRenderDrawColor(display->renderer, ONE_R, ONE_G, ONE_B, SDL_ALPHA_OPAQUE); // White
                } else {
                    SDL_SetRenderDrawColor(display->renderer, ZERO_R, ZERO_G, ZERO_B, SDL_ALPHA_OPAQUE); // Black
                }
                SDL_Rect rect = { x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE };
                SDL_RenderFillRect(display->renderer, &rect);
            }
        }
        SDL_RenderPresent(display->renderer); // Update the display
        chip8->display_changed = 0;
    }
}

int run_app(int argc, char *argv[])
{
    Data data = {0};
    Arguments args = {NULL, NULL, NULL, 0};
    get_args(&args, argc, argv);
    handle_args(&args, &data);
    print_launch_options(&args);
    Chip8 chip8;
    chip8_init(&chip8);
    chip8_load_ram(&chip8, data.program, data.program_size);

    
    Display display;
    SDL_Event e;

    if(strstr(args.ui, "window") != NULL)
    {
        initSDL();
        display = createDisplay();
    }

    uint8_t result = 0;

    
    while (!result) {

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) 
            {
                result = 1;
                break;
            }
        }

        

        Opcode opcode = chip8_fetch_opcode(&chip8);
        result = chip8_execute_opcode(&chip8, &opcode);
        if(chip8_should_buzz(&chip8))
        {
            sound_buzzer();
        }     

        if(strstr(args.ui, "terminal") != NULL)
        {
            if(read_keyboard(&chip8))
            {
                result = 1;
                break;
            }
            print_display(&chip8);
        }
        else if(strstr(args.ui, "window") != NULL)
        {
            if(read_keyboard_sdl(&chip8))
            {
                result = 1;
                break;
            }
            drawDisplay(&display, &chip8);
            print_opcode(&opcode);
        }

        //sleep_ms(8000);
        //print_opcode(&opcode);
    }

    if (strstr(args.ui, "window") != NULL) {
        removeDisplay(&display);
    }

    SDL_Quit();

    return 0;
}

int main(int argc, char *argv[]) {
    return run_app(argc, argv);
}
