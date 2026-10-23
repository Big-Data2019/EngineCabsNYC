#pragma once

#include <ProcessorsTrips/TripBatchProcessor.hpp>

struct InvalidTripStats{
    int8_t files = 0;
    int64_t numRows = 0;
    int64_t batchCounter = 0;
    int64_t bytesProcessed = 0;
    int64_t numBatch = 0;
    int64_t validTrips = 0;
    int64_t invalidTrips = 0;

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

    arrow::Status process(int64_t batchSize, const std::string& file) override;

    void finish(const std::string& output) override;
};