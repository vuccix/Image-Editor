#include "utils/loader.h"
#include <iostream>
#include <string>

// 0 = ./a.out
// 1 = input path
// 2 = output path
// 3 = effect

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Not enough arguments! <input path> <output path> <effect>\n";
        return EXIT_FAILURE;
    }

    if (!processImage(argv[1], argv[2], argv[3]))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
