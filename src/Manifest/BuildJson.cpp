#include "Manifest/BuildJson.hpp"
#include "Utils/MeasureTime.hpp"
#include "Download/CheckDownload.hpp"

nlohmann::ordered_json buildManifestJson(const configYaml& dataYAML, const configGlobal& configGl){
    nlohmann::ordered_json dataJson;
    auto [time, download] = timeLapse(verifyDownload, dataYAML, configGl);
    dataJson["dataset"] = dataYAML.datasetName;
    dataJson["files"] = nlohmann::ordered_json::array();
    dataJson["download_time"] = std::to_string(time) + "s";

    for(auto& year : dataYAML.years){
        for(auto& month : dataYAML.months){
            dataJson["files"].push_back({
                {"year", year},
                {"month", month},
                {"path", configGl.getOutputPath(year, month)},
                {"source", "nyc_tlc"},
                {"status", download ? "Downloaded" : "Failed"}
            });
        }
    }

    return dataJson;
}
