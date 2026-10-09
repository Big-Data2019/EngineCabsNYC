#include "ProcessorsTrips/ProcessorRegister.hpp"
#include <spdlog/spdlog.h>

void ProcessorRegister::registerProcessors(RowStats& rowStats,
    RevenueDayStats& revenueDStats,
    TripsHourStats& tripsHStats,
    InvalidTripStats& invalidTStats
){
    processors["row"] = std::make_unique<RowCounter>(rowStats);
    processors["revenueD"] = std::make_unique<RevenueByDay>(revenueDStats);
    processors["tripsH"] = std::make_unique<TripsByHour>(tripsHStats);
    processors["invalid"] = std::make_unique<InvalidTripDetector>(invalidTStats);
}

void ProcessorRegister::searchProcessor(const ProcessorData& data){
    for(const auto& processor : data.processors){
        auto it = processors.find(processor);

        if(it != processors.end()){
            auto status = it->second->process(data.batchSize, data.filesToProcess);
            if(!status.ok()){
                spdlog::error("Processor: {}, failed: {}", processor, status.ToString());
            }

            it->second->finish(data.output);
        }
    }
}