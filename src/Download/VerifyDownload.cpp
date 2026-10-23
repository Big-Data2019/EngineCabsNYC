#include "Download/VerifyDownload.hpp"
#include "Download/parquetFiles.hpp"
#include "Filesystem/ExistsFile.hpp"
#include <spdlog/spdlog.h>
#include <filesystem>

bool verifyDownload(const Config& config){
    bool ready = true;

    for(const auto& year : config.years()){
        for(const auto& month : config.months()){
            const std::string output = config.getOutputPath(year, month);
            const std::string url = config.getDownloadUrl(year, month);

            if(!fileExists(output)){
                if(!downloadParquetFiles(url, output)){
                    spdlog::error("The download failed for the period: {}, {}", year, month);
                    ready = false;
                    continue;
                }
            }

            std::uintmax_t bytes = std::filesystem::file_size(output);
            double megaBytes = bytes / (1024.0 * 1024.0);

            spdlog::info("Link: {}", url);
            spdlog::info("File size: {:.2f} Mb\n", megaBytes);
        }
    }

    return ready;
}