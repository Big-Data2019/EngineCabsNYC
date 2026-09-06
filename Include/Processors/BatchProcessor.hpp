#pragma once
#include <arrow/api.h>
#include <string>
#include <filesystem>

class TripBatchProcessor{
public:
    virtual ~TripBatchProcessor() = default;

    virtual arrow::Status process(
        const int64_t& batchSize, 
        const std::vector<std::string>& files
    ) = 0;

    virtual void finish(const std::filesystem::path& output) = 0;
};