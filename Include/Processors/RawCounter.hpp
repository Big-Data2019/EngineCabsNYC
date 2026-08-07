#pragma once
#include "Processors/BatchProcessor.hpp"
#include "Config/Config.hpp"

class RawCounter final : public TripBatchProcessor{
private:
    const Config& config;

public:
    explicit RawCounter(const Config& _config);
    arrow::Status process() override;
};