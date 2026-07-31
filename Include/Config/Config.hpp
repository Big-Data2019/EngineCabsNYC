#pragma once
#include <string>
#include <vector>
#include <string_view>
#include <yaml-cpp/yaml.h>

class Config final{
private:
    explicit Config(const YAML::Node& node);
    static constexpr std::string_view baseUrl = "https://d37ci6vzurychx.cloudfront.net/trip-data";

    std::string datasetName_;
    std::string outputDir_;
    std::vector<int> years_;
    std::vector<int> months_;

public:
    explicit Config(const std::string& yamlFile);
    std::string downloadUrl(int year, int month) const;
    std::string outputPath(int year, int month) const;

    const std::vector<int>& years() const noexcept;
    const std::vector<int>& months() const noexcept;
    const std::string& datasetName() const noexcept;
    const std::string& outputDir() const noexcept;
};