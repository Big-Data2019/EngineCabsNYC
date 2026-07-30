#include "Cli/CommandRegister.hpp"
#include "Cli/Commands/DownloadCommand.hpp"
#include "Cli/Commands/InspectCommand.hpp"

CommandRegister::CommandRegister(const configYaml& dataYaml, const configGlobal& configGl){
    commandList.push_back(std::make_unique<InspectCommand>());
    commandList.push_back(std::make_unique<DownloadCommand>(dataYaml, configGl));
}

void CommandRegister::registerCommands(CLI::App& app){
    for(auto& command : commandList){
        command->registerCommand(app);
    }
}