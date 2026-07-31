#include "Cli/CommandRegister.hpp"
#include "Cli/Commands/DownloadCommand.hpp"
#include "Cli/Commands/InspectCommand.hpp"

CommandRegister::CommandRegister(const Config& config){
    registerCommand<InspectCommand>();
    registerCommand<DownloadCommand>(config);
}

void CommandRegister::logCommands(CLI::App& app){
    for(const auto& command : commandList){
        command->registerCommand(app);
    }
}