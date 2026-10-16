#include "Manifest/Json/ParquetDownload.hpp"
#include "Manifest/JsonBuilder.hpp"
#include "Manifest/JsonGenerator.hpp"
#include "Utils/MeasureTime.hpp"
#include "Download/CheckDownload.hpp"

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

namespace Manifest{
void generateDownloadManifest(const Config& config){
    JsonBuilder data;
    std::vector<nlohmann::ordered_json> dinamicList;
    auto[time, download] = timeLapse(verifyDownload, config);

    for(auto& year : config.years()){
        for(auto& month : config.months()){
            nlohmann::ordered_json fileList;
            fileList["year"] = year;
            fileList["month"] = month;
            fileList["path"] = config.getOutputPath(year, month);
            fileList["source"] = "nyc_tlc";
            fileList["status"] = download ? "Downloaded" : "Failed";

            dinamicList.push_back(fileList);
        }
    }

    data.newData("dataset", config.datasetName());
    data.manyNewData("files", dinamicList);
    data.newData("download_time", std::to_string(time) + "s");

    const std::string manifest = "-manifest.json";
    std::string fileName = generateTimestamp(manifest);

    JsonGenerator generator;
    generator.generateManifest(fileName, data);
}
}