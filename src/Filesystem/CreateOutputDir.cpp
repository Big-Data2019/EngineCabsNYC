#include "Filesystem/CreateOutputDir.hpp"
#include <spdlog/spdlog.h>
#include <filesystem>

bool createOutputDir(const std::string& outputDir){
    std::filesystem::path pathToDownload(outputDir);
    std::error_code errorCode;

    std::filesystem::create_directories(pathToDownload, errorCode);
    if(std::filesystem::is_directory(pathToDownload) && !errorCode){
        return true;
    } else{
        spdlog::error("Folder failed to load because: {}", errorCode.message());
        return false;
    }
}