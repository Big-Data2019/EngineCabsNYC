#include "Network/CurlGlobal.hpp"
#include "Config/Config.hpp"
#include "Cli/CommandRegister.hpp"
#include <spdlog/spdlog.h>
#include <CLI/CLI.hpp>
#include <locale>

int main(int argc, char **argv){
    CLI::App app{"EngineCabsNYC: Analysis engine for intelligent decision based on hard data"};
    std::locale::global(std::locale("en_US.UTF-8"));
    netGlobal::CurlGlobal curlGlobal;

    try{
        Config config("dataset_config.yaml");
        
        CommandRegister registry(config);
        registry.logCommands(app);

        CLI11_PARSE(app, argc, argv);
    } catch(const CLI::ParseError& e){
        return app.exit(e);
    }

    if(argc == 1){
        spdlog::info(app.help());
        return 0;
    }

    return 0;
}