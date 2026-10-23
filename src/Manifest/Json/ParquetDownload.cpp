#include "Manifest/Json/ParquetDownload.hpp"
#include "Manifest/JsonGenerator.hpp"
#include "Download/VerifyDownload.hpp"
#include "Manifest/JsonBuilder.hpp"
#include "Utils/MeasureTime.hpp"
#include "Utils/GenerateTimestamp.hpp"

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