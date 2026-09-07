#pragma once

#include "Cli/ICommand.hpp"
#include "Config/Config.hpp"

class DownloadCommand final : public ICommand{
private:
    const Config& config;
    void execute() override;

public:
    DownloadCommand(const Config& config_);
    void registerCommand(CLI::App& app) override;
};