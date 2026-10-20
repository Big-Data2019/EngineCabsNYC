#pragma once

#include "Cli/ICommand.hpp"
#include <string>

class ValidateCommand final : public ICommand{
private:
    std::string file;
    void execute() override;

public:
    void registerCommand(CLI::App& app) override;
};