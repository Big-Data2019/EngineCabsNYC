#pragma once

#include "Config/Config.hpp"
#include "Cli/ICommand.hpp"

class DownloadCommand final : public ICommand{
private:
    const Config& config;
    void execute() override;

public:
    DownloadCommand(const Config& config_);
    void registerCommand(CLI::App& app) override;
};