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

    cmd->add_option(
        "--processors",
        processors,
        "Processors that will run the files"
    )->required();

    cmd->add_option(
        "--batch",
        batchSize,
        "Set the batch size of the processors"
    )->required();

    cmd->callback([this]{
        execute();
    });
}