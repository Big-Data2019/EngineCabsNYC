#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>
#include <CLI/CLI.hpp>
#include <string>
#include <vector>

int main(){
    try{
        YAML::Node config = YAML::LoadFile("dataset_config.yaml");

        if(config["dataset"]){
            std::string dataset_name = config["dataset"].as<std::string>();
            spdlog::info("Dataset specified in config: {}", dataset_name);
        } else{
            spdlog::warn("No dataset specified in config file.");
            return 1;
        }

        if(config["years"] && config["years"].IsSequence()){ 
            auto years = config["years"].as<std::vector<int>>();
            spdlog::info("Years specified in config:");

            for(int year : years){
                fmt::print("- {}\n", year);
            }
        }

        if(config["months"]){
            const auto months = config["months"].as<std::vector<std::string>>();
            spdlog::info("Months specified in config:");

            for(const std::string& month : months){
                fmt::print("- {}\n", month);
            }
        }

    } catch(const YAML::BadFile & e){
        spdlog::error("Failed to load config file: {}", "dataset_config.yaml");
        return 1;
    } catch(const YAML::Exception & e){
        spdlog::error("Error loading YAML file: {}", e.what());
        return 1;
    }

    return 0;
}