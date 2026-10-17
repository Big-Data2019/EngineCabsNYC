#include "Cli/Commands/InspectCommand.hpp"
#include "Parquet/PrintSchemaInfo.hpp"
#include "Parquet/InspectSchema.hpp"
#include <spdlog/spdlog.h>

void InspectCommand::execute(){
    auto result = inspectParquetSchema(file);
    if(!result.ok()){
        spdlog::error("Error of the status: {}", result.status().ToString());
        return; 
    }

    printSchemaInfo(*result);
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