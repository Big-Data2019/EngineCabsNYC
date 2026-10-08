#pragma once

#include <ProcessorsTrips/BatchProcessor.hpp>

struct InvalidTripStats{
    int64_t files = 0;
    int64_t numRows = 0;
    int64_t batchCounter = 0;
    int64_t bytesProcessed = 0;
    int64_t numBatch = 0;

    double elapsedSeconds = 0.0;
    double rowsPerSecond() const{
        return numRows / elapsedSeconds;
    }

    double megabytesPerSecond() const{
        return bytesProcessed / 1024.0 / 1024.0 / elapsedSeconds;
    }
};

class InvalidTripDetector final : public TripBatchProcessor{
private:
    InvalidTripStats& invalidTStats;

public:
    InvalidTripDetector(InvalidTripStats& invalidTStats);

    arrow::Status process(
        const int64_t& batchSize,
        const std::vector<std::string>& files
    ) override;

    void finish(const std::string& output) override;
};