#include "Cli/Commands/InspectCommand.hpp"
#include "Parquet/Inspect.hpp"
#include <spdlog/spdlog.h>

void InspectCommand::execute(){
    auto status = inspectSchema(file);
    if(!status.ok()){
        spdlog::error("Error of the status: {}", status.ToString());
        return;
    }
}

void InspectCommand::registerCommand(CLI::App& app){
    auto* cmd = app.add_subcommand("inspect", "Show number of columns, rows, and the schema");
    cmd->add_option("path", file, "Path to input parquet file")
        ->required()
        ->check(CLI::ExistingFile);

    cmd->callback([this]{
        execute();
    });
}