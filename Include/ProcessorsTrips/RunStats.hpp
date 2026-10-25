#pragma once

#include <cstdint>

struct RunStats{
    std::int64_t files = 0;
    std::int64_t numRows = 0;
    std::int64_t batchCounter = 0;
    std::int64_t bytesProcessed = 0;
    std::int64_t numBatch = 0;

    double elapsedSeconds = 0.0;

    double megabytesPerSecond() const{
        return bytesProcessed / 1024.0 / 1024.0 / elapsedSeconds;
    }

    double rowsPerSecond() const{
        return numRows / elapsedSeconds;
    }
};