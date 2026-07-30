#include "Manifest/BuildJson.hpp"
#include <chrono>
#include <iomanip>
#include <fstream>
#include <spdlog/spdlog.h>

namespace{
std::string generateTimestamp(const std::string& name){
    auto now = std::chrono::system_clock::now();
    time_t timeActual = std::chrono::system_clock::to_time_t(now);
    // Divide it into a format of year, month, day, etc
    tm* timePart = localtime(&timeActual);

    std::stringstream ss;
    ss<<std::put_time(timePart, "%Y%m%d%H%M%S");

    std::string timestamp = ss.str() + name;
    return timestamp;
}
}

bool generateJsonFile(const configYaml& dataYAML, const configGlobal& configGl){
    try{
        const std::string manifest = "-manifest.json";
        std::string fileName = generateTimestamp(manifest);

        std::ofstream file(fileName);
        if(!file.is_open()){
            spdlog::error("Failed to open or create local output file");
            return false;
        }

        auto jsonData = buildManifestJson(dataYAML, configGl);
        file<<std::setw(4)<<jsonData;
        return file.good();
    } catch(const std::exception& e){
        spdlog::error("Error: {}", e.what());
        return false;
    }
}
