#pragma once
#include <arrow/api.h>
#include <filesystem>
#include <string>

struct BatchContext{
    std::filesystem::path filePath;
    int64_t batchIdx = 0;
};

class TripBatchProcessor{
public:
    virtual ~TripBatchProcessor() = default;

    virtual std::string name() const = 0;
    virtual std::vector<std::string> requiredColumns() const = 0;

    virtual arrow::Status process(
        const arrow::RecordBatch& batch,
        const BatchContext& context
    ) = 0;

    virtual void finish() = 0;
};