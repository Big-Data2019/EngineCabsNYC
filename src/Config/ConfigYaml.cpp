#include "Config/ConfigYaml.hpp"

// Constructor to get the data from the YAML and put it into variables
configYaml::configYaml(const YAML::Node& configFile){
    datasetName = configFile["dataset"].as<std::string>();
    years = configFile["years"].as<std::vector<int>>();
    months = configFile["months"].as<std::vector<int>>();
    outputDir = configFile["output_dir"].as<std::string>();
}