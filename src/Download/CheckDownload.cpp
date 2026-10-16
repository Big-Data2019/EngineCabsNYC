#include "Download/CheckDownload.hpp"
#include "Filesystem/File.hpp"
#include "Download/parquetFiles.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>

namespace{
bool readyToProcess(const std::string& outputPath, const std::string& url, bool exists){
    if(!exists){
        if(!downloadFiles(url, outputPath)){
            spdlog::error("The download failed for the period");
            return false;
        }
    }

    std::uintmax_t bytes = std::filesystem::file_size(outputPath);
    double megabytes = bytes / (1024.0 * 1024.0);
    spdlog::info("File size: {:.2f} Mb\n", megabytes);
    return true;
}
}

bool verifyDownload(const Config& config){
    bool ready = true;

    for(auto& year : config.years()){
        for(auto& month : config.months()){
            std::string outputPath = config.getOutputPath(year, month);
            std::string url = config.getDownloadUrl(year, month);
            bool exists = existsFile(outputPath);

            spdlog::info("Link: {}", url);
            ready = (!readyToProcess(outputPath, url, exists)) ? false : true;
        }
    }

    return (!ready) ? false : true;
}