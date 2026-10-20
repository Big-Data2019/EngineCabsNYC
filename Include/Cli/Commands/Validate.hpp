#pragma once

#include "Cli/ICommand.hpp"
#include <string>
#include <vector>

class ValidateCommand final : public ICommand{
private:
    std::vector<std::string> paths;
    void execute() override;

public:
    void registerCommand(CLI::App& app) override;
};