#pragma once
#include <arrow/api.h>

class TripBatchProcessor{
public:
    virtual ~TripBatchProcessor() = default;

    virtual arrow::Status process(const arrow::RecordBatch& batch) = 0;

    virtual void finish() = 0;
};