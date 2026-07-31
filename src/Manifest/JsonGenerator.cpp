#include "Manifest/JsonGenerator.hpp"
#include <spdlog/spdlog.h>
#include <fstream>

void JsonGenerator::generateManifest(const std::string& fileName, const JsonBuilder& readyBuilder){
    nlohmann::ordered_json jsonFile = readyBuilder.build();
    std::ofstream file(fileName);
    if(file.is_open()){
        file<<jsonFile.dump(4);
        file.close();
        spdlog::info("Manifest: {}, was created", fileName);
    } else{
        spdlog::warn("Manifest: {}, could not be created", fileName);
    }
}