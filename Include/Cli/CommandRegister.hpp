#pragma once

#include "ProcessorsTrips/ProcessorData.hpp"
#include "Config/Config.hpp"
#include "Cli/ICommand.hpp"
#include <utility>
#include <vector>
#include <memory>

class CommandRegister final{
private:
    std::vector<std::unique_ptr<ICommand>> commandList;

    template<typename T, typename...Args>
    void registerCommand(Args&&...args){
        commandList.push_back(
            std::make_unique<T>(std::forward<Args>(args)...)
        );
    }

public:
    CommandRegister(const Config& config, ProcessorData& data);
    void logCommands(CLI::App& app);
};