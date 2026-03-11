#ifndef MACROS_HPP
#define MACROS_HPP

#include <chrono>
#include <iostream>

// Claude just generated this macro for me. Very useful.
#define START_TIMING(name) auto _start_##name = std::chrono::high_resolution_clock::now();

#define END_TIMING(name) { \
    auto _end_##name = std::chrono::high_resolution_clock::now(); \
    auto _us_##name = std::chrono::duration_cast<std::chrono::microseconds>(_end_##name - _start_##name).count(); \
    std::cout << #name << ": " << _us_##name << "us\n"; \
}





#endif // MACROS_HPP