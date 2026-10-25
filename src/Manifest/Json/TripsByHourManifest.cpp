#include "ProcessorsTrips/Processors/TripsByHour.hpp"
#include "Manifest/JsonGenerator.hpp"
#include "Manifest/JsonBuilder.hpp"
#include <filesystem>

void TripsByHour::finish(const std::string& output){
    JsonBuilder data;
    std::vector<nlohmann::ordered_json> tripsHours;
    for(const auto& [hourId, trip] : tripsByHour){
        nlohmann::ordered_json tripHour;

        tripHour["hourId"] = hourId;
        tripHour["trips"] = trip;

        tripsHours.push_back(tripHour);
    }

    data.newData("Files", tripsHourStats.files);
    data.newData("Batches", tripsHourStats.batchCounter);
    data.newData("Input size", tripsHourStats.bytesProcessed);
    data.newData("Elapsed", tripsHourStats.elapsedSeconds);
    data.newData("Batch size", tripsHourStats.numBatch);
    data.newData("Rows/sec", tripsHourStats.rowsPerSecond());
    data.newData("Troughput", tripsHourStats.megabytesPerSecond());
    data.manyNewData("Trips by hour", tripsHours);

    std::filesystem::create_directories(output);
    const std::filesystem::path fileName = std::filesystem::path(output) / "TripsByHourManifest.json";

    JsonGenerator generator;
    generator.generateManifest(fileName.string(), data);
}