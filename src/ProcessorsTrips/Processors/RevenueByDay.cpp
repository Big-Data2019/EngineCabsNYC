#include "ProcessorsTrips/Processors/RevenueByDay.hpp"

RevenueByDay::RevenueByDay(RevenueDayStats& stats_)
    : stats(stats_)
{}

arrow::Status RevenueByDay::process(const int64_t& batchSize,
                                    const std::vector<std::string>& files){
    return arrow::Status::OK();
}

void RevenueByDay::finish(const std::string& output){
    
}