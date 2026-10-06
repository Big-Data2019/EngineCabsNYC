#include "ProcessorsTrips/Processors/TripsByHour.hpp"

TripsByHour::TripsByHour(TripsHourStats& stats_)
    : tripsHStats(stats_)
{}

arrow::Status TripsByHour::process(const int64_t& batchSize, const std::vector<std::string>& files)
{
    return arrow::Status::OK();
}

void TripsByHour::finish(const std::string& output){

}