#include "Cli/Commands/InspectCommand.hpp"
#include "Parquet/Inspect.hpp"
#include <spdlog/spdlog.h>

void InspectCommand::execute(){
    auto status = inspectSchema(path);
    if(!status.ok()){
        spdlog::error("Error of the status: {}", status.ToString());
    }
}

void InspectCommand::registerCommand(CLI::App& app){
    auto* cmd = app.add_subcommand("inspect", "Show number of columns, rows, and schema");
    cmd->add_option("path", path, "Path to input parquet file")
        ->required()
        ->check(CLI::ExistingFile);

    cmd->callback([this]{
        execute();
    });
}