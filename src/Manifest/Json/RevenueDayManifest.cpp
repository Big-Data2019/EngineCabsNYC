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

    data.newData("Files", revenueDStats.files);
    data.newData("Batches", revenueDStats.batchCounter);
    data.newData("Input size", revenueDStats.bytesProcessed);
    data.newData("Elapsed", revenueDStats.elapsedSeconds);
    data.newData("Batch size", revenueDStats.numBatch);
    data.newData("Rows/sec", revenueDStats.rowsPerSecond());
    data.newData("Troughput", revenueDStats.megabytesPerSecond());
    data.manyNewData("Revenue by day", days);

    std::filesystem::create_directories(output);
    const std::filesystem::path fileName = std::filesystem::path(output) / "RevenueDayManifest.json";

    JsonGenerator generator;
    generator.generateManifest(fileName.string(), data);
}