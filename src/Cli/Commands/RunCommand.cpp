#include "ProcessorsTrips/ProcessorRegister.hpp"
#include "Cli/Commands/RunCommand.hpp"

RunCommand::RunCommand(ProcessorData& data_)
    : data(data_)
{}

void RunCommand::execute(){
    RowStats rowStats;
    RevenueDayStats RevenueDStats;
    TripsHourStats tripsHStats;
    InvalidTripStats invalidTStats;
    PickupZoneStats pickupZStats;
    FareDistributionStats fareDisStats; 
    ProcessorRegister processR;

    processR.registerProcessors(rowStats,
        RevenueDStats,
        tripsHStats,
        invalidTStats,
        pickupZStats,
        fareDisStats
    );
    processR.searchProcessor(data);
}

void RunCommand::registerCommand(CLI::App& app){
    auto* cmd = app.add_subcommand("run", "Execute processors over the files");

    cmd->add_option(
        "--files",
        data.filesToProcess,
        "Files that will be processed"
    )->delimiter(',');

    cmd->add_option(
        "--processors",
        data.processors,
        "Processors that will run the files"
    )->delimiter(',');

    cmd->add_option(
        "--batch",
        data.batchSize,
        "Set the batch size of the processors"
    );

    cmd->add_option(
        "--output",
        data.output,
        "Where the stats will be saved"
    );

    cmd->require_option(4);

    cmd->callback([this]{
        execute();
    });
}