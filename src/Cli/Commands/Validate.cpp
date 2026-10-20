#include "Cli/Commands/Validate.hpp"

void ValidateCommand::execute(){

}

void ValidateCommand::registerCommand(CLI::App& app){
    auto* cmd = app.add_subcommand("validate", "Verify the integrity of the data");
    cmd->add_option("path", file, "Path to inputh the parquet files")
        ->required()
        ->check(CLI::ExistingPath);

    cmd->callback([this]{
        execute();
    });
}