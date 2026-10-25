#pragma once

#include "ProcessorsTrips/TripBatchProcessor.hpp"
#include "ProcessorsTrips/RunStats.hpp"
#include <unordered_map>
#include <cstdint>

class TopPickupZones final : public TripBatchProcessor{
private:
	std::unordered_map<std::int32_t, std::int64_t> pickUpZone;
	RunStats& pickupZoneStats;

public:
	explicit TopPickupZones(RunStats& pickupZoneStats);

	arrow::Status process(std::int64_t batchSize, const std::string& file) override;

	void finish(const std::string& output) override;		
};