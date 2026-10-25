#include "ProcessorsTrips/Processors/RevenueByDay.hpp"
#include "Manifest/JsonGenerator.hpp"
#include "Manifest/JsonBuilder.hpp"

void RevenueByDay::finish(const std::string& output){
    JsonBuilder data;
    std::vector<nlohmann::ordered_json> days;
    for(const auto& [dayId, revenue] : Accumulator){
        nlohmann::ordered_json day;

        day["dayId"] = dayId;
        day["Revenue"] = revenue;

        days.push_back(day);
    }

    data.newData("Files", revenueDayStats.files);
    data.newData("Batches", revenueDayStats.batchCounter);
    data.newData("Input size", revenueDayStats.bytesProcessed);
    data.newData("Elapsed", revenueDayStats.elapsedSeconds);
    data.newData("Batch size", revenueDayStats.numBatch);
    data.newData("Rows/sec", revenueDayStats.rowsPerSecond());
    data.newData("Troughput", revenueDayStats.megabytesPerSecond());
    data.manyNewData("Revenue by day", days);

    std::filesystem::create_directories(output);
    const std::filesystem::path fileName = std::filesystem::path(output) / "RevenueDayManifest.json";

    JsonGenerator generator;
    generator.generateManifest(fileName.string(), data);
}