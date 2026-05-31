#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>
#include <CLI/CLI.hpp>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

int main(){
    const std::string config_path = "dataset_config.yaml";

    try{
        YAML::Node config = YAML::LoadFile(config_path);

        if(!config["dataset"] || config["dataset"].as<std::string>() != "yellow_taxi"){
            spdlog::error("Invalid dataset specified in config file.");
            return 1;
        }

        auto years = config["years"].as<std::vector<int>>();
        auto months = config["months"].as<std::vector<std::string>>();

        fs::create_directories("downloads");

        spdlog::info("Initializing dataset downloader for dataset: {}", config["dataset"].as<std::string>());

        for(int year : years){
            for(const std::string& month : months){
                std::string file_name = fmt::format("yellow_tripdata_{}-{}.parquet", year, month);
                std::string url = fmt::format("https://cloudfront.net{}", file_name); 
                std::string local_path = fmt::format("downloads/{}", file_name);

                spdlog::info("Processing file: {}", file_name);
                spdlog::info("   -> URL origin: {}", url);
                spdlog::info("   -> Local path: {}", local_path);
            }
        }

        // if(config["dataset"]){
        //     std::string dataset_name = config["dataset"].as<std::string>();
        //     spdlog::info("Dataset specified in config: {}", dataset_name);
        // } else{
        //     spdlog::warn("No dataset specified in config file.");
        //     return 1;
        // }

        // if(config["years"] && config["years"].IsSequence()){ 
        //     auto years = config["years"].as<std::vector<int>>();
        //     spdlog::info("Years specified in config:");

        //     for(int year : years){
        //         fmt::print("- {}\n", year);
        //     }
        // }

        // if(config["months"]){
        //     const auto months = config["months"].as<std::vector<std::string>>();
        //     spdlog::info("Months specified in config:");

        //     for(const std::string& month : months){
        //         fmt::print("- {}\n", month);
        //     }
        // }

    } catch(const YAML::BadFile & e){
        spdlog::error("Failed to load config file: {}", config_path);
        return 1;
    } catch(const YAML::Exception & e){
        spdlog::error("Error loading YAML file: {}", e.what());
        return 1;
    }

    return 0;
}