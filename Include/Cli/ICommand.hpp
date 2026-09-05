#pragma once
#include <CLI/CLI.hpp>

struct ProcessorData{
    int64_t batchSize;
    std::vector<std::string> processors;
    std::vector<std::string> filesToProcess;
    std::filesystem::path output;
};

class ICommand{
public:
    virtual ~ICommand() = default;
    virtual void registerCommand(CLI::App& app) = 0;
    virtual void execute() = 0;
};