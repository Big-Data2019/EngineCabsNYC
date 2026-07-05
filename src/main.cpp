#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>
#include <CLI/CLI.hpp>
#include <string>
#include <vector>
#include <filesystem>
#include <re2/re2.h>
#include <memory>
#include <cstdlib>
#include <arrow/io/file.h>
#include <parquet/arrow/reader.h>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <fstream>

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

// Function to show the data of YAML configuration
static void showDataYaml(const configYaml& config){
    spdlog::info("Dataset to process: {}\n", config.datasetName);

    for(const auto& year : config.years){
        spdlog::info("Years to process: {}", year);

        for(const auto& month : config.months){
            spdlog::info("Months to process: {:02}", month);
        }
    }
}

// Function to transform text into lowercase
static std::string toLowercase(std::string&& text){
    std::transform(text.begin(), text.end(), text.begin(),
        [](unsigned char c){ 
            return std::tolower(c);
    });

    // Receive and send the data
    return std::move(text);
}

// Function to search the type of dataset in the datasetName with optional for exceptions
static std::optional<std::string> getTypeDataset(const configYaml& config){
    std::string carType;

    // Ignore uppercase and lowercase letters
    RE2::Options options;
    options.set_case_sensitive(false);

    static const re2::RE2 patron("(fhvhv|fhv|yellow|green)", options);

    // Search for one of the previously stated words
    if(RE2::PartialMatch(config.datasetName, patron, &carType)){
        spdlog::info("Type of dataset to download: {}\n", carType);

        // Send and receive the carType at function toLowercase
        return toLowercase(std::move(carType));
    } 

    return std::nullopt;
}

// Function to create the final URL where the datasets are downloaded
static void generateUrlDownload(const configYaml& config){
    const std::string initialUrl = "https://d37ci6vzurychx.cloudfront.net/trip-data";
    auto carTypeOpt = getTypeDataset(config);

    if(!carTypeOpt.has_value()){
        spdlog::error("The type of car is not specified in the name");
        return;
    }

    std::string carType = *carTypeOpt;
    spdlog::info("Links to where the datasets will be downloaded: ");

    for(const auto& year : config.years){
        for(const auto& month : config.months){
            std::string endUrl = fmt::format("{}/{}_tripdata_{}-{:02}.parquet", initialUrl, carType, year, month);

            spdlog::info(endUrl);
        }
    }
}

/*
Function to return true if the directory exists and is ready to use, 
false if an error occurs or there is a file obstructing the path
 */
static bool ensureOutputDirectory(const configYaml& config){
    std::filesystem::path currentWorkingDir = std::filesystem::current_path();
    std::filesystem::path routeDownload = currentWorkingDir / config.outputDirectory;

    std::error_code errorCode;
    std::filesystem::create_directories(routeDownload, errorCode);

    if(std::filesystem::is_directory(routeDownload) && !errorCode){
        return true;
    }
    
    spdlog::error("Folder failed to load because: {}", errorCode.message());
    return false;
}

int main(int argc, char* argv[]){
    std::string configPath = "dataset_config.yaml";

    if(!std::filesystem::exists(configPath)){
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

        if(!ensureOutputDirectory(config)){
            return 1;
        }

        showDataYaml(config);

        generateUrlDownload(config);

    } catch(const YAML::Exception& e){
        spdlog::error("Error Yaml: {}", e.what());
        return 1;
    } catch(const std::filesystem::filesystem_error& e){
        spdlog::error("Error system file: {}", e.what());
        return 1;
    } 

    return 0;
}