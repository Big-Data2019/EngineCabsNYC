#include "Cli/CommandRegister.hpp"
#include "Cli/Commands/DownloadCommand.hpp"
#include "Cli/Commands/InspectCommand.hpp"

CommandRegister::CommandRegister(const configYaml& dataYaml, const configGlobal& configGl){
    registerCommand<InspectCommand>();
    registerCommand<DownloadCommand>(dataYaml, configGl);
}

void CommandRegister::logCommands(CLI::App& app){
    for(const auto& command : commandList){
        command->registerCommand(app);
    }
}