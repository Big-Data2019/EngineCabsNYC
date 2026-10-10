#pragma once

#include "ProcessorsTrips/BatchProcessor.hpp"
#include "ProcessorsTrips/ProcessorData.hpp"
#include "ProcessorsTrips/Processors/RowCounter.hpp"
#include "ProcessorsTrips/Processors/RevenueByDay.hpp"
#include "ProcessorsTrips/Processors/TripsByHour.hpp"
#include "ProcessorsTrips/Processors/InvalidTripDetector.hpp"
#include "ProcessorsTrips/Processors/TopPickupZones.hpp"
#include "ProcessorsTrips/Processors/FareDistribution.hpp"
#include <unordered_map>
#include <string>
#include <memory>

class ProcessorRegister final{
private:
    std::unordered_map<std::string, std::unique_ptr<TripBatchProcessor>> processors;

public:
    void registerProcessors(RowStats& rowStats,
        RevenueDayStats& revenueDStats,
        TripsHourStats& tripsHStats,
        InvalidTripStats& invalidTStats,
        PickupZoneStats& pickupZStats,
        FareDistributionStats& fareDisStats
    );

    void searchProcessor(const ProcessorData& data);
};