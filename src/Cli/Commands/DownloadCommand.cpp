#include "Cli/Commands/DownloadCommand.hpp"
#include "Filesystem/EnsureDirectory.hpp"
#include "Manifest/GenerateJson.hpp"
#include <spdlog/spdlog.h>

DownloadCommand::DownloadCommand(const configYaml& dataYaml_, const configGlobal& configGl_)
    : dataYAML(dataYaml_), configGl(configGl_)
{}

void DownloadCommand::execute(){
    spdlog::info("Type of dataset: {}", configGl.carType);

    if(!ensureOutputDir(dataYAML.outputDir)){
        spdlog::error("The output file could not be secured");
        return;
    }

    spdlog::info("Manifest: {}", generateJsonFile(dataYAML, configGl)
        ? "created" : "could not be created");
}

void DownloadCommand::registerCommand(CLI::App& app){
    auto* cmd = app.add_subcommand("download", "Download the .parquet files");

    cmd->callback([this]{
        execute();
    });
}