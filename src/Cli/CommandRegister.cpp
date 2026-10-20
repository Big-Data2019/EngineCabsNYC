#include "Cli/Commands/Download.hpp"
#include "Cli/Commands/Inspect.hpp"
#include "Cli/CommandRegister.hpp"
#include "Cli/Commands/Run.hpp"

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