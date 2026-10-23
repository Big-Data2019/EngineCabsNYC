#include "ProcessorsTrips/ProcessorRegister.hpp"
#include "Utils/ExpanderFiles.hpp"
#include <spdlog/spdlog.h>

void ProcessorRegister::registerProcessors(RowStats& rowStats,
    RevenueDayStats& revenueDStats,
    TripsHourStats& tripsHStats,
    InvalidTripStats& invalidTStats,
    PickupZoneStats& pickupZStats,
    FareDistributionStats& fareDisStats
){
    processors["row"] = std::make_unique<RowCounter>(rowStats);
    processors["revenueD"] = std::make_unique<RevenueByDay>(revenueDStats);
    processors["tripsH"] = std::make_unique<TripsByHour>(tripsHStats);
    processors["invalid"] = std::make_unique<InvalidTripDetector>(invalidTStats);
    processors["pickupZ"] = std::make_unique<TopPickupZones>(pickupZStats);
    processors["fareDis"] = std::make_unique<FareDistribution>(fareDisStats);
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