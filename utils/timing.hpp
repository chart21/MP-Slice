#pragma once
#include <chrono>
#include <string>
#include <iostream>
template <typename F, typename... Args>
auto funcTime(std::string printText, F func, Args &&...args)
{
    std::chrono::high_resolution_clock::time_point t1 =
        std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...);
#ifdef US
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(
                     std::chrono::high_resolution_clock::now() - t1)
                     .count();
     std::cout << "---TIMING--- " << time << "us " << printText << '\n';
#else
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - t1)
                    .count();
    std::cout << "---TIMING--- " << time << "ms " << printText << '\n';
#endif
    return time;
}

