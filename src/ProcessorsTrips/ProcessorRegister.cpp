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
    processors["row-counter"] = std::make_unique<RowCounter>(rowCounterStats);
    processors["revenue-by-day"] = std::make_unique<RevenueByDay>(revenueDayStats);
    processors["trips-by-hour"] = std::make_unique<TripsByHour>(tripsHourStats);
    processors["invalid-trip-detector"] = std::make_unique<InvalidTripDetector>(invalidTripStats);
    processors["top-pickup-zones"] = std::make_unique<TopPickupZones>(pickupZoneStats);
    processors["fare-dis"] = std::make_unique<FareDistribution>(fareDisResult, fareDisStats);
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