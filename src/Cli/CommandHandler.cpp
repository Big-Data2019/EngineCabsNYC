#include "Cli/CommandHandler.hpp"
#include "Cli/Commands/DownloadCommand.hpp"
#include "Cli/Commands/InspectCommand.hpp"

CommandHandler::CommandHandler(const configYaml& dataYaml, const configGlobal& configGl){
    commandList.push_back(std::make_unique<InspectCommand>());
    commandList.push_back(std::make_unique<DownloadCommand>(dataYaml, configGl));
}

void CommandHandler::registerCommands(CLI::App& app){
    for(auto& command : commandList){
        command->registerCommand(app);
    }
}