#include "Filesystem/File.hpp"
#include <spdlog/spdlog.h>
#include <filesystem>

bool existsFile(const std::string& file){
    if(std::filesystem::exists(file)){
        if(std::filesystem::file_size(file) > 0){
            spdlog::info("File already exists locally, skipping download");
            return true;
        } else{
            spdlog::warn("File {} is corrupted, re-downloading", file);
            std::filesystem::remove(file);
            return false;
        }
    } else{
        return false;
    }
}
