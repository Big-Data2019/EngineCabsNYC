#pragma once
#include "Cli/ICommand.hpp"
#include "Config/ConfigYaml.hpp"
#include "Config/ConfigGlobal.hpp"

class DownloadCommand final : public ICommand{
private:
    const configYaml& dataYAML;
    const configGlobal& configGl;
    void execute();

public:
    DownloadCommand(const configYaml& dataYAML_, const configGlobal& configGl_);
    void registerCommand(CLI::App& app) override;
};