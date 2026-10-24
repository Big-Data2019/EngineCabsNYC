#include "Parquet/ParquetReader.hpp"
#include "Cli/Commands/Inspect.hpp"
#include <spdlog/spdlog.h>

void InspectCommand::execute(){
    auto result = openParquetReader(file);
    if(!result.ok()){
        spdlog::error("Error of the status: {}", result.status().ToString());
        return;
    }

    const auto metadata = result->reader->parquet_reader()->metadata();

    spdlog::info("File: {}", file);
    spdlog::info("Num Columns: {}", metadata->num_columns());
    spdlog::info("Num Rows: {}", metadata->num_rows());
    spdlog::info("Num Row Groups: {}", metadata->num_row_groups());
    spdlog::info("Schema: \n{}", result->schema->ToString());
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