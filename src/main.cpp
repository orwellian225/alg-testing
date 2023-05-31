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

#define TEST_NAME "BRUTE_FORCE_BC"
#define TEST_OUTPUT_DIR ".\\data\\"
#define TEST_SIZE 1000
#define TEST_SAMPLE_RATE 1000
#define DEMO_SIZE 10

#define GENERATE_DATA(n) ( generate_rects_hline(n) )
#define SOLVE(d) ( construct_adjs_bf(d) )

std::vector<std::string> names = { 
    "BRUTE_FORCE_BC", "BRUTE_FORCE_WC", "BRUTE_FORCE_RC",
    "SMART_SEARCH_BC", "SMART_SEARCH_WC", "SMART_SEARCH_RC",
    "SWEEP_LINE_BC", "SWEEP_LINE_WC", "SWEEP_LINE_RC",
    "Brute_Force_Best", "Brute_Force_Worst", "Brute_Force_Ave",
    "Optimised_Linear_Search_Best", "Optimised_Linear_Search_Worst", "Optimised_Linear_Search_Ave",
    "Line_Sweep_Best", "Line_Sweep_Worst", "Line_Sweep_Ave",
};

std::vector<std::function<std::vector<rect_t>(size_t)>> gen_data = {
    generate_rects_hline, generate_rects_vline, generate_rects_random,
    generate_rects_hline, generate_rects_vline, generate_rects_random,
    generate_rects_hline, generate_rects_vline, generate_rects_random,
    generate_rects_hline, generate_rects_vline, generate_rects_random,
    generate_rects_hline, generate_rects_vline, generate_rects_random,
    generate_rects_hline, generate_rects_vline, generate_rects_random,
};

std::vector<std::function<std::vector<adj_t>(std::vector<rect_t>&)>> execute = {
    construct_adjs_bf, construct_adjs_bf, construct_adjs_bf,
    construct_adjs_smart_search, construct_adjs_smart_search, construct_adjs_smart_search,
    construct_adjs_sweep_line, construct_adjs_sweep_line, construct_adjs_sweep_line,
    construct_adjs_bf, construct_adjs_bf, construct_adjs_bf,
    construct_adjs_smart_search, construct_adjs_smart_search, construct_adjs_smart_search,
    construct_adjs_sweep_line, construct_adjs_sweep_line, construct_adjs_sweep_line,
};

int main() {
    for (size_t k = 0; k < names.size(); ++k) {
        const std::string test_name = names[k];
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
                auto data = gen_data[k](current_test);

                auto start_time = std::chrono::high_resolution_clock::now();
                // Solve the problem
                auto solution = execute[k](data);
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

        std::vector<rect_t> data = gen_data[k](demo_size);
        std::vector<adj_t> solution = execute[k](data);

        for (size_t i = 0; i < data.size(); ++i) {
            fmt::println(out_data_file, "{}", data[i].to_string());
        }

        for (size_t i = 0; i < solution.size(); ++i) {
            fmt::println(out_solved_file, "{}", solution[i].to_string());
        }

        fclose(out_data_file);
        fclose(out_solved_file);
    }

    return 0;
}