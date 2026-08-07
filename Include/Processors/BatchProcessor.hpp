#pragma once
#include <arrow/api.h>

class TripBatchProcessor{
public:
    virtual ~TripBatchProcessor() = default;

    virtual arrow::Status process() = 0;
};