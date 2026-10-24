#pragma once

#include <utility>
#include <chrono>

template <typename Func, typename... Args>
[[nodiscard]]
auto timeLapse(Func&& function, Args&&... args){
    auto start = std::chrono::steady_clock::now();

    // Call the function, forward the arguments exactly as they arrived
    auto result = std::forward<Func>(function)(std::forward<Args>(args)...);

    auto end = std::chrono::steady_clock::now();
    long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    return std::make_pair(elapsed, result);
}