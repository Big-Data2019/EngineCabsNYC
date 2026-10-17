#pragma once

#include <yaml-cpp/yaml.h>
#include <string_view>
#include <vector>

class Config final{
private:
    explicit Config(const YAML::Node& node);
    static constexpr std::string_view baseUrl = "https://d37ci6vzurychx.cloudfront.net/trip-data";

    const std::string datasetName_;
    const std::string outputDir_;
    std::vector<int> years_;
    std::vector<int> months_;

public:
    explicit Config(const std::string& yamlFile);
    std::string getDownloadUrl(int year, int month) const;
    std::string getOutputPath(int year, int month) const;

    const std::vector<int>& years() const noexcept;
    const std::vector<int>& months() const noexcept;
    const std::string& datasetName() const noexcept;
    const std::string& outputDir() const noexcept;
};