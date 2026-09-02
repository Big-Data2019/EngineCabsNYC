#include <Cli/Commands/RunCommand.hpp>
#include <iostream>

void RunCommand::execute(){
}

void RunCommand::registerCommand(CLI::App& app){
    auto* cmd = app.add_subcommand("Run", "Execute processors over the files");

    cmd->add_option(
        "--files",
        filesToProcess,
        "Files that will be processed"
    )->required();

    cmd->callback([this]{
        execute();
    });
}