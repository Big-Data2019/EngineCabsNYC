#pragma once
#include "Cli/ICommand.hpp"
#include <vector>
#include <memory>
#include <utility>

struct configYaml;
struct configGlobal;

class CommandRegister{
private:
    std::vector<std::unique_ptr<ICommand>> commandList;

    template<typename T,typename...Args>
    void registerCommand(Args&&...args){
        commandList.push_back(
            std::make_unique<T>(std::forward<Args>(args)...)
        );
    }

public:
    CommandRegister(const configYaml& dataYAML, const configGlobal& configGl);
    void logCommands(CLI::App& app);
};