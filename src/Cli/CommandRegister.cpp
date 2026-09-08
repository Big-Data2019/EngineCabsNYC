#include "Cli/CommandRegister.hpp"
#include "Cli/Commands/DownloadCommand.hpp"
#include "Cli/Commands/InspectCommand.hpp"
#include "Cli/Commands/RunCommand.hpp"

CommandRegister::CommandRegister(const Config& config, ProcessorData& data){
    registerCommand<InspectCommand>();
    registerCommand<DownloadCommand>(config);
    registerCommand<RunCommand>(data);
}

void CommandRegister::logCommands(CLI::App& app){
    for(const auto& command : commandList){
        command->registerCommand(app);
    }
}