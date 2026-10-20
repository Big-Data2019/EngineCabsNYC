#include "Manifest/Json/ParquetDownload.hpp"
#include "Filesystem/CreateOutputDir.hpp"
#include "Cli/Commands/Download.hpp"
#include "Config/DatasetType.hpp"
#include <spdlog/spdlog.h>

DownloadCommand::DownloadCommand(const Config& config_)
    : config(config_)
{}

void DownloadCommand::execute(){
    spdlog::info("Type of dataset: {}", getTypeDataset(config.datasetName()));

    if(!createOutputDir(config.outputDir())){
        spdlog::error("The output file {} could not be secured", config.outputDir());
        return;
    }

    Manifest::generateDownloadManifest(config);
}

void DownloadCommand::registerCommand(CLI::App& app){
    auto* cmd = app.add_subcommand("download", "Download the .parquet files");

    cmd->callback([this]{
        execute();
    });
}