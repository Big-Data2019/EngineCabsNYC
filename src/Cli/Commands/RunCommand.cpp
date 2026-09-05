#include <Cli/Commands/RunCommand.hpp>
#include <iostream>

void RunCommand::execute(){

}

void RunCommand::registerCommand(CLI::App& app){
    auto* cmd = app.add_subcommand("run", "Execute processors over the files");

    cmd->add_option(
        "--files",
        filesToProcess,
        "Files that will be processed"
    );

    cmd->add_option(
        "--processors",
        processors,
        "Processors that will run the files"
    );

    cmd->add_option(
        "--batch",
        batchSize,
        "Set the batch size of the processors"
    );

    cmd->add_option(
        "--output",
        output,
        "Where the stats will be saved"
    );

    cmd->require_option(4);

    cmd->callback([this]{
        execute();
    });
}