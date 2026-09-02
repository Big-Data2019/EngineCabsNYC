#pragma once
#include "Cli/ICommand.hpp"
#include <string>
#include <vector>
#include <filesystem>

class RunCommand final : public ICommand{
private:
    int64_t batchSize;
    std::vector<std::string> processors;
    std::vector<std::string> filesToProcess;
    std::filesystem::path output;

    void execute() override;

public:
    void registerCommand(CLI::App& app) override;
};