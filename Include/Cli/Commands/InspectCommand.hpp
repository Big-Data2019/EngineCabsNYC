#pragma once
#include "Cli/ICommand.hpp"
#include <string>

class InspectCommand final : public ICommand{
private:
    std::string path;
    void execute();

public:
    void registerCommand(CLI::App& app) override;
};