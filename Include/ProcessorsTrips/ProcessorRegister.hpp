#pragma once

#include "ProcessorsTrips/Processors/InvalidTripDetector.hpp"
#include "ProcessorsTrips/Processors/FareDistribution.hpp"
#include "ProcessorsTrips/Processors/TopPickupZones.hpp"
#include "ProcessorsTrips/Processors/RevenueByDay.hpp"
#include "ProcessorsTrips/Processors/TripsByHour.hpp"
#include "ProcessorsTrips/Processors/RowCounter.hpp"
#include "ProcessorsTrips/TripBatchProcessor.hpp"
#include "ProcessorsTrips/ProcessorData.hpp"
#include <unordered_map>
#include <string>
#include <memory>

class ProcessorRegister final{
private:
    std::unordered_map<std::string, std::unique_ptr<TripBatchProcessor>> processors;

public:
    void registerProcessors(
        RunStats& rowCounterStats,
        RunStats& revenueDayStats,
        RunStats& tripsHourStats,
        RunStats& invalidTripStats,
        RunStats& pickupZoneStats,
        RunStats& FareDisStats,
        FareDistributionResult& fareDisResult
    );

    void searchProcessor(const ProcessorData& data);
};