#pragma once
#include <arrow/api.h>
#include <string>

class TripBatchProcessor{
public:
    virtual ~TripBatchProcessor() = default;

    virtual std::string name() const = 0;

    virtual arrow::Status process() = 0;

    virtual void finish() = 0;
};