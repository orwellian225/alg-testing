#include <algorithm>
#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include <stdio.h>
#include <vector>

#include <fmt/core.h>
#include <fmt/color.h>

#include "rectadj.h"

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
    } else if (argc != 5) {
        fmt::println("{}", fmt::format(fg(fmt::terminal_color::red), "ERROR: Incorrect arguments!"));
        return -1;
    }

    const std::string test_name = argv[1];
    const std::string test_ouput_dir = argv[2];
    const size_t test_size = atoi(argv[3]);
    const size_t test_sample_rate = atoi(argv[4]);

    fmt::println("{}", fmt::format(fg(fmt::terminal_color::yellow), "Test Details"));
    fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Name"), test_name);
    fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Output directory"), test_ouput_dir);
    fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Size"), test_size);
    fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Sample rate"), test_sample_rate);
    fmt::print("\n");

    std::filesystem::path out_filepath(fmt::format("{}\\{}.csv", test_ouput_dir, test_name));
    FILE* out_file = fopen(out_filepath.string().c_str(), "w");
    for (size_t current_test = 0; current_test < test_size; ++current_test) {
        fmt::print("\rTest {} / {}", current_test + 1, test_size);

        std::chrono::duration<double, std::milli> sum_time = std::chrono::seconds(0);
        #pragma omp parallel for shared(sum_time)
        for (size_t sample = 0; sample < test_sample_rate; ++sample) {
            // Generate data sample
            std::vector<rect_t> rectangles = generate_rects_random(current_test);

            auto start_time = std::chrono::high_resolution_clock::now();
            // Solve the problem
            std::vector<adj_t> adjacencies = construct_adjs_bf(rectangles);
            auto end_time = std::chrono::high_resolution_clock::now();

            #pragma omp critical
            sum_time += end_time - start_time;
        }
        std::chrono::duration<double, std::milli> average_time = sum_time / test_sample_rate;
        fmt::println(out_file, "{},{}", current_test + 1, average_time.count());
    }
    fclose(out_file);

    fmt::println("\r{:<32}", fmt::format("Finished Tests"));
    fmt::print("\n");

    return 0;
}