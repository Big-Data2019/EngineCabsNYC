#include "Download/parquetFiles.hpp"
#include "Network/CurlEasy.hpp"
#include <spdlog/spdlog.h>
#include <filesystem>
#include <fstream>

namespace{
// Function to receive the network data and insert in the outputFile
size_t writeDataCallback(void* ptr, size_t size, size_t numberMember, void* stream){
    size_t totalBytes = size * numberMember;
    std::ofstream* outputFile = static_cast<std::ofstream*>(stream);

    // Insert the binary data from the .parquet file into the outputFile
    outputFile->write(static_cast<const char*>(ptr), totalBytes);

    /*
    Returns totalBytes because if the amount is 0 it means a failure,
    either due to lack of space or a download error
    */
    return totalBytes;
}
}

bool downloadFiles(const std::string& url, const std::string& outputPath){
    netEasy::CurlEasy curll;
    CURL* curl = curll.get();

    // Open the outputFile in binary form
    std::ofstream outputFile(outputPath, std::ios::binary);
    if(!outputFile.is_open()){
        "Failed to create or open local outputfile";
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeDataCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outputFile);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    // Starts the file transfer
    CURLcode resCode = curl_easy_perform(curl);
    outputFile.close();

    if(resCode != CURLE_OK){
        // If the download failed halfway through, delete the incomplete file to prevent corruption
        if(std::filesystem::exists(outputPath)){
            std::filesystem::remove(outputPath);
        }

        spdlog::error("Libcurl transfer failed: {}", curl_easy_strerror(resCode));
        return false;
    }

    return true;
}