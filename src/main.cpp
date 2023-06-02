#include <algorithm>
#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include <stdio.h>
#include <vector>
#include <functional>

#include <fmt/core.h>
#include <fmt/color.h>

#include "main.h"
#include "rectadj.h"

#define TEST_NAME "DEBUG"
#define TEST_OUTPUT_DIR ".\\data\\"
#define TEST_SIZE 100
#define TEST_SAMPLE_RATE 10
#define DEMO_SIZE 10

#define GENERATE_DATA(n) ( generate_rects_hline(n) )
#define SOLVE(d) ( construct_adjs_smart_search(d) )
#define SOLUTION_TYPE std::vector<adj_t>
#define DATA_TYPE std::vector<rect_t>

int main() {
        const std::string test_name = TEST_NAME;
        const std::string test_ouput_dir = TEST_OUTPUT_DIR;
        const size_t test_size = TEST_SIZE;
        const size_t test_sample_rate = TEST_SAMPLE_RATE;
        const size_t demo_size = DEMO_SIZE;

        fmt::println("{}", fmt::format(fg(fmt::terminal_color::yellow), "Test Details"));
        fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Name"), test_name);
        fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Output directory"), test_ouput_dir);
        fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Size"), test_size);
        fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Sample rate"), test_sample_rate);
        fmt::print("\t{}: {}\n", fmt::format(fg(fmt::terminal_color::blue), "Demo size"), demo_size);
        fmt::print("\n");

        auto all_start_time = std::chrono::high_resolution_clock::now();

        std::filesystem::path out_time_filepath(fmt::format("{}\\{}.csv", test_ouput_dir, test_name));
        FILE* out_time_file = fopen(out_time_filepath.string().c_str(), "w");
        for (size_t current_test = 0; current_test < test_size; ++current_test) {
            fmt::print("\rTest {} / {}", current_test + 1, test_size);

            std::chrono::duration<double, std::milli> sum_time = std::chrono::seconds(0);
            #pragma omp parallel for reduction(add_duration: sum_time)
            for (size_t sample = 0; sample < test_sample_rate; ++sample) {
                // Generate data sample
                auto data = GENERATE_DATA(current_test);

                auto start_time = std::chrono::high_resolution_clock::now();
                // Solve the problem
                auto solution = SOLVE(data);
                auto end_time = std::chrono::high_resolution_clock::now();

                sum_time += end_time - start_time;
            }
            std::chrono::duration<double, std::milli> average_time = sum_time / test_sample_rate;
            fmt::println(out_time_file, "{},{}", current_test + 1, average_time.count());
        }
        fclose(out_time_file);

        fmt::println("\r{:<32}", fmt::format("Finished Tests"));
        fmt::print("\n");

        auto all_end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> net_time = all_end_time - all_start_time;
        fmt::println("{}: {}s", fmt::format(fg(fmt::terminal_color::cyan), "Test took"), net_time.count());

        std::filesystem::path out_data_filepath(fmt::format("{}\\{}_data.csv", test_ouput_dir, test_name));
        std::filesystem::path out_solved_filepath(fmt::format("{}\\{}_solved.csv", test_ouput_dir, test_name));
        FILE* out_data_file = fopen(out_data_filepath.string().c_str(), "w");
        FILE* out_solved_file = fopen(out_solved_filepath.string().c_str(), "w");

        auto data = GENERATE_DATA(demo_size);
        auto solution = SOLVE(data);

        for (size_t i = 0; i < data.size(); ++i) {
            fmt::println(out_data_file, "{}", data[i].to_string());
        }

        for (size_t i = 0; i < solution.size(); ++i) {
            fmt::println(out_solved_file, "{}", solution[i].to_string());
        }

        fclose(out_data_file);
        fclose(out_solved_file);

    return 0;
}