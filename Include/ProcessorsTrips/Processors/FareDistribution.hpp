#pragma once

#include "ProcessorsTrips/BatchProcessor.hpp"
#include <array>

struct FareDistributionStats{
	int8_t files = 0;
    int64_t numRows = 0;
	int64_t batchCounter = 0;
    int64_t bytesProcessed = 0;
	int64_t numBatch = 0;

	double validFares = 0.0;
	double fareSum = 0.0;
	double minFare = 0.0;
	double maxFare = 0.0;
	double averageFare = 0.0;

	double percentile25 = 0.0;
	double percentile50 = 0.0;
	double percentile75 = 0.0;
	double percentile99 = 0.0;

	double elapsedSeconds = 0.0;
    double rowsPerSecond() const{
		return numRows / elapsedSeconds;
    }

    double megabytesPerSecond() const{
        return bytesProcessed / 1024.0 / 1024.0 / elapsedSeconds;
    }
};

class FareDistribution final : public TripBatchProcessor{
private:
	static constexpr double bucketWidth = 5.0;
	static constexpr std::size_t regularBucket = 10;
	std::array<int64_t, 11> buckets{};
	std::vector<double> fares;

	FareDistributionStats& fareDisStats;

public:
	FareDistribution(FareDistributionStats& fareDisStats);

	arrow::Status process(
		const int64_t& batchSize,
		const std::vector<std::string>& files	
	) override;

	void finish(const std::string& output) override;
};