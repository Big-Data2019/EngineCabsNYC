#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <CLI/CLI.hpp>
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <fstream>
#include <chrono>
#include <ctime>
#include <arrow/api.h>
#include <arrow/io/file.h>
#include <parquet/arrow/reader.h>
#include <utility>
#include <locale>
#include "Network/CurlGlobal.hpp"
#include "Config/ConfigGlobal.hpp"
#include "Config/ConfigYaml.hpp"
#include "Utils/MeasureTime.hpp"
#include "Filesystem/EnsureDirectory.hpp"
#include "Download/parquetFiles.hpp"
#include "Parquet/Inspect.hpp"
#include "Manifest/BuildJson.hpp"
#include "Manifest/GenerateJson.hpp"

template <typename T>
[[nodiscard("Error: You need to put the function in an if statement")]]
static bool hasValue(const T& element, std::string_view message){
    if(!element){
        spdlog::error(message);
        return false;
    }

    return true;
}

template <typename T>
[[nodiscard("Error: You need to put the function in an if statement")]]
static bool isOpen(const T& element, std::string_view message){
    if(!element.is_open()){                                                      
        spdlog::error(message);
        return false;
    }

    return true;
}

template <typename T>
[[nodiscard("Error: You need to put the function in an if statement")]]
static bool isOk(const T& element, std::string_view message){
    if(!element.ok()){
        spdlog::error(message);
        return false;
    }

    return true;
}

template <typename InternalFunction>
[[nodiscard("ERROR: You need to put some action in the function")]]
//Function that creates two nested for loops with an internal action
static void iteration(const configYaml& dataYaml, InternalFunction action){
    for(auto& year : dataYaml.years){
        for(auto& month : dataYaml.months){
            action(year, month);
        }
    }
}

// static bool existsFile(const std::string& file){
//     if(std::filesystem::exists(file)){
//         if(std::filesystem::file_size(file) > 0){
//             spdlog::info("File already exists locally, skipping download");
//             return true;
//         } else{
//             spdlog::warn("File {} is corrupted, re-downloading", file);
//             std::filesystem::remove(file);
//             return false;
//         }
//     } else{
//         return false;
//     }
// }

// static bool readyToProcess(const std::string& outputPath, const std::string& url, bool exists){
//     if(!exists){
//         if(!downloadFiles(url, outputPath)){
//             spdlog::error("The download failed for the period");
//             return false;
//         }
//     }

//     std::uintmax_t bytes = std::filesystem::file_size(outputPath);
//     double megabytes = bytes / (1024.0 * 1024.0);
//     spdlog::info("File size: {:.2f} Mb\n", megabytes);
//     return true;
// }

// static bool verifyDownload(const configYaml& dataYAML, const configGlobal& configGl){
//     bool ready = true;

//     iteration(dataYAML, [&](int year, int month){
//         std::string outputPath = configGl.getOutputPath(year, month);
//         std::string url = configGl.getDownloadUrl(year, month);
//         bool exists = existsFile(outputPath);

//         spdlog::info("link: {}", url);
//         ready = (!readyToProcess(outputPath, url, exists)) ? false : true;
//     });

//     return (!ready) ? false : true;
// }

// nlohmann::ordered_json buildManifestJson(const configYaml& dataYAML, const configGlobal& configGl){
//     nlohmann::ordered_json dataJson;
//     auto [time, download] = timeLapse(verifyDownload, dataYAML, configGl);
//     dataJson["dataset"] = dataYAML.datasetName;
//     dataJson["files"] = nlohmann::ordered_json::array();
//     dataJson["download_time"] = std::to_string(time) + "s";

//     iteration(dataYAML, [&](int year, int month){
//         dataJson["files"].push_back({
//             {"year", year},
//             {"month", month},
//             {"path", configGl.getOutputPath(year, month)},
//             {"source", "nyc_tlc"},
//             {"status", download ? "Downloaded" : "Failed"}
//         });
//     });

//     return dataJson;
// }

// static std::string generateTimestamp(const std::string& name){
//     auto now = std::chrono::system_clock::now();
//     time_t timeActual = std::chrono::system_clock::to_time_t(now);
//     // Divide it into a format of year, month, day, etc
//     tm* timePart = localtime(&timeActual);

//     std::stringstream ss;
//     ss<<std::put_time(timePart, "%Y%m%d%H%M%S");

//     std::string timestamp = ss.str() + name;
//     return timestamp;
// }

// static bool generateJsonFile(const configYaml& dataYAML, const configGlobal& configGl){
//     try{
//         const std::string manifest = "-manifest.json";
//         std::string fileName = generateTimestamp(manifest);

//         std::ofstream file(fileName);
//         if(!isOpen(file, "Failed to open or create local output file")) return false;

//         auto jsonData = buildManifestJson(dataYAML, configGl);
//         file<<std::setw(4)<<jsonData;
//         return file.good();
//     } catch(const std::exception& e){
//         spdlog::error("Error: {}", e.what());
//         return false;
//     }
// }

class ICommand{
public:
    virtual ~ICommand() = default;
    virtual void registerCommand(CLI::App& app) = 0;
};

class DownloadCommand : public ICommand{
private:
    const configYaml& dataYAML;
    const configGlobal& configGl;

    void execute(){
        spdlog::info("Type of dataset: {}\n", configGl.carType);

        if(!hasValue(ensureOutputDir(dataYAML.outputDir), "The output file could not be secured")){
            return;
        }

        spdlog::info("Manifest: {}", generateJsonFile(dataYAML, configGl)
            ? "created" : "could not be created");
    }

public:
    DownloadCommand(const configYaml& dataYAML_, const configGlobal& configGl_)
        : dataYAML(dataYAML_), configGl(configGl_)
    {}

    void registerCommand(CLI::App& app) override{
        auto* cmd = app.add_subcommand("download", "Download the .parquet files from the yaml");

        cmd->callback([this]{
            this->execute();
        });
    }
};

class InspectCommand : public ICommand{
private:
    std::string path;

    void execute(){
        auto status = inspectSchema(path);
        if(!isOk(status, "Error of the status")) return;
    }

public:
    void registerCommand(CLI::App& app) override{
        auto* cmd = app.add_subcommand("inspect", "show number of columns, rows, and the schema");
        cmd->add_option("path", path, "Path to input Parquet file for inspect")
            ->required()
            ->check(CLI::ExistingFile);
        
        cmd->callback([this]{
            execute();
        });
    }
};

class CommandHandler{
private:
    std::vector<std::unique_ptr<ICommand>> commandList;

public:
    CommandHandler(CLI::App& app, const configYaml& dataYAML, const configGlobal& configGl){
        commandList.push_back(std::make_unique<InspectCommand>());
        commandList.push_back(std::make_unique<DownloadCommand>(dataYAML, configGl));
    }

    void registerCommands(CLI::App& app){
        for(auto& command : commandList){
            command->registerCommand(app);
        }
    }
};

int main(int argc, char **argv){
    CLI::App app{"EngineCabsNYC: Analysis engine for intelligent decision based on hard data"};
    std::locale::global(std::locale("en_US.UTF-8"));
    netGlobal::CurlGlobal curlGlobal;

    try{
        YAML::Node configFile = YAML::LoadFile("dataset_config.yaml");
        configYaml dataYAML(configFile);
        configGlobal configGl(dataYAML);

        CommandHandler handler(app, dataYAML, configGl);
        handler.registerCommands(app);

        CLI11_PARSE(app, argc, argv);
    } catch(const YAML::ParserException& e){
        spdlog::error("Error YAML: {}", e.what());
        return 1;
    } catch(const CLI::ParseError& e){
        return app.exit(e);
    }

    if(argc == 1){
        spdlog::info(app.help());
        return 0;
    }

    return 0;
}