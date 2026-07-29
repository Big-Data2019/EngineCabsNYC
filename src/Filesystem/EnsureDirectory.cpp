#include "Filesystem/EnsureDirectory.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>

bool ensureOutputDir(const std::string& outputDir){
    std::filesystem::path currentWorkingDir = std::filesystem::current_path();
    std::filesystem::path routeToDownload = currentWorkingDir / outputDir;
    std::error_code errorCode;

    std::filesystem::create_directories(routeToDownload, errorCode);
    if(std::filesystem::is_directory(routeToDownload) && !errorCode){
        return true;
    } else{
        spdlog::error("Folder failed to load because: {}", errorCode.message());
        return false;
    }
}