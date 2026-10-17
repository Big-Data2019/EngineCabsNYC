#pragma once

#include "ProcessorsTrips/ProcessorData.hpp"
#include "Cli/ICommand.hpp"

class RunCommand final : public ICommand{
private:
    ProcessorData& data;
    void execute() override;

public:
    RunCommand(ProcessorData& data_);
    void registerCommand(CLI::App& app) override;
};