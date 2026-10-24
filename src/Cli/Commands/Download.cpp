#include "Manifest/Json/ParquetDownload.hpp"
#include "Filesystem/CreateOutputDir.hpp"
#include "Download/VerifyDownload.hpp"
#include "Filesystem/ExistsFile.hpp"
#include "Cli/Commands/Download.hpp"
#include "Config/DatasetType.hpp"
#include "Utils/MeasureTime.hpp"
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

    // Assign the time and the bool download successfully
    auto[time, successfulDownload] = timeLapse(verifyDownload, config);

    generateDownloadManifest(config, successfulDownload, time);
}

void DownloadCommand::registerCommand(CLI::App& app){
    auto* cmd = app.add_subcommand("download", "Download the .parquet files");

    cmd->callback([this]{
        execute();
    });
}