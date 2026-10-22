#include "Cli/Commands/Validate.hpp"
#include "Utils/ExpanderFiles.hpp"

void ValidateCommand::execute(){
    auto files = expandFiles(paths);
}

void ValidateCommand::registerCommand(CLI::App& app){
    auto* cmd = app.add_subcommand("validate", "Verify the integrity of the data");
    cmd->add_option("--path", paths, "Path to inputh the parquet files")
        ->required()
        ->check(CLI::ExistingPath);

    cmd->add_option("--output", output, "Where the stats will be saved");

    cmd->callback([this]{
        execute();
    });
}