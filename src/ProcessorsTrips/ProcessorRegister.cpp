#include "ProcessorsTrips/ProcessorRegister.hpp"
#include "Utils/ExpanderFiles.hpp"
#include <spdlog/spdlog.h>

void ProcessorRegister::registerProcessors(
    RunStats& rowCounterStats,
    RunStats& revenueDayStats,
    RunStats& tripsHourStats,
    RunStats& invalidTripStats,
    RunStats& pickupZoneStats,
    RunStats& fareDisStats,
    FareDistributionResult& fareDisResult
){
    processors["row"] = std::make_unique<RowCounter>(rowCounterStats);
    processors["revenueD"] = std::make_unique<RevenueByDay>(revenueDayStats);
    processors["tripsH"] = std::make_unique<TripsByHour>(tripsHourStats);
    processors["invalid"] = std::make_unique<InvalidTripDetector>(invalidTripStats);
    processors["pickupZ"] = std::make_unique<TopPickupZones>(pickupZoneStats);
    processors["fareDis"] = std::make_unique<FareDistribution>(fareDisResult, fareDisStats);
}

void ProcessorRegister::searchProcessor(const ProcessorData& data){
    auto files = expandFiles(data.filesToProcess);

    for(const auto& processor : data.processors){
        auto it = processors.find(processor);
        if(it == processors.end()) continue;

        for(const auto& file : files){
            auto status = it->second->process(data.batchSize, file);

            if(!status.ok()){
                spdlog::error("Processor: {}, failed: {}", processor, status.ToString());
            }
        }

        it->second->finish(data.output);
    }
}