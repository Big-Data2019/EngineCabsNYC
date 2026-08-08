#pragma once
#include "Processors/BatchProcessor.hpp"
#include "Config/Config.hpp"

class RawCounter final : public TripBatchProcessor{
private:
    const Config& config;
    int64_t numRows = 0;

public:
    explicit RawCounter(const Config& _config);

    std::string name() const override;

    std::vector<std::string> requiredColumns() const override;

    arrow::Status process(
        const arrow::RecordBatch& batch,
        const BatchContext& context
    ) override;

    void finish() override;
};