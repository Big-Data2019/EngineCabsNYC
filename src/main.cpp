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
#include <re2/re2.h>

namespace fs = std::filesystem;

// Struct with the data of the yaml
struct configYaml{
    std::string datasetName;
    std::vector<int> years;
    std::vector<int> months;
    std::string outputDirectory;

    // Constructor to get the data from the YAML and put it into the variables
    configYaml(const YAML::Node& configFile){
        datasetName = configFile["dataset"].as<std::string>();
        years = configFile["years"].as<std::vector<int>>();
        months = configFile["months"].as<std::vector<int>>();
        outputDirectory = configFile["output_dir"].as<std::string>();
    }
};

// Function to read the data of YAML configuration
static void processDataYaml(const configYaml& config){
    for(const auto& year : config.years){
        spdlog::info("Years to process: {}", year);

        for(const auto& month : config.months){
            spdlog::info("Months to process: {}", month);
        }
    }
}

int main(int argc, char* argv[]){
    const std::string configPath = "dataset_config.yaml";

    if(!fs::exists(configPath)){
        spdlog::error("The configuration file: {} was not found", configPath);
        spdlog::info("Make sure the configuration file is in the same directory");
        return 1;
    }

    try{
        YAML::Node configFile = YAML::LoadFile(configPath);
        CLI::App app{"EngineCabsNYC: Program to download taxi datasets in New York"};

        configYaml config(configFile);

        app.add_option("-n, --dataset", config.datasetName, "Change dataset name");
        app.add_option("-y, --years", config.years, "Change years to process");
        app.add_option("-m, --months", config.months, "Change months to process");
        app.add_option("-d, --directory", config.outputDirectory, "Change the directory file");

        CLI11_PARSE(app, argc, argv)

        fs::path currentWorkingDir = fs::current_path();

        fs::path routeDownload = currentWorkingDir / config.outputDirectory;

        if(fs::create_directories(routeDownload)){
            spdlog::info("The directory to download datasets was created");
        }

        spdlog::info("Dataset to process: {}\n", config.datasetName);

        processDataYaml(config);

        const std::string url = "https://d37ci6vzurychx.cloudfront.net/trip-data";

        std::string carType;

        RE2::Options options;
        options.set_case_sensitive(false);

        re2::RE2 patron("(fhvhv|fhv|yellow|green)", options);

        if(RE2::PartialMatch(config.datasetName, patron, &carType)){
            spdlog::info("Type of dataset to download: {}", carType);

            for(char& c : carType){
                c = std::tolower(static_cast<unsigned char>(c));
            }
        }

        for(const auto& year : config.years){
            for(const auto &month : config.months){
            std::string url2 = fmt::format("{}/{}_tripdata_{}-{}", url, carType, year, month);

            spdlog::info("url1: {}", url2);
            }
        }

    } catch(const YAML::Exception& e){
        spdlog::error("Error Yaml: {}", e.what());
        return 1;
    } catch(const fs::filesystem_error& e){
        spdlog::error("Error system file: {}", e.what());
        return 1;
    }

    return 0;
}