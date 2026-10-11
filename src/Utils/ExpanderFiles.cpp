#include "Utils/ExpanderFiles.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>
#include <algorithm>

std::vector<std::string> expandFiles(const std::vector<std::string>& files){
    std::vector<std::string> extractedFiles;

    for(const auto& file : files){
        std::filesystem::path path(file);

        if(path.filename() == "*.parquet"){
            auto dir = path.parent_path();
            if(dir.empty()) dir = ".";
            if(!std::filesystem::exists(dir)){
                spdlog::warn("Directory does not exists: {}", dir.string());
                continue;
            }

            for(const auto& input : std::filesystem::directory_iterator(dir)){
                if(input.is_regular_file() && input.path().extension() == ".parquet"){
                    extractedFiles.push_back(input.path().string());
                }
            }
        } else{
            if(std::filesystem::exists(path)){
                extractedFiles.push_back(path.string());
            } else{
                spdlog::warn("File does not exists: {}", path.string());
            }
        }
    }

    std::sort(extractedFiles.begin(), extractedFiles.end());
    return extractedFiles;
}