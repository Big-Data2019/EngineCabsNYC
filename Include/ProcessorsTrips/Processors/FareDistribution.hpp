#pragma once

#include "ProcessorsTrips/TripBatchProcessor.hpp"
#include "ProcessorsTrips/RunStats.hpp"
#include <cstdint>
#include <array>

struct FareDistributionResult{
	double validFares = 0.0;
	double fareSum = 0.0;
	double minFare = 0.0;
	double maxFare = 0.0;
	double averageFare = 0.0;

	double percentile25 = 0.0;
	double percentile50 = 0.0;
	double percentile75 = 0.0;
	double percentile99 = 0.0;
};

class FareDistribution final : public TripBatchProcessor{
private:
	static constexpr double bucketWidth = 5.0;
	static constexpr std::size_t regularBucket = 10;
	std::array<std::int64_t, 11> buckets{};
	std::vector<double> fares;

	FareDistributionResult& fareDisResult;
	RunStats& fareDisStats;

public:
	FareDistribution(FareDistributionResult& fareDisResult, RunStats& fareDisStats);

	arrow::Status process(std::int64_t batchSize, const std::string& file) override;

	void finish(const std::string& output) override;
};