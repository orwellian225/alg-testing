#pragma once

#include <chrono>

#pragma omp declare reduction(add_duration : std::chrono::duration<double, std::milli> : omp_out += omp_in)
