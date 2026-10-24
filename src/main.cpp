#include "ProcessorsTrips/ProcessorData.hpp"
#include "Cli/CommandRegister.hpp"
#include "Network/CurlGlobal.hpp"
#include "Config/Config.hpp"
#include <spdlog/spdlog.h>
#include <CLI/CLI.hpp>

int main(int argc, char **argv){
    CLI::App app{"EngineCabsNYC: Analysis engine for intelligent decision based on hard data"};
    netGlobal::CurlGlobal curlGlobal;

    try{
        Config config("dataset_config.yaml");
        ProcessorData data;

        CommandRegister registry(config, data);
        registry.logCommands(app);

        CLI11_PARSE(app, argc, argv);
    } catch(const CLI::ParseError& e){
        return app.exit(e);
    }

    if(argc == 1) spdlog::info(app.help());
    return 0;
}