#include "Config/Config.hpp"
#include "Config/DatasetType.hpp"
#include <fmt/format.h>

Config::Config(const std::string& yamlFile) :
    Config(YAML::LoadFile(yamlFile))
{}

Config::Config(const YAML::Node& node) :
    datasetName_(node["dataset"].as<std::string>()),
    years_(node["years"].as<std::vector<int>>()),
    months_(node["months"].as<std::vector<int>>()),
    outputDir_(node["output_dir"].as<std::string>())
{}

std::string Config::getDownloadUrl(int year, int month) const{
    return fmt::format("{}/{}_tripdata_{}-{:02}.parquet",
        baseUrl, getTypeDataset(datasetName_), year, month
    );
}

std::string Config::getOutputPath(int year, int month) const{
    return fmt::format("{}/dataset_{}-{:02}.parquet",
        outputDir_, year, month
    );
}

const std::vector<int>& Config::years() const noexcept{ return years_; }
const std::vector<int>& Config::months() const noexcept{ return months_; }
const std::string& Config::outputDir() const noexcept{ return outputDir_; }
const std::string& Config::datasetName() const noexcept{ return datasetName_; }