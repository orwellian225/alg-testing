#include <iostream>
#include <string>

#include <fmt/core.h>
#include <fmt/color.h>

int main(int argc, char* argv[]) {

    if (argc == 2) {
        const char* arg = argv[1];
        if (strcmp(arg, "algorithms") == 0) {
            // Print Algorithms
            fmt::println("{}:", fmt::format(fg(fmt::terminal_color::yellow), "Available algorithms to test"));
            return 0;
        } else if (strcmp(arg, "help") == 0 || strcmp(arg, "help") == 0 || strcmp(arg, "h") == 0 ) {
            fmt::println("{}:", fmt::format(fg(fmt::terminal_color::yellow), "How to use"));
            return 0;
        } else {
            fmt::println("{}", fmt::format(fg(fmt::terminal_color::red), "ERROR: Incorrect arguments!"));
            return -1;
        }
    } else if (argc != 6) {
        fmt::println("{}", fmt::format(fg(fmt::terminal_color::red), "ERROR: Incorrect arguments!"));
        return -1;
    }

    const std::string test_name = argv[1];
    const std::string test_ouput_dir = argv[2];
    const size_t test_size = atoi(argv[3]);
    const size_t test_sample_rate = atoi(argv[4]);
    const std::string test_algorithm = argv[5];

    fmt::println("{}", fmt::format(fg(fmt::terminal_color::yellow), "Test Details"));
    fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Name"), test_name);
    fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Output directory"), test_ouput_dir);
    fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Size"), test_size);
    fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Sample rate"), test_sample_rate);
    fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Algorithm"), test_algorithm);
    fmt::print("\n");

    for (size_t current_test = 0; current_test < test_size; ++current_test) {
        fmt::print("\rTest {} / {}", current_test + 1, test_size);

        #pragma omp parallel for
        for (size_t sample = 0; sample < test_sample_rate; ++sample) {
            // Conduct Test
        }

    }
    fmt::println("\r{:<32}", fmt::format("Finished Tests"));
    fmt::print("\n");

    return 0;
}