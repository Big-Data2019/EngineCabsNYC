#include "Utils/GenerateTimestamp.hpp"
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

std::string generateTimestamp(const std::string& name){
    const auto now = std::chrono::system_clock::now();
    const time_t actualTime = std::chrono::system_clock::to_time_t(now);
    // Divide it into a format of year, month, day, etc
    const tm* timePart = std::localtime(&actualTime);

    std::stringstream ss;
    ss<<std::put_time(timePart, "%Y%m%d%H%M%S");

    return ss.str() + name;
}