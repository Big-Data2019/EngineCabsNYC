#pragma once
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

struct configYaml{
    std::string datasetName;
    std::vector<int> years;
    std::vector<int> months;
    std::string outputDir;

    explicit configYaml(const YAML::Node& configFile);
};