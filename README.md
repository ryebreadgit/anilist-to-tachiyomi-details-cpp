# Manga Details

This program, `main.cpp`, is designed to download Anilist data for a manga and save it in the `details.json` format used by Tachiyomi. It is written in C++ and provides a command-line interface for the user to input a manga ID. The program fetches the necessary information from Anilist's GraphQL API using the cpr and nlohmann/json libraries.

## Prerequisites

- C++ compiler
- cpr library (https://github.com/whoshuu/cpr)
- nlohmann/json library (https://github.com/nlohmann/json)

## Installation

1. Clone the repository or download the source code files.
2. Make sure the cpr and nlohmann/json libraries are properly installed and configured.
3. Compile the program using a C++ compiler with the required libraries linked.

## Usage

1. Run the compiled executable or execute the program.
2. Enter the manga ID when prompted. You can find the manga ID on Anilist's website or through other sources.
3. Wait for the program to download the manga details and save them in the `details.json` file.
4. Once the process is complete, a success message will be displayed.
5. Press Enter to exit the program.

## Dependencies

This program relies on the following third-party libraries:

- cpr: A C++ HTTP library that provides a simple and convenient way to make HTTP requests.
- nlohmann/json: A JSON library for modern C++ that provides a simple and flexible API to work with JSON data.

## Contributing

Contributions to this program are welcome. If you encounter any issues or have suggestions for improvements, please open an issue or submit a pull request on the GitHub repository.

## License

This program is licensed under the MIT License. Feel free to modify and distribute it as needed.

## Acknowledgments

This program was inspired by the need to import manga details from Anilist into Tachiyomi. Thanks to the developers of Anilist for providing the necessary APIs and resources.
