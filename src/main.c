#include "../include/chip8.h"
#include "../include/chip8_opcodes.h"
#include "../include/keyboard.h"
#include "../include/display.h"
#include "../include/params.h"
#include "../include/utils.h"

/**
 * @brief Initializes the UI based on user arguments.
 *
 * Sets up the SDL window and creates a display if the UI type is "window".
 *
 * @param args The command-line arguments specifying UI options.
 * @param display Pointer to a Display structure that will be initialized.
 */
static void initializeUI(const Arguments *args, Display *display)
{
    if (strstr(args->ui, "window") != NULL) {
        initSDL();
        *display = createDisplay();
    }
}

/**
 * @brief Handles user input and updates the display.
 *
 * Polls for SDL events, executes Chip8 opcodes, updates the display, and handles sound.
 *
 * @param chip8 Pointer to the Chip8 emulator instance.
 * @param args The command-line arguments specifying UI options.
 * @param display Pointer to a Display structure used for rendering.
 * @param result Pointer to a uint8_t that will be set to indicate the exit status.
 */
static void handleInputAndDisplay(Chip8 *chip8, const Arguments *args, Display *display, uint8_t *result)
{
    SDL_Event e;

    // Poll for SDL events
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            *result = 1; // Set result to indicate exit
            return;
        }
    }

    // Fetch and execute the next opcode
    Opcode opcode = chip8_fetch_opcode(chip8);
    *result = chip8_execute_opcode(chip8, &opcode);

    // Check if a sound should be played
    if (chip8_should_buzz(chip8)) {
        sound_buzzer();
    }

    // Update the display based on the UI type
    if (strstr(args->ui, "terminal") != NULL) {
        if (read_keyboard(chip8)) {
            *result = 1; // Set result to indicate exit
        }
        show_terminal_display(chip8);
    } else if (strstr(args->ui, "window") != NULL) {
        if (read_keyboard_sdl(chip8)) {
            *result = 1; // Set result to indicate exit
        }
        show_sdl_display(display, chip8);
        print_opcode(&opcode);
    }
}

/**
 * @brief Cleans up resources used by the application.
 *
 * Removes the display and performs any other necessary cleanup.
 *
 * @param display Pointer to a Display structure to be cleaned up.
 */
static void cleanup(Display *display)
{
    if (display) {
        removeDisplay(display);
    }
}

/**
 * @brief Main application function to run the CHIP-8 emulator.
 *
 * Handles command-line arguments, initializes the emulator, and manages
 * the main event loop for processing input and updating the display.
 *
 * @param argc The number of command-line arguments.
 * @param argv Array of command-line argument strings.
 *
 * @return Exit status code.
 */
int run_app(int argc, char *argv[])
{
    Data data = {0};
    Arguments args = {NULL, NULL, NULL, 0};
    
    // Parse and handle command-line arguments
    get_args(&args, argc, argv);
    handle_args(&args, &data);
    print_launch_options(&args);
    
    // Initialize CHIP-8 emulator and load program
    Chip8 chip8;
    chip8_init(&chip8);
    chip8_load_ram(&chip8, data.program, data.program_size);

    Display display;
    SDL_Event e;
    uint8_t result = 0;

    // Initialize UI components
    initializeUI(&args, &display);

    // Main application loop
    while (!result) {
        handleInputAndDisplay(&chip8, &args, &display, &result);
    }

    // Perform cleanup before exiting
    cleanup(&display);

    SDL_Quit();
    return 0;
}

/**
 * @brief Main entry point of the application.
 *
 * Calls the run_app function to start the CHIP-8 emulator.
 *
 * @param argc The number of command-line arguments.
 * @param argv Array of command-line argument strings.
 *
 * @return Exit status code from run_app.
 */
int main(int argc, char *argv[]) {
    return run_app(argc, argv);
}
