# Chip8 Emulator in C

This is a Chip8 emulator written in C. It supports cross-platform usage with two display options: SDL and terminal. Game data can be loaded either from a file or as a raw byte string.

## Features

- **Cross-Platform**: Compatible with various operating systems.
- **Display Options**: Choose between SDL for a graphical interface or terminal for a text-based display.
- **Flexible Data Input**: Load game data from a file or directly provide raw bytes.
- **Build System**: Utilizes CMake for easy compilation and configuration.

## Requirements

- **SDL2**: Required if you choose the SDL display option. Make sure SDL2 is installed on your system.
- **CMake**: Required for building the project.

## Usage

To run the emulator, use the following command-line options:

```bash
./chip8-emulator --ui <terminal|window> --type <file|raw> --data <path to file|bytes>
```


### Parameters

- `--ui <terminal|window>`: Selects the display mode. Use `terminal` for text-based output or `window` for graphical output.
- `--type <file|raw>`: Specifies the method for loading game data. Use `file` to read from a file or `raw` to input raw byte data.
- `--data <path to file|bytes>`: If `--type` is `file`, provide the path to the game file. If `--type` is `raw`, input the raw bytes of the game data as a space-separated list of hexadecimal values.

## Building

To build the project, follow these steps:

0. **Download dependencies**

    - linux:
    ```sh
    sudo apt-get install libsdl2-dev
    ```
    - windows
    ```sh
    https://www.libsdl.org/
    ```

1. **Clone the repository:**

    ```sh
    git clone https://github.com/yourusername/chip8-emulator.git
    cd chip8-emulator
    ```

2. **Create a build directory:**

    ```sh
    mkdir build
    cd build
    ```

3. **Run CMake to configure the project:**

    ```sh
    cmake ..
    ```

4. **Build the project:**

    ```sh
    cmake --build .
    ```

5. **Run the emulator:**

    ```sh
    ./chip8-emulator --ui terminal --type file --data path/to/game.ch8
    ```

## Example Usage




### Terminal Display with File Input

```bash
./chip8-emulator --ui terminal --type file --data games/pong.ch8
```

### SDL Display with Raw Byte Input

```bash
./chip8-emulator --ui window --type raw --data 00E0 6000 6100 6200 6300 6400 6500 6600
```



## Contributing

Feel free to submit issues or pull requests. Contributions are welcome!

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [SDL2](https://www.libsdl.org/) for the graphical display.
- The Chip8 community for the resources and documentation.

## Contact

For any questions or suggestions, please contact [my email](mailto:wojciech.linowski.2137@gmail.com).
