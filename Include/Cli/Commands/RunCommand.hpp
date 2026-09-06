#pragma once
#include "Cli/ICommand.hpp"

class RunCommand final : public ICommand{
private:
    ProcessorData& data;
    void execute() override;

public:
    void registerCommand(CLI::App& app) override;
};