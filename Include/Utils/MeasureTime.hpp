#pragma once

#include <chrono>
#include <utility>

template <typename Func, typename... Args>
// Measures the duration from the start to an end time
auto timeLapse(Func&& function, Args&&... args){
    auto start = std::chrono::high_resolution_clock::now();
    /*
    Call the function, Forward the arguments exactly as they arrived
    And it stores the result of the function in the variable "result"
    */
    auto result = std::forward<Func>(function)(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();

    long long seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    return std::make_pair(seconds, result);
}
