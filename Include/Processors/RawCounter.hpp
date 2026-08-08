#pragma once
#include "Processors/BatchProcessor.hpp"

class RawCounter final : public TripBatchProcessor{
private:
    int64_t numRows = 0;

public:
    std::string name() const override;

    std::vector<std::string> requiredColumns() const override;

    arrow::Status process(
        const arrow::RecordBatch& batch,
        const BatchContext& context
    ) override;

    void finish() override;
};