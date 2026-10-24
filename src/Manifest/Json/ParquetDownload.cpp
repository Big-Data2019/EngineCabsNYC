#include "Manifest/Json/ParquetDownload.hpp"
#include "Utils/GenerateTimestamp.hpp"
#include "Manifest/JsonGenerator.hpp"
#include "Manifest/JsonBuilder.hpp"

void generateDownloadManifest(const Config& config, bool successfulDownload, long long elapsedMs){
    JsonBuilder data;
    std::vector<nlohmann::ordered_json> dinamicList;

    for(const auto& year : config.years()){
        for(const auto& month : config.months()){
            nlohmann::ordered_json fileList;
            fileList["year"] = year;
            fileList["month"] = month;
            fileList["path"] = config.getOutputPath(year, month);
            fileList["source"] = "nyc_tlc";
            fileList["status"] = successfulDownload ? "Downloaded" : "Failed";

            dinamicList.push_back(fileList);
        }
    }

    data.newData("dataset", config.datasetName());
    data.manyNewData("files", dinamicList);
    data.newData("download_time", std::to_string(elapsedMs) + "s");

    const std::string manifest = "-manifest.json";
    std::string fileName = generateTimestamp(manifest);

    JsonGenerator generator;
    generator.generateManifest(fileName, data);
}