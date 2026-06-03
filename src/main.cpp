#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>
#include <CLI/CLI.hpp>
#include <string>
#include <vector>
#include <filesystem>
#include <future>
#include <thread>
#include <arrow/api.h>
#include <arrow/io/api.h>
#include <arrow/result.h>
#include <arrow/util/io_util.h>
#include <iostream>

namespace fs = std::filesystem;

// Struct with the data of the yaml
struct configYaml{
    std::string datasetName;
    std::vector<int> years;
    std::vector<int> months;

    // Constructor to get the data from the YAML and put it into the variables
    configYaml(const YAML::Node& configFile){
        datasetName = configFile["dataset"].as<std::string>();
        years = configFile["years"].as<std::vector<int>>();
        months = configFile["months"].as<std::vector<int>>();
    }
};

// Function to read the data of YAML configuration
static void processData(const configYaml& config){
    for(const auto& year : config.years){
        spdlog::info("Years to process: {}", year);

        for(const auto& month : config.months){
            spdlog::info("Months to process: {}", month);
        }
    }
}

int main(int argc, char* argv[]){
    const std::string configPath = "dataset_config.yaml";
 
    YAML::Node configFile = YAML::LoadFile(configPath);
    CLI::App app{"EngineCabsNYC: Program to download taxi datasets in New York"};

    try{
        configYaml config(configFile);

        app.add_option("-d, --dataset", config.datasetName, "Change dataset name");
        app.add_option("-y, --years", config.years, "Change years to process");
        app.add_option("-m, --months", config.months, "Change months to process");

        CLI11_PARSE(app, argc, argv)

        spdlog::info("Dataset to process: {}\n", config.datasetName);

        processData(config);

        std::vector<std::string> people = {"Juan_Perez", "Maria_Gomez", "Carlos_Lopez"};
        fs::path file = "test";

        for (const auto& name : people) {
        fs::path path = file / name; 

        fs::create_directories(path);
        std::cout << "File created to: " << name << "\n";
        }

    } catch(const YAML::Exception& e){
        spdlog::error("Error: {}", e.what());
        return 1;
    }

    return 0;
}