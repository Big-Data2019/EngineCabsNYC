#include "Filesystem/ExistsFile.hpp"
#include <spdlog/spdlog.h>
#include <filesystem>

bool fileExists(const std::string& file){
    if(std::filesystem::exists(file)){
        if(std::filesystem::file_size(file) > 0){
            spdlog::info("File already exists locally, skipping it");
            return true;
        } else{
            std::filesystem::remove(file);
            return false;
        }
    } else{
        return false;
    }
}
