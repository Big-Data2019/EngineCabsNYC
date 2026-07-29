#include "Config/ConfigGlobal.hpp"
#include "Config/DatasetType.hpp"
#include <fmt/format.h>

configGlobal::configGlobal(const configYaml& dataYAML)
    : carType(getTypeDataset(dataYAML.datasetName)), outputDir(dataYAML.outputDir)
{}

std::string configGlobal::getDownloadUrl(int year, int month) const{
    return fmt::format("{}/{}_tripdata_{}-{:02}.parquet", baseUrl, carType, year, month);
}

std::string configGlobal::getOutputPath(int year, int month) const{
    return fmt::format("{}/dataset_{}-{:02}.parquet", outputDir, year, month);
}