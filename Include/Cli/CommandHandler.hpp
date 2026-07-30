#pragma once
#include "Cli/ICommand.hpp"
#include <vector>
#include <memory>

struct configYaml;
struct configGlobal;

class CommandHandler{
private:
    std::vector<std::unique_ptr<ICommand>> commandList;

public:
    CommandHandler(const configYaml& dataYAML, const configGlobal& configGl);

    void registerCommands(CLI::App& app);
};