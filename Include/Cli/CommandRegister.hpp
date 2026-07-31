#pragma once
#include "Cli/ICommand.hpp"
#include "Config/Config.hpp"
#include <vector>
#include <memory>
#include <utility>

class CommandRegister final{
private:
    std::vector<std::unique_ptr<ICommand>> commandList;

    template<typename T,typename...Args>
    void registerCommand(Args&&...args){
        commandList.push_back(
            std::make_unique<T>(std::forward<Args>(args)...)
        );
    }

public:
    CommandRegister(const Config& config);
    void logCommands(CLI::App& app);
};